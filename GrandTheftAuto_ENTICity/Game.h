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
    int playerHP;
    int playerAttack;
    static const int NUM_FPS = 10;
    Direction playerDirection;

private:
    bool isRunning;

    Map* map;
    int playerX, playerY;
    bool paidTollToSanFierro;
    bool paidTollToLasVenturas;
    bool inCar;
    bool CanMoveTo(int x, int y);

    std::vector<Pedestrian> pedestrians;
    
    void init();
    void update();
    void render();

    void spawnPedestrians(const IslandConfig& config, int xMin, int xMax);
    void updatePedestrians();

    void HandleInput();

    void GameOver();
};