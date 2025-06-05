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
    state = GameState::TitleScreen;

    playerAttack = 0;
    playerHP = 0;

    bigSmoke = { 0, 0, 0, 0, false, PedestrianType::Neutral };
    bigSmokeAlive = false;

    money = 0;
    isRunning = true;
    map = nullptr;

    playerDirection = Direction::Down;
    playerX = 1;
    playerY = 1;

    paidTollToSanFierro = false;
    paidTollToLasVenturas = false;

    inCar = false;
}

Game::~Game() {
    if (map != nullptr) {
        delete map;
    }
}

void Game::run() {
    while (true) {
        system("cls");

        switch (state) {
        case GameState::TitleScreen:
            ShowTitleScreen();
            break;

        case GameState::MainMenu:
            ShowMainMenu();
            break;

        case GameState::Playing:
            HandleInput();
            update();
            render();
            Sleep(1000 / NUM_FPS); 
            break;

        case GameState::GameOver:
            ShowGameOverScreen();
            return;

        case GameState::Victory:
            ShowVictoryScreen();
            return;
        }
    }
}


void Game::init() {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::cout << "Iniciando el juego..." << std::endl;
    ConfigData config = loadConfig("config.txt");

    map = new Map(config.mapWidth, config.mapHeight);
    map->initialize();

    int mapW = map->getWidth();
    int mapH = map->getHeight();

    int islandStart = mapW / 3;
    int islandEnd = 2 * mapW / 3;

    playerX = (islandStart + islandEnd) / 2;
    playerY = mapH / 2;

    map->GetData()[playerY][playerX].type = CellType::Player;

    
    playerHP = config.playerHP;
    playerAttack = config.playerAttack;

    spawnPedestrians(config.losSantos, 1, mapW / 3);
    spawnPedestrians(config.sanFierro, mapW / 3 + 1, 2 * mapW / 3);
    spawnPedestrians(config.lasVenturas, 2 * mapW / 3 + 1, mapW - 1);

    bool placed = false;
    int w = map->getWidth();
    int h = map->getHeight();

    while (!placed) {
        int x = rand() % (w / 3) + 2 * (w / 3);
        int y = rand() % h;

        if (map->GetData()[y][x].type == CellType::Empty) {
            bigSmoke.x = x;
            bigSmoke.y = y;
            bigSmoke.hp = 300;
            bigSmoke.attack = 30;
            bigSmoke.isAlive = true;
            bigSmoke.type = PedestrianType::Aggressive;

            bigSmokeAlive = true;
            map->GetData()[y][x].type = CellType::Pedestrian;
            placed = true;
        }
    }

   
    for (int i = 0; i < 3; ++i) {
        bool placed = false;
        while (!placed) {
            int zoneStart = i * (mapW / 3);
            int zoneEnd = (i + 1) * (mapW / 3);
            int x = rand() % (zoneEnd - zoneStart) + zoneStart;
            int y = rand() % map->getHeight();

            if (map->GetData()[y][x].type == CellType::Empty) {
                map->GetData()[y][x].hasCar = true;
                placed = true;
            }
        }
    }
}


void Game::update() {
    bool moved = false;

    if (bigSmokeAlive && std::abs(bigSmoke.x - playerX) + std::abs(bigSmoke.y - playerY) == 1) {
        bigSmoke.hp -= playerAttack;
        std::cout << "[CJ] Atacas a Big Smoke!" << std::endl;

        if (bigSmoke.hp <= 0) {
            bigSmokeAlive = false;
            map->GetData()[bigSmoke.y][bigSmoke.x].type = CellType::Empty;
            std::cout << "Has derrotado a Big Smoke! GANASTE EL JUEGO! " << std::endl;
            Sleep(3000);
            ShowVictoryScreen(); 
        }
        else {
            playerHP -= bigSmoke.attack;
            std::cout << "[BIG SMOKE] ¡Te contraataca! Vida restante: " << playerHP << std::endl;
            if (playerHP <= 0) {
                std::cout << " CJ ha muerto en el duelo con Big Smoke... GAME OVER " << std::endl;
                Sleep(3000);
                ShowGameOverScreen();
            }
        }

        Sleep(200);
        return;
    }


    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        for (auto& p : pedestrians) {
            if (!p.isAlive) continue;

            int dx = std::abs(p.x - playerX);
            int dy = std::abs(p.y - playerY);

            if ((dx + dy) == 1) { 
                p.hp -= playerAttack;
                std::cout << "[CJ] Atacas a un peaton!" << std::endl;

                if (p.hp <= 0) {
                    p.isAlive = false;
                    map->GetData()[p.y][p.x].type = CellType::Money;
                    std::cout << "[CJ] El peaton ha muerto y dejo dinero en el suelo!" << std::endl;
                    Sleep(2500);
                }
                else if (p.type == PedestrianType::Aggressive) {
                    playerHP -= p.attack;
                    std::cout << "[PEATÓN] Te ha contraatacado! Vida restante: " << playerHP << std::endl;
                    Sleep(2500);

                    if (playerHP <= 0) {
                        std::cout << " CJ ha muerto en combate... GAME OVER " << std::endl;
                        Sleep(5000);
                        ShowGameOverScreen();
                    }
                }

                break; 
            }
        }

        Sleep(200); 
    }


    updatePedestrians();

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        isRunning = false;
    }
}

void Game::HandleInput() {
    int prevX = playerX;
    int prevY = playerY;
    map->GetData()[playerY][playerX].type = CellType::Empty;

    int step = inCar ? 2 : 1;
    int mapW = map->getWidth();
    int mapH = map->getHeight();

    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        bool canMove = true;
        for (int i = 1; i <= step; ++i) {
            if (playerY - i < 0 || map->GetData()[playerY - i][playerX].type == CellType::Wall || map->GetData()[playerY - i][playerX].hasCar) {
                canMove = false;
                break;
            }
        }
        if (canMove) {
            playerY -= step;
            playerDirection = Direction::Up;
        }
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        bool canMove = true;
        for (int i = 1; i <= step; ++i) {
            if (playerY + i >= mapH || map->GetData()[playerY + i][playerX].type == CellType::Wall || map->GetData()[playerY + i][playerX].hasCar) {
                canMove = false;
                break;
            }
        }
        if (canMove) {
            playerY += step;
            playerDirection = Direction::Down;
        }
    }
    else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        bool canMove = true;
        for (int i = 1; i <= step; ++i) {
            if (playerX - i < 0 || map->GetData()[playerY][playerX - i].type == CellType::Wall || map->GetData()[playerY][playerX - i].hasCar) {
                canMove = false;
                break;
            }
        }
        if (canMove) {
            playerX -= step;
            playerDirection = Direction::Left;
        }
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        bool canMove = true;
        for (int i = 1; i <= step; ++i) {
            if (playerX + i >= mapW || map->GetData()[playerY][playerX + i].type == CellType::Wall || map->GetData()[playerY][playerX + i].hasCar) {
                canMove = false;
                break;
            }
        }
        if (canMove) {
            playerX += step;
            playerDirection = Direction::Right;
        }
    }

    
    if (inCar) {
        CheckAtropello(prevX, prevY, playerX, playerY);
    }

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
                Sleep(3000);
            }
            else {
                std::cout << "[CJ] No tienes suficiente dinero para cruzar.\n";
                Sleep(3000);
                GameOver();
            }
        }
        else if (playerX == wall2 && !paidTollToLasVenturas) {
            if (money >= config.tollToLasVenturas) {
                money -= config.tollToLasVenturas;
                currentCell.type = CellType::Empty;
                paidTollToLasVenturas = true;
                std::cout << "[CJ] Has pagado el peaje a Las Venturas.\n";
                Sleep(3000);
            }
            else {
                std::cout << "[CJ] No tienes suficiente dinero para cruzar.\n";
                Sleep(3000);
                GameOver();
            }
        }
    }

  
    if (GetAsyncKeyState(0x45) & 0x8000) {
        if (!inCar) {
            bool foundCar = false;
            int carX = -1, carY = -1;

            int adjacentCells[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

            for (int i = 0; i < 4; i++) {
                int checkX = playerX + adjacentCells[i][0];
                int checkY = playerY + adjacentCells[i][1];

                if (checkX >= 0 && checkX < mapW && checkY >= 0 && checkY < mapH) {
                    if (map->GetData()[checkY][checkX].hasCar &&
                        map->GetData()[checkY][checkX].type == CellType::Empty) {
                        foundCar = true;
                        carX = checkX;
                        carY = checkY;
                        break;
                    }
                }
            }

            if (foundCar) {
                inCar = true;
                map->GetData()[carY][carX].hasCar = false;
                std::cout << "[CJ] Has subido al coche.\n";
                Sleep(1000);
            }
            else {
                std::cout << "[CJ] No hay ningun coche cerca.\n";
                Sleep(1000);
            }
        }
        else if (inCar) {
            bool foundSpot = false;
            int spawnX = -1, spawnY = -1;

            int adjacentCells[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

            for (int i = 0; i < 4; i++) {
                int checkX = playerX + adjacentCells[i][0];
                int checkY = playerY + adjacentCells[i][1];

                if (checkX >= 0 && checkX < mapW && checkY >= 0 && checkY < mapH) {
                    if (map->GetData()[checkY][checkX].type == CellType::Empty &&
                        !map->GetData()[checkY][checkX].hasCar) {
                        foundSpot = true;
                        spawnX = checkX;
                        spawnY = checkY;
                        break;
                    }
                }
            }

            if (foundSpot) {
                inCar = false;
                currentCell.hasCar = true;
                map->GetData()[playerY][playerX].type = CellType::Empty;
                playerX = spawnX;
                playerY = spawnY;
                std::cout << "[CJ] Has salido del coche.\n";
                Sleep(1000);
            }
            else {
                std::cout << "[CJ] No hay espacio para salir del coche.\n";
                Sleep(1000);
            }
        }
    }


   
    if (!inCar && currentCell.type == CellType::Money) {
        money += rand() % 100 + 1;
        currentCell.type = CellType::Empty;
        std::cout << "[CJ] Has recogido dinero del suelo!\n";
        Sleep(500);
    }
    else if (inCar && currentCell.type == CellType::Money) {
        std::cout << "[CJ] No puedes recoger dinero mientras conduces.\n";
        Sleep(500);
    }

   
    currentCell.type = CellType::Player;
}

void Game::render() {
    map->render(playerX, playerY, 20, 10, playerDirection, &bigSmoke, bigSmokeAlive);
    std::cout << "Dinero: $" << money << std::endl;
}

void Game::spawnPedestrians(const IslandConfig& config, int xMin, int xMax) {
    for (int i = 0; i < config.numPedestrians; ++i) {
        Pedestrian p;

        bool placed = false;
        while (!placed) {
            p.x = rand() % (xMax - xMin) + xMin;
            p.y = rand() % map->getHeight();

            if (map->GetData()[p.y][p.x].type == CellType::Empty) {
                p.hp = config.pedestrianHP;
                p.attack = config.pedestrianAttack;
                p.isAlive = true;
                p.type = (rand() % 100 < 25) ? PedestrianType::Aggressive : PedestrianType::Neutral;

                pedestrians.push_back(p);
                map->GetData()[p.y][p.x].type = CellType::Pedestrian;
                placed = true;
            }
        }
    }
}


void Game::updatePedestrians() {
    for (auto& p : pedestrians) {
        if (!p.isAlive) continue;

        if (abs(playerX - p.x) <= 1 && abs(playerY - p.y) <= 1) continue;

       
        map->GetData()[p.y][p.x].type = CellType::Empty;

       
        int dir = rand() % 4;
        int newX = p.x;
        int newY = p.y;

        switch (dir) {
        case 0: newX--; break; 
        case 1: newX++; break; 
        case 2: newY--; break; 
        case 3: newY++; break; 
        }

       
        if (newX >= 0 && newX < map->getWidth() &&
            newY >= 0 && newY < map->getHeight() &&
            map->GetData()[newY][newX].type == CellType::Empty &&
            !map->GetData()[newY][newX].hasCar) { 

            p.x = newX;
            p.y = newY;
        }

        map->GetData()[p.y][p.x].type = CellType::Pedestrian;
    }
}


void Game::GameOver() {
    system("cls");
    std::cout << " CJ ha sido arrestado por no pagar el peaje " << std::endl;
    std::cout << "GAME OVER" << std::endl;
    Sleep(7000);
    isRunning = false;
    exit(0);
}

bool Game::CanMoveTo(int targetX, int targetY) {
    if (targetX < 0 || targetX >= map->getWidth() || targetY < 0 || targetY >= map->getHeight())
        return false;

    CellType targetType = map->GetData()[targetY][targetX].type;
    bool hasCar = map->GetData()[targetY][targetX].hasCar;

    return targetType != CellType::Wall && !hasCar; 
}

void Game::CheckAtropello(int startX, int startY, int endX, int endY) {
    int dx = (endX > startX) ? 1 : (endX < startX ? -1 : 0);
    int dy = (endY > startY) ? 1 : (endY < startY ? -1 : 0);

    int x = startX;
    int y = startY;

    int dxSteps = std::abs(endX - startX);
    int dySteps = std::abs(endY - startY);
    int steps = (dxSteps > dySteps) ? dxSteps : dySteps;

    for (int i = 0; i < steps; ++i) {
        x += dx;
        y += dy;

        if (bigSmokeAlive && x == bigSmoke.x && y == bigSmoke.y) {
            std::cout << "[CJ] No puedes atropellar a Big Smoke!" << std::endl;
            Sleep(500);
            continue;
        }

        for (auto& p : pedestrians) {
            if (!p.isAlive) continue;
            if (p.x == x && p.y == y) {
                p.isAlive = false;
                map->GetData()[p.y][p.x].type = CellType::Money;
                std::cout << "[CJ] Atropellaste a un peaton!" << std::endl;
                Sleep(500);
                break;
            }
        }
    }
}

void Game::ShowTitleScreen() {
    std::cout << "==============================" << std::endl;
    std::cout << "     GTA ENTI CITY 2D         " << std::endl;
    std::cout << "By:                           " << std::endl;
    std::cout << "  Pau Rafel Diaz Hernandez    " << std::endl;
    std::cout << "  Biel Vilarnau Espinola      " << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "Cargando..." << std::endl;
    Sleep(3000);  
    state = GameState::MainMenu;
}

void Game::ShowMainMenu() {
    std::cout << "=== MENU PRINCIPAL ===" << std::endl;
    std::cout << "1. Jugar" << std::endl;
    std::cout << "2. Salir" << std::endl;
    std::cout << "Selecciona una opcion: ";

    char option;
    std::cin >> option;

    if (option == '1') {
        init(); 
        state = GameState::Playing;
    }
    else {
        exit(0);
    }
}

void Game::ShowGameOverScreen() {
    std::cout << " GAME OVER " << std::endl;
    std::cout << "CJ ha muerto en la lucha." << std::endl;
    system("pause");
    exit(0);
}

void Game::ShowVictoryScreen() {
    std::cout << " VICTORIA! " << std::endl;
    std::cout << "Has derrotado a Big Smoke y liberado Las Venturas." << std::endl;
    system("pause");
    exit(0);
}