//Pau Rafel Díaz Hernández & Biel Vilarnau Espínola ENTI 2025
#include <iostream>
#include <windows.h> 
#include "Map.h"
#include "Direction.h"

Map::Map(int width, int height) {
    this->width = width;
    this->height = height;

    data = new Cell * [height];
    for (int i = 0; i < height; ++i) {
        data[i] = new Cell[width];
    }
}

Map::~Map() {
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

void Map::initialize() {
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            data[y][x].type = CellType::Empty;

    for (int x = 0; x < width; ++x) {
        data[0][x].type = CellType::Wall;
        data[height - 1][x].type = CellType::Wall;
    }
    for (int y = 0; y < height; ++y) {
        data[y][0].type = CellType::Wall;
        data[y][width - 1].type = CellType::Wall;
    }

    int width1 = width / 3;
    int width2 = 2 * width / 3;

    for (int y = 0; y < height; ++y) {

        data[y][width1].type = CellType::Wall;
        data[y][width2].type = CellType::Wall;
    }

    int midY = height / 2;

    data[midY][width1].type = CellType::Toll;
    data[midY][width2].type = CellType::Toll;
}

char GetSymbolForCell(const Cell& cell, int x, int y, int playerX, int playerY, Direction playerDir) {
    if (x == playerX && y == playerY) {
        switch (playerDir) {
        case Direction::Up: return '^';
        case Direction::Down: return 'v';
        case Direction::Left: return '<';
        case Direction::Right: return '>';
        }
    }
    if (cell.hasCar) return 'C';

    switch (cell.type) {
    case CellType::Wall: return 'X';
    case CellType::Pedestrian: return 'P';
    case CellType::Money: return '$';
    case CellType::Empty: return ' ';
    case CellType::Toll: return 'T';
    }
}


void Map::render(int playerX, int playerY, int viewWidth, int viewHeight, Direction playerDir) {
    system("cls");

    int startX = playerX - viewWidth / 2;
    int startY = playerY - viewHeight / 2;

    for (int y = 0; y < viewHeight; ++y) {
        for (int x = 0; x < viewWidth; ++x) {
            int mapX = startX + x;
            int mapY = startY + y;

            if (mapX >= 0 && mapX < width && mapY >= 0 && mapY < height) {
                std::cout << GetSymbolForCell(data[mapY][mapX], mapX, mapY, playerX, playerY, playerDir);
            }
            else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }
}


Cell** Map::GetData() {
    return data;
}