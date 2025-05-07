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
    isRunning = true;
    map = nullptr;
    playerX = 1;
    playerY = 1;
}

Game::~Game() {
    if (map != nullptr) {
        delete map;
    }
}

void Game::run() {
    init();
    money = 0;

    while (isRunning) {
        update();
        render();
        Sleep(100); 
    }
}

void Game::init() {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::cout << "Iniciando el juego..." << std::endl;
    ConfigData config = loadConfig("config.txt");

    map = new Map(config.mapWidth, config.mapHeight);
    map->initialize();

    map->getData()[playerY][playerX] = 'v';

    spawnPedestrians(config.pedestriansLosSantos);
}

void Game::update() {
    map->getData()[playerY][playerX] = ' ';

    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        if (map->getData()[playerY - 1][playerX] != 'X') { 
            playerY--;
        }
        map->getData()[playerY][playerX] = '^'; 
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        if (map->getData()[playerY + 1][playerX] != 'X') {
            playerY++;
        }
        map->getData()[playerY][playerX] = 'v';
    }
    else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        if (map->getData()[playerY][playerX - 1] != 'X') {
            playerX--;
        }
        map->getData()[playerY][playerX] = '<';
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        if (map->getData()[playerY][playerX + 1] != 'X') {
            playerX++;
        }
        map->getData()[playerY][playerX] = '>';
    }
    else {
        
        map->getData()[playerY][playerX] = 'v'; 
    }

    
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        
        for (auto& p : pedestrians) {
            if (!p.isAlive) continue;

            if ((abs(p.x - playerX) + abs(p.y - playerY)) == 1) { 
                p.isAlive = false;

                
                map->getData()[p.y][p.x] = '$';
                break; 
            }
        }
    }

    updatePedestrians();

    
    if (map->getData()[playerY][playerX] == '$') {
        money += rand() % 100 + 1; 
        map->getData()[playerY][playerX] = ' '; 
    }

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        isRunning = false;
    }
}

void Game::render() {
    map->render(playerX, playerY, 20, 10);
    std::cout << "Dinero: $" << money << std::endl;
}

void Game::spawnPedestrians(int numPeatones){
    for (int i = 0; i < numPeatones; ++i) {
        Pedestrian p;

        do {
            p.x = rand() % map->getWidth();
            p.y = rand() % map->getHeight();
        } while (map->getData()[p.y][p.x] != ' ');

        p.movesHorizontally = (rand() % 2 == 0);
        p.isAlive = true;

        pedestrians.push_back(p);

        map->getData()[p.y][p.x] = 'P';
    }
}

void Game::updatePedestrians(){
    for (auto& p : pedestrians) {
        if (!p.isAlive) continue;

        if (abs(playerX - p.x) <= 1 && abs(playerY - p.y) <= 1) continue;

        map->getData()[p.y][p.x] = ' ';

        int move = (rand() % 2 == 0) ? -1 : 1;

        if (p.movesHorizontally) {
            if (map->getData()[p.y][p.x + move] == ' ') {
                p.x += move;
            }
        }
        else {
            if (map->getData()[p.y + move][p.x] == ' ') {
                p.y += move;
            }
        }

        map->getData()[p.y][p.x] = 'P';
    }

}