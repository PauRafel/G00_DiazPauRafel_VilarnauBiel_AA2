//PokeENTI Pau Rafel Díaz Hernández & Biel Vilarnau Espínola. AA2

#include <iostream>
#include <Windows.h>
#include <ctime>
#include "funciones.h"
#include "teclas.h"
#include "configuracion.h"

int main() {

    srand(time(nullptr));

    mostrarPantallaInicial();
    mostrarMenuPrincipal();

    char** mapa = nullptr;
    if (!cargarConfiguracion(mapa)) {
        return 1;
    }


    mapa = new char* [FILAS];
    for (int i = 0; i < FILAS; ++i) {
        mapa[i] = new char[COLUMNAS];
        for (int j = 0; j < COLUMNAS; ++j) {
            mapa[i][j] = ' ';
        }
    }

    colocarPokemons(mapa);
    colocarPokeball(mapa);

    int posX = FILAS / 2;
    int posY = COLUMNAS / 2;
    mapa[posX][posY] = '^';

    while (true) {
        imprimirMapa(mapa, posX, posY, pokemonsCapturados);

        if (GetAsyncKeyState(VK_UP) & 0x8000) {

            if (posX > 0 && mapa[posX - 1][posY] != 'X') {
                mapa[posX][posY] = ' ';
                --posX;
                mapa[posX][posY] = '^';
            }
        }
        else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {

            if (posX < FILAS - 1 && mapa[posX + 1][posY] != 'X') {
                mapa[posX][posY] = ' ';
                ++posX;
                mapa[posX][posY] = 'v';
            }
        }
        else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {

            if (posY > 0 && mapa[posX][posY - 1] != 'X') {
                mapa[posX][posY] = ' ';
                --posY;
                mapa[posX][posY] = '<';
            }
        }
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {

            if (posY < COLUMNAS - 1 && mapa[posX][posY + 1] != 'X') {
                mapa[posX][posY] = ' ';
                ++posY;
                mapa[posX][posY] = '>';
            }
        }
        else if (GetAsyncKeyState(VK_SPACE) & 0x8000) {

            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (mapa[posX + i][posY + j] == 'P') {
                        ++pokemonsCapturados;
                        mapa[posX + i][posY + j] = ' ';

                        if (pokemonsCapturados >= pokemonsParaDesbloquearCueva) {
                            std::cout << "¡Has desbloqueado la Cueva Celeste!" << std::endl;

                        }
                        if (pokemonsCapturados >= pokemonsParaDesbloquearLiga) {
                            std::cout << "¡Has desbloqueado la Liga PokENTI!" << std::endl;

                        }
                        colocarPokemons(mapa);
                    }
                }
            }
        }
        else if (GetAsyncKeyState(VK_ESC) & 0x8000) {
            break;
        }

        if (mapa[posX][posY] == 'O') {
            ++pokebolasEnInventario; // Incrementar el contador de Pokeballs
            colocarPokeball(mapa); // Colocar una nueva Pokeball en el mapa
        }

        if (mapa[posX - 1][posY] == 'P' || mapa[posX + 1][posY] == 'P' ||
            mapa[posX][posY - 1] == 'P' || mapa[posX][posY + 1] == 'P') {

            // Esperar a que el jugador seleccione una opción
            while (true) {
                int opcion;
                std::cin >> opcion;

                switch (opcion) {
                case 1: // Capturar Pokémon
                    capturarPokemon(mapa, posX, posY, pokemonsCapturados);
                    break;
                case 2:
                    break;
                case 3: // Huir
                    std::cout << "Has huido del combate." << std::endl;
                    break;
                default:
                    std::cout << "Opcion invalida. Por favor, selecciona una opción valida." << std::endl;
                }

                if (opcion == 1 || opcion == 2 || opcion == 3) {
                    // Salir del bucle si el jugador selecciona una opción válida
                    break;
                }
            }
        }

        Sleep(100);
    }

    for (int i = 0; i < FILAS; ++i) {
        delete[] mapa[i];
    }
    delete[] mapa;

    return 0;
}