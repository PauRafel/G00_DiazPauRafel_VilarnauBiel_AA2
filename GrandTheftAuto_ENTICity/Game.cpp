#include "Game.h"
#include <iostream>
#include <windows.h> 
#include "Config.h"  
#include "Map.h"     
#include "Player.h"  
#include "Pedestrian.h" 

Game::Game() {
    isRunning = true;
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
    std::cout << "Prueba iniciando el juego..." << std::endl;

    ConfigData config = loadConfig("config.txt");
    std::cout << "Tamaño del mapa: " << config.mapWidth << " x " << config.mapHeight << std::endl;
    std::cout << "Peatones en Los Santos: " << config.pedestriansLosSantos << std::endl;
}

void Game::update() {
   
    if (GetAsyncKeyState(VK_ESCAPE)) {
        isRunning = false;
    }
}

void Game::render() {
    
}