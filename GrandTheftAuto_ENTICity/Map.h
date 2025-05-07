#pragma once
#include "Config.h"

class Map {
public:
    Map(int width, int height);
    ~Map();

    void initialize();    
    void render(int playerX, int playerY, int viewWidth, int viewHeight); 
    char** getData() { return data; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }


private:
    int width;
    int height;
    char** data;          
};