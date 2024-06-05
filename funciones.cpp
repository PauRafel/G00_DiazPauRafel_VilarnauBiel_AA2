#include <iostream>
#include <fstream>
#include <Windows.h>
#include <ctime>
#include "funciones.h"
#include "configuracion.h"
#include <cstdlib>

void imprimirMapa(char** mapa, int posX, int posY, int pokemonsCapturados) {
    system("cls");

    int inicioFila = posX - 5;
    int finFila = posX + 5;
    int inicioColumna = posY - 5;
    int finColumna = posY + 5;

    if (inicioFila < 0) inicioFila = 0;
    if (finFila >= FILAS) finFila = FILAS - 1;
    if (inicioColumna < 0) inicioColumna = 0;
    if (finColumna >= COLUMNAS) finColumna = COLUMNAS - 1;

    for (int i = inicioFila; i <= finFila; ++i) {
        for (int j = inicioColumna; j <= finColumna; ++j) {
            std::cout << mapa[i][j] << " ";
        }
        std::cout << std::endl;
    }

    if (mapa[posX - 1][posY] == 'P' || mapa[posX + 1][posY] == 'P' ||
        mapa[posX][posY - 1] == 'P' || mapa[posX][posY + 1] == 'P') {
        std::cout << "¡Un Pokemon salvaje aparecio cerca de ti!" << std::endl;
        std::cout << "Opciones de interaccion:" << std::endl;
        std::cout << "1. Capturar Pokemon" << std::endl;
        std::cout << "2. Atacar Pokemon" << std::endl;
        std::cout << "3. Huir" << std::endl;
    }

    std::cout << "Pokemons capturados: " << pokemonsCapturados << std::endl;
}

bool cargarConfiguracion(char** mapa) {

    std::ifstream archivoConfig("config.txt");
    if (!archivoConfig.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de configuracion." << std::endl;
        return false;
    }


    char delim;
    archivoConfig >> FILAS >> delim >> COLUMNAS;
    if (delim != ';') {
        std::cerr << "Error: Formato incorrecto en la primera linea del archivo de configuracion." << std::endl;
        return false;
    }


    mapa = new char* [FILAS];
    for (int i = 0; i < FILAS; ++i) {
        mapa[i] = new char[COLUMNAS];
    }


    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            mapa[i][j] = 'X';
        }
    }


    archivoConfig >> pokemonsEnPuebloPaleta >> delim >> pokemonsEnBosque;


    archivoConfig.close();

    return true;
}

int numeroAleatorio(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void colocarPokemons(char** mapa) {

    std::cout << "Colocando Pokemons en Pueblo Paleta..." << std::endl;
    for (int i = 0; i < pokemonsEnPuebloPaleta; ++i) {
        int posX = numeroAleatorio(1, FILAS - 2);
        int posY = numeroAleatorio(1, COLUMNAS - 2);
        std::cout << "Pokemon en posicion (" << posX << ", " << posY << ")" << std::endl;
        if (mapa[posX][posY] != 'X') {
            mapa[posX][posY] = 'P';
        }
        else {
            std::cout << "Error: Pokemon en posicion invalida" << std::endl;
        }
    }


    std::cout << "Colocando Pokemons en el Bosque..." << std::endl;
    for (int i = 0; i < pokemonsEnBosque; ++i) {
        int posX = numeroAleatorio(FILAS / 2 + 1, FILAS - 2);
        int posY = numeroAleatorio(1, COLUMNAS - 2);
        std::cout << "Pokemon en posicion (" << posX << ", " << posY << ")" << std::endl;
        if (mapa[posX][posY] != 'X') {
            mapa[posX][posY] = 'P';
        }
        else {
            std::cout << "Error: Pokemon en posicion invalida" << std::endl;
        }
    }


    std::cout << "Colocando Pokemons en Cueva Celeste..." << std::endl;
    for (int i = 0; i < pokemonsEnCuevaCeleste; ++i) {
        int posX = numeroAleatorio(1, FILAS - 2);
        int posY = numeroAleatorio(COLUMNAS / 2 + 1, COLUMNAS - 2);
        std::cout << "Pokemon en posicion (" << posX << ", " << posY << ")" << std::endl;
        if (mapa[posX][posY] != 'X') {
            mapa[posX][posY] = 'P';
        }
        else {
            std::cout << "Error: Pokemon en posicion invalida" << std::endl;
        }
    }


    std::cout << "Colocando Pokemons en Liga PokENTI..." << std::endl;
    for (int i = 0; i < pokemonsEnLigaPokENTI; ++i) {
        int posX = numeroAleatorio(FILAS / 2 + 1, FILAS - 2);
        int posY = numeroAleatorio(COLUMNAS / 2 + 1, COLUMNAS - 2);
        std::cout << "Pokemon en posicion (" << posX << ", " << posY << ")" << std::endl;
        if (mapa[posX][posY] != 'X') {
            mapa[posX][posY] = 'P';
        }
        else {
            std::cout << "Error: Pokemon en posicion invalida" << std::endl;
        }
    }
}

void capturarPokemon(char** mapa, int& posX, int& posY, int& pokemonsCapturados) {
    // Verificar si hay una Pokébola en el inventario
    if (pokebolasEnInventario > 0) {
        // Reducir el número de Pokébolas en el inventario
        --pokebolasEnInventario;

        // Calcular la probabilidad de captura
        int saludPokemon = obtenerSaludPokemon(mapa[posX][posY]); // Debes implementar esta función
        int probabilidadCaptura = 10 + (pokemonsCapturados * 5) - (saludPokemon * 2); // Ejemplo de cálculo

        // Generar un número aleatorio entre 1 y 100
        int randNum = numeroAleatorio(1, 100);

        // Comprobar si el Pokémon es capturado
        if (randNum <= probabilidadCaptura) {
            std::cout << "¡Has capturado al Pokémon!" << std::endl;
            --pokemonsEnMapa; // Disminuir la cantidad de Pokémons en el mapa
            ++pokemonsCapturados; // Incrementar el contador de Pokémons capturados
            mapa[posX][posY] = ' '; // Eliminar el Pokémon del mapa
        }
        else {
            std::cout << "¡El Pokémon ha escapado!" << std::endl;
        }
    }
    else {
        std::cout << "No tienes Pokébolas para capturar Pokémon." << std::endl;
    }
}

int obtenerSaludPokemon(char pokemon) {
    // Supongamos que asignamos una salud aleatoria entre 50 y 100
    return rand() % 51 + 50;
}

void mostrarPantallaInicial() {
    system("cls");
    std::cout << "*********************************" << std::endl;
    std::cout << "*       ¡Bienvenido a           *" << std::endl;
    std::cout << "*          PokeEnti!            *" << std::endl;
    std::cout << "*********************************" << std::endl;

    Sleep(3000); // Pausa durante 3 segundos
}

void mostrarMenuPrincipal() {
    int opcion = 0;

    while (true) {
        system("cls");
        std::cout << "Bienvenido al juego de PokeEnti" << std::endl;
        std::cout << "1. Play" << std::endl;
        std::cout << "2. Exit" << std::endl;
        std::cout << "Selecciona una opcion: ";
        std::cin >> opcion;

        if (opcion == 1 || opcion == 2) {
            break;
        }
        else {
            std::cout << "Opción inválida. Por favor, selecciona una opción válida." << std::endl;
            Sleep(2000); // Pausa para que el usuario pueda leer el mensaje
        }
    }

    if (opcion == 2) {
        exit(0);
    }
}

void colocarPokeball(char** mapa) {
    while (true) {
        int posX = numeroAleatorio(1, FILAS - 2);
        int posY = numeroAleatorio(1, COLUMNAS - 2);
        if (mapa[posX][posY] == ' ') { // Asegurar que la posición está vacía
            mapa[posX][posY] = 'O'; // Colocar una Pokeball
            break;
        }
    }
}
