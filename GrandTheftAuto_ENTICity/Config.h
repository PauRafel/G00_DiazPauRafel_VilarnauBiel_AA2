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

// Funci�n para cargar la configuraci�n
ConfigData loadConfig(const std::string& filename);
