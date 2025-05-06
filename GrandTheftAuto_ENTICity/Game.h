#pragma once
#include "Map.h"

class Game {
public:

    Game();
    ~Game();

    void run(); 

private:

    void init();
    void update();
    void render();

    bool isRunning;

    Map* map;
    int playerX, playerY;
};