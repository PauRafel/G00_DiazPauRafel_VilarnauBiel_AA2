//Pau Rafel Díaz Hernández & Biel Vilarnau Espínola ENTI 2025
#include <fstream>
#include <sstream>
#include <iostream>
#include "Config.h"


ConfigData loadConfig(const std::string& filename) {
    ConfigData config{};
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de configuración: " << filename << std::endl;
        exit(1); 
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        int values[3] = { 0 };

        int i = 0;
        while (std::getline(ss, token, ';')) {
            if (!token.empty()) {
                values[i++] = std::stoi(token);
            }
        }

        if (lineNumber == 0) {
            config.mapWidth = values[0];
            config.mapHeight = values[1];
        }
        else if (lineNumber == 1) {
            config.pedestriansLosSantos = values[0];
            config.tollLosSantos = values[1];
            config.maxMoneyLosSantos = values[2];
        }
        else if (lineNumber == 2) {
            config.pedestriansSanFierro = values[0];
            config.tollSanFierro = values[1];
            config.maxMoneySanFierro = values[2];
        }

        lineNumber++;
    }

    file.close();
    return config;
}