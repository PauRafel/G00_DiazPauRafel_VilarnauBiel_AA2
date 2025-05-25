//Pau Rafel Díaz Hernández & Biel Vilarnau Espínola ENTI 2025
#include <fstream>
#include <sstream>
#include <iostream>
#include "Config.h"


ConfigData loadConfig(const std::string& filename) {
    ConfigData config{};
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error al abrir config.txt" << std::endl;
        exit(1);
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        int values[5] = { 0 };
        int i = 0;

        while (std::getline(ss, token, ';') && i < 5) {
            if (!token.empty()) {
                values[i++] = std::stoi(token);
            }
        }

        switch (lineNumber) {
        case 0:
            config.mapWidth = values[0];
            config.mapHeight = values[1];
            break;
        case 1:
            config.playerHP = values[0];
            config.playerAttack = values[1];
            break;
        case 2:
            config.tollToSanFierro = values[0];
            config.tollToLasVenturas = values[1];
            break;
        case 3:
            config.losSantos = { values[0], values[1], values[2], values[3] };
            break;
        case 4:
            config.sanFierro = { values[0], values[1], values[2], values[3] };
            break;
        case 5:
            config.lasVenturas = { values[0], values[1], values[2], values[3] };
            break;
        }

        lineNumber++;
    }

    file.close();
    return config;
}