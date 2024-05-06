//PokeENTI fase 1. Pau Rafel Díaz Hernández & Biel Vilarnau Espínola. AA2

#include <iostream>
#include <fstream>
#include <Windows.h> 
#include <ctime>   

#define VK_UP 0x26
#define VK_DOWN 0x28
#define VK_LEFT 0x25
#define VK_RIGHT 0x27
#define VK_SPACE 0x20
#define VK_ESC 0x1B

int FILAS;
int COLUMNAS;

int pokemonsEnPuebloPaleta;
int pokemonsEnBosque;
int pokemonsEnCuevaCeleste = 3;
int pokemonsEnLigaPokENTI = 3; 

int pokemonsCapturados = 0;


int pokemonsParaDesbloquearCueva = 3;
int pokemonsParaDesbloquearLiga = 3; 

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

int main() {
   
    srand(time(nullptr));

   
    char** mapa = nullptr; 
    if (!cargarConfiguracion(mapa)) {
        return 1;
    }

  
    mapa = new char* [FILAS];
    for (int i = 0; i < FILAS; ++i) {
        mapa[i] = new char[COLUMNAS];
        for (int j = 0; j < COLUMNAS; ++j) {
            mapa[i][j] = '.'; 
        }
    }

   
    colocarPokemons(mapa);

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
                        mapa[posX + i][posY + j] = '.'; 
                       
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

        Sleep(100); 
    }

    for (int i = 0; i < FILAS; ++i) {
        delete[] mapa[i];
    }
    delete[] mapa;

    return 0;
}
