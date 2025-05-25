//Pau Rafel Díaz Hernández & Biel Vilarnau Espínola ENTI 2025
#pragma once

enum class CellType {
    Empty,
    Wall,
    Player,
    Pedestrian,
    Money
};

struct Cell {
    CellType type = CellType::Empty;
};