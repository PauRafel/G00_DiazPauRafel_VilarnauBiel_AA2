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
   
    if (GetAsyncKeyState(VK_ESCAPE)) {
        isRunning = false;
    }
}

void Game::render() {
    map->render(playerX, playerY, 20, 10);
}

Game::~Game() {
    if (map != nullptr) {
        delete map;
    }
}