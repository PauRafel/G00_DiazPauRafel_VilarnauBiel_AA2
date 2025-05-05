#pragma once
#include "Map.h"

class Game {
public:
    Game();
    ~Game();

    void run(); 

private:
    Map* map;
    int playerX, playerY;
    bool isRunning;

    void init(); 
    void update(); 
    void render();
};