#include <Windows.h>
#include "teclas.h"
#include "funciones.h"
#include "configuracion.h"

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

int pokebolasEnInventario = 5; 

int pokemonsEnMapa;
