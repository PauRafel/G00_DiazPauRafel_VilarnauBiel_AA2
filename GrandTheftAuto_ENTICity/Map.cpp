#include "Map.h"
#include <iostream>
#include <windows.h> 

Map::Map(int width, int height) {
    this->width = width;
    this->height = height;

    data = new char* [height];
    for (int i = 0; i < height; ++i) {
        data[i] = new char[width];
    }
}

Map::~Map() {
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

void Map::initialize() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            data[y][x] = ' ';
        }
    }

    for (int x = 0; x < width; ++x) {
        data[0][x] = 'X';
        data[height - 1][x] = 'X';
    }
    for (int y = 0; y < height; ++y) {
        data[y][0] = 'X';
        data[y][width - 1] = 'X';
    }
}

void Map::render(int playerX, int playerY, int viewWidth, int viewHeight) {
    system("cls"); 

    int startX = playerX - viewWidth / 2;
    int startY = playerY - viewHeight / 2;

    for (int y = 0; y < viewHeight; ++y) {
        for (int x = 0; x < viewWidth; ++x) {
            int mapX = startX + x;
            int mapY = startY + y;

            if (mapX >= 0 && mapX < width && mapY >= 0 && mapY < height) {
                std::cout << data[mapY][mapX];
            }
            else {
                std::cout << ' '; 
            }
        }
        std::cout << std::endl;
    }
}

char** Map::getData() {
    return data;
}