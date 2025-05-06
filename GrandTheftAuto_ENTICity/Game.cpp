#include "Game.h"
#include <iostream>
#include <windows.h> 
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

    while (isRunning) {
        update();
        render();
        Sleep(100); 
    }
}

void Game::init() {
    std::cout << "Iniciando el juego..." << std::endl;

    ConfigData config = loadConfig("config.txt");

    map = new Map(config.mapWidth, config.mapHeight);
    map->initialize();

    map->getData()[playerY][playerX] = 'v';
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

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        isRunning = false;
    }
}

void Game::render() {
    map->render(playerX, playerY, 20, 10);
}   