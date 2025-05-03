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

// Función para cargar la configuración
ConfigData loadConfig(const std::string& filename);
