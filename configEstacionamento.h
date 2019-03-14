#include "entradaSaida.h"
#ifndef ESTACIONAMENTO
#define ESTACIONAMENTO

#define SIZEMAP 6

int** criaMapa();
int configInicialMapa(int qtdVeiculos, Auto* veiculo, int **mapa);
int verificaEspacoMapa(Auto veiculo, int **mapa);

#endif
