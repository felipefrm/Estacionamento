#include "entradaSaida.h"
#ifndef MANOBRAS
#define MANOBRAS

#define CAR_COLLISION 3
#define WALL_COLLISION 2

void apagaPosAnterior(Auto veiculo, int **mapa);
int movimentaVeiculo(Auto *veiculo, int **mapa, Movimento manobra);
int realizaManobra(int qtdVeiculos, Auto *veiculo, Movimento manobra, int **mapa);
int verificaTrajeto(Auto veiculo, int **mapa, Movimento manobra);
int verificaColisaoParede(int i);
int verificaSaidaZ(Auto *veiculo, int qtdVeiculos);
int verificaInicioeFinal(int *inicio,int* final);
int verificaLinhaLivre(Auto veiculo, char direcao, int inicio, int final, int** mapa);
int verificaMatrizLivre(Auto veiculo, int inicio, int final, int traseira, int frente, int** mapa);

#endif
