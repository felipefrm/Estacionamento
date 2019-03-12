#include <stdio.h>
#include <stdlib.h>
#include "configEstacionamento.h"
#include "entradaSaida.h"

int** criaMapa(){
  int **mapa = malloc(sizeof(int*)*SIZEMAP);
  for (int i=0; i<SIZEMAP; i++){
    mapa[i] = malloc(sizeof(int)*SIZEMAP);    // formação do estacionamento
    for (int j=0; j<SIZEMAP; j++)             // atraves de uma matriz
      mapa[i][j] = 0;
    }
  return mapa;
}

void configInicialMapa(int qtdVeiculos, Auto* veiculo, int **mapa){
  for(int i=0; i<qtdVeiculos; i++){

    if (veiculo[i].direcao == 'X'){
      printf("%d %d\n",veiculo[i].x,veiculo[i].y);
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;
      mapa[veiculo[i].y-1][veiculo[i].x] = 1;
      if (veiculo[i].tamanho == '3')
        mapa[veiculo[i].y-1][veiculo[i].x+1] = 1;
    }
    else if (veiculo[i].direcao == 'Y'){
      printf("%d %d\n",veiculo[i].x,veiculo[i].y);
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;  // configuração inicial do mapa
      mapa[veiculo[i].y][veiculo[i].x-1] = 1;   // ja com os veiculos
      if (veiculo[i].tamanho == '3')
        mapa[veiculo[i].y+1][veiculo[i].x-1] = 1;
    }
  }
}
