//Pau Rafel Díaz Hernández & Biel Vilarnau Espínola ENTI 2025
#pragma once

enum class PedestrianType {
    Neutral,
    Aggressive
};

struct Pedestrian {
    int x;
    int y;
    int hp;                 
    int attack;             
    bool isAlive;
    PedestrianType type;    
};