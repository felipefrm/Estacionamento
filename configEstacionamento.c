#include <stdio.h>
#include <stdlib.h>
#include "configEstacionamento.h"


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
    if (veiculo.tamanho == TRUCK && veiculo.x > 4)
      return 0;
    else if (veiculo.tamanho == CAR && veiculo.x > 5)
      return 0;
    if (mapa[veiculo.y-1][veiculo.x-1] == 0 && mapa[veiculo.y-1][veiculo.x] == 0){
      if (veiculo.tamanho == TRUCK && mapa[veiculo.y-1][veiculo.x+1] == 0)
        return 1;
      else if (veiculo.tamanho == CAR)    // checa se as posições em que estão sendo iseridas
        return 1;                     // os veiculos estão disponíveis
      return 0;
    }
    return 0;
  }
  else if (veiculo.direcao == 'Y'){
    if (veiculo.tamanho == TRUCK && veiculo.y > 4)
      return 0;
    else if (veiculo.tamanho == CAR && veiculo.y > 5)
      return 0;
    if (mapa[veiculo.y-1][veiculo.x-1] == 0 && mapa[veiculo.y][veiculo.x-1] == 0){
      if (veiculo.tamanho == TRUCK && mapa[veiculo.y+1][veiculo.x-1] == 0)
        return 1;
      else if (veiculo.tamanho == CAR)
        return 1;
      return 0;
    }
    return 0;
  }
}


int configInicialMapa(int qtdVeiculos, Auto* veiculo, int **mapa){
  for(int i=0; i<qtdVeiculos; i++){
    if (veiculo[i].direcao == 'X'){
      if (veiculo[i].tamanho != CAR && veiculo[i].tamanho != TRUCK){
        printf("Tamanho de veículo inválido.\n");
        return 0;
    }
      if(!verificaEspacoMapa(veiculo[i], mapa)){
        printf("Configuração física impossível (dois veículos ocupando a mesma "
          "posição inicial ou um veículo estrapolando o limite do estacionamento).\n");
        return 0;
      }
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;
      mapa[veiculo[i].y-1][veiculo[i].x] = 1;
      if (veiculo[i].tamanho == TRUCK)         // configuração inicial do estacionamento
        mapa[veiculo[i].y-1][veiculo[i].x+1] = 1;
    }
    else if (veiculo[i].direcao == 'Y'){
      if (veiculo[i].tamanho != CAR && veiculo[i].tamanho != TRUCK){
        printf("Tamanho de veículo inválido.\n");
        return 0;
    }
      if(!verificaEspacoMapa(veiculo[i], mapa)){
        printf("Configuração física impossível (dois veículos ocupando a mesma "
          "posição inicial ou um veículo estrapolando o limite do estacionamento).\n");
        return 0;
      }
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;
      mapa[veiculo[i].y][veiculo[i].x-1] = 1;
      if (veiculo[i].tamanho == TRUCK)
        mapa[veiculo[i].y+1][veiculo[i].x-1] = 1;
    }
    else if (veiculo[i].id != 0 && veiculo[i].direcao != 'Y' && veiculo[i].direcao != 'X') {
      printf("Entende-se por direção apenas o eixo X e Y.\n");
      return 0;
    }
  }
  return 1;
}
