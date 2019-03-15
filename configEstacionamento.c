#include <stdio.h>
#include <stdlib.h>
#include "configEstacionamento.h"
#include "entradaSaida.h"

int** criaMapa(){
  int **mapa = malloc(sizeof(int*)*SIZEMAP);
  for (int i=0; i<SIZEMAP; i++){
    mapa[i] = malloc(sizeof(int)*SIZEMAP);    // formação do estacionamento
    for (int j=0; j<SIZEMAP; j++)             // atraves de uma matriz 6x6
      mapa[i][j] = 0;
    }
  return mapa;
}

int verificaEspacoMapa(Auto veiculo, int **mapa){
  if (veiculo.direcao == 'X'){
    if (mapa[veiculo.y-1][veiculo.x-1] == 0 && mapa[veiculo.y-1][veiculo.x] == 0){
      if (veiculo.tamanho == TRUCK && mapa[veiculo.y-1][veiculo.x+1] == 0)
        return 1;
      if (veiculo.tamanho == CAR){    // checa se as posições em que estão sendo iseridas
        return 1;                     // os veiculos estão disponíveis
      }
      return 0;
    }
    return 0;
  }
  else if (veiculo.direcao == 'Y'){
    if (mapa[veiculo.y-1][veiculo.x-1] == 0 && mapa[veiculo.y][veiculo.x-1] == 0){
      if (veiculo.tamanho == TRUCK && mapa[veiculo.y+1][veiculo.x-1] == 0)
        return 1;
      if (veiculo.tamanho == CAR)
        return 1;
      return 0;
    }
    return 0;
  }
}

int configInicialMapa(int qtdVeiculos, Auto* veiculo, int **mapa){
  for(int i=0; i<qtdVeiculos; i++){

    if (veiculo[i].direcao == 'X'){
      if(!verificaEspacoMapa(veiculo[i], mapa)){
        printf("Configuração física impossível (dois veículos ocupando a mesma posição inicial).\n");
        return 0;
      }
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;          // configuração inicial do estacionamento
      mapa[veiculo[i].y-1][veiculo[i].x] = 1;
      if (veiculo[i].tamanho == TRUCK)
        mapa[veiculo[i].y-1][veiculo[i].x+1] = 1;
    }
    else if (veiculo[i].direcao == 'Y'){

      if(!verificaEspacoMapa(veiculo[i], mapa)){
        printf("Configuração física impossível (dois veículos ocupando a mesma posição inicial).\n");
        return 0;
      }
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;  
      mapa[veiculo[i].y][veiculo[i].x-1] = 1;
      if (veiculo[i].tamanho == TRUCK)
        mapa[veiculo[i].y+1][veiculo[i].x-1] = 1;
    }
  }
  return 1;
}
