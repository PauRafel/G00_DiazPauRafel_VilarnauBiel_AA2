#pragma once

void imprimirMapa(char** mapa, int posX, int posY, int pokemonsCapturados);
bool cargarConfiguracion(char** mapa);
int numeroAleatorio(int min, int max);
void colocarPokemons(char** mapa);
void capturarPokemon(char** mapa, int& posX, int& posY, int& pokemonsCapturados);
int obtenerSaludPokemon(char pokemon);
void atacarPokemon(char** mapa, int posX, int posY);

void colocarPokeball(char** mapa);
void mostrarMenuPrincipal();
void mostrarPantallaInicial();