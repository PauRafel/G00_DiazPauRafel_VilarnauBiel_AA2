#pragma once

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
};