#pragma once
#include <string>

struct ConfigData {
    int mapWidth;
    int mapHeight;

    int pedestriansLosSantos;
    int tollLosSantos;
    int maxMoneyLosSantos;

    int pedestriansSanFierro;
    int tollSanFierro;
    int maxMoneySanFierro;
};

ConfigData loadConfig(const std::string& filename);