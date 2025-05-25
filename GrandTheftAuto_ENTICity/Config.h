//Pau Rafel Díaz Hernández & Biel Vilarnau Espínola ENTI 2025
#pragma once

struct IslandConfig {
    int numPedestrians = 0;
    int maxMoney = 0;
    int pedestrianHP = 0;
    int pedestrianAttack = 0;
};

struct ConfigData {
    int mapWidth = 0;
    int mapHeight = 0;

    int playerHP = 0;
    int playerAttack = 0;

    int tollToSanFierro = 0;
    int tollToLasVenturas = 0;

    IslandConfig losSantos;
    IslandConfig sanFierro;
    IslandConfig lasVenturas;
};

ConfigData loadConfig(const std::string& filename);