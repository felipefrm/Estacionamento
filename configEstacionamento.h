#include "entradaSaida.h"
#ifndef ESTACIONAMENTO
#define ESTACIONAMENTO

#define SIZEMAP 6

int** criaMapa();
void configInicialMapa(int qtdVeiculos, Auto* veiculo, int **mapa);
void imprimeMapa(int **mapa);

#endif
