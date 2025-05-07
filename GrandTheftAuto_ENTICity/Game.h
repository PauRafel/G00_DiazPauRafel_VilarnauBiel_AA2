#pragma once
#include "Map.h"
#include <vector>
#include "Pedestrian.h"

class Game {
public:

    Game();
    ~Game();

    void run(); 

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
};