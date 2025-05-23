//Pau Rafel Díaz Hernández & Biel Vilarnau Espínola ENTI 2025
#pragma once
#include <vector>
#include "Map.h"
#include "Pedestrian.h"

class Game {
public:

    Game();
    ~Game();

    void run(); 
    int money; 
    static const int NUM_FPS = 10;

private:
    bool isRunning;

    Map* map;
    int playerX, playerY;

    std::vector<Pedestrian> pedestrians;
    
    void init();
    void update();
    void render();

    void spawnPedestrians(int numPeatones);
    void updatePedestrians();

    void HandleInput(); 
};