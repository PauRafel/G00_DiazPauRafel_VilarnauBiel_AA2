//Pau Rafel Díaz Hernández & Biel Vilarnau Espínola ENTI 2025
#pragma once
#include <vector>
#include "Map.h"
#include "Pedestrian.h"
#include "Direction.h"

class Game {
public:

    Game();
    ~Game();

    void run(); 
    int money; 
    static const int NUM_FPS = 10;
    Direction playerDirection;

private:
    bool isRunning;

    Map* map;
    int playerX, playerY;
    bool paidTollToSanFierro;
    bool paidTollToLasVenturas;

    std::vector<Pedestrian> pedestrians;
    
    void init();
    void update();
    void render();

    void spawnPedestrians(int numPeatones);
    void updatePedestrians();

    void HandleInput();

    void GameOver();
};