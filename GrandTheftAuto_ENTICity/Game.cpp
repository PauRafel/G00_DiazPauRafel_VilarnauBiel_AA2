//Pau Rafel Díaz Hernández & Biel Vilarnau Espínola ENTI 2025
#include <iostream>
#include <windows.h> 
#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "Config.h"  
#include "Map.h"     
#include "Player.h"  
#include "Pedestrian.h" 

Game::Game() {
    money = 0;  
    isRunning = true;
    map = nullptr;
    playerDirection = Direction::Down;
    playerX = 1;
    playerY = 1;
    paidTollToSanFierro = false;
    paidTollToLasVenturas = false;
}

Game::~Game() {
    if (map != nullptr) {
        delete map;
    }
}

void Game::run() {
    init();

    while (isRunning) {
        HandleInput();
        update();
        render();
        Sleep(1000 / NUM_FPS);
    }
}

void Game::init() {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::cout << "Iniciando el juego..." << std::endl;
    ConfigData config = loadConfig("config.txt");

    map = new Map(config.mapWidth, config.mapHeight);
    map->initialize();

    map->GetData()[playerY][playerX].type = CellType::Player;

    spawnPedestrians(config.losSantos.numPedestrians);
}

void Game::update() {

    bool moved = false;

    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        for (auto& p : pedestrians) {
            if (!p.isAlive) continue;
            if ((abs(p.x - playerX) + abs(p.y - playerY)) == 1) {
                p.isAlive = false;
                map->GetData()[p.y][p.x].type = CellType::Money;

                bool placed = false;
                while (!placed) {
                    int newX = rand() % map->getWidth();
                    int newY = rand() % map->getHeight();

                    if (map->GetData()[newY][newX].type == CellType::Empty) {
                        p.x = newX;
                        p.y = newY;
                        p.isAlive = true;
                        p.movesHorizontally = rand() % 2 == 0;
                        map->GetData()[p.y][p.x].type = CellType::Pedestrian;
                        placed = true;
                    }
                }

                break;
            }
        }
    }


    updatePedestrians();

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        isRunning = false;
    }
}

void Game::HandleInput() {
    map->GetData()[playerY][playerX].type = CellType::Empty;

    char newSymbol = 'v'; 

    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        if (map->GetData()[playerY - 1][playerX].type != CellType::Wall) {
            playerY--;
            playerDirection = Direction::Up;
        }
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        if (map->GetData()[playerY + 1][playerX].type != CellType::Wall) {
            playerY++;
            playerDirection = Direction::Down;
        }
    }
    else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        if (map->GetData()[playerY][playerX - 1].type != CellType::Wall) {
            playerX--;
            playerDirection = Direction::Left;
        }
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        if (map->GetData()[playerY][playerX + 1].type != CellType::Wall) {
            playerX++;
            playerDirection = Direction::Right;
        }
    }

    if (map->GetData()[playerY][playerX].type == CellType::Money) {
        money += rand() % 100 + 1;
        map->GetData()[playerY][playerX].type = CellType::Empty;
    }

    map->GetData()[playerY][playerX].type = CellType::Player;

    Cell& currentCell = map->GetData()[playerY][playerX];

    if (currentCell.type == CellType::Toll) {

        int wall1 = map->getWidth() / 3;
        int wall2 = 2 * map->getWidth() / 3;

        ConfigData config = loadConfig("config.txt");

        if (playerX == wall1 && !paidTollToSanFierro) {
            if (money >= config.tollToSanFierro) {
                money -= config.tollToSanFierro;
                currentCell.type = CellType::Empty;
                paidTollToSanFierro = true;
                std::cout << "[CJ] Has pagado el peaje a San Fierro.\n";
                Sleep(500);
            }
            else {
                std::cout << "[CJ] No tienes suficiente dinero para cruzar.\n";
                Sleep(1000);
                GameOver();
            }
        }
        else if (playerX == wall2 && !paidTollToLasVenturas) {
            if (money >= config.tollToLasVenturas) {
                money -= config.tollToLasVenturas;
                currentCell.type = CellType::Empty;
                paidTollToLasVenturas = true;
                std::cout << "[CJ] Has pagado el peaje a Las Venturas.\n";
                Sleep(500);
            }
            else {
                std::cout << "[CJ] No tienes suficiente dinero para cruzar.\n";
                Sleep(1000);
                GameOver();
            }
        }
    }

}

void Game::render() {
    map->render(playerX, playerY, 20, 10, playerDirection);
    std::cout << "Dinero: $" << money << std::endl;
}

void Game::spawnPedestrians(int numPeatones){
    for (int i = 0; i < numPeatones; ++i) {
        Pedestrian p;

        do {
            p.x = rand() % map->getWidth();
            p.y = rand() % map->getHeight();
        } while (map->GetData()[p.y][p.x].type != CellType::Empty);

        p.movesHorizontally = (rand() % 2 == 0);
        p.isAlive = true;

        pedestrians.push_back(p);

        map->GetData()[p.y][p.x].type = CellType::Pedestrian;
    }
}

void Game::updatePedestrians(){
    for (auto& p : pedestrians) {
        if (!p.isAlive) continue;

        if (abs(playerX - p.x) <= 1 && abs(playerY - p.y) <= 1) continue;

        map->GetData()[p.y][p.x].type = CellType::Empty;

        int move = (rand() % 2 == 0) ? -1 : 1;

        if (p.movesHorizontally) {
            if (map->GetData()[p.y][p.x + move].type == CellType::Empty) {
                p.x += move;
            }
        }
        else {
            if (map->GetData()[p.y + move][p.x].type == CellType::Empty) {
                p.y += move;
            }
        }

        map->GetData()[p.y][p.x].type = CellType::Pedestrian;
    }
}

void Game::GameOver() {
    system("cls");
    std::cout << " CJ ha sido arrestado por no pagar el peaje " << std::endl;
    std::cout << "GAME OVER" << std::endl;
    Sleep(5000);
    isRunning = false;
}
