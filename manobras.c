#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "manobras.h"
#include "entradaSaida.h"

void contaTempoProcessador(double *utime, double *stime){
  struct rusage resources;
  getrusage(RUSAGE_SELF, &resources);
  *utime = (double) resources.ru_utime.tv_sec + 1.e-6 * (double) resources.ru_utime.tv_usec;
  *stime = (double) resources.ru_stime.tv_sec + 1.e-6 * (double) resources.ru_stime.tv_usec;
}

struct timeval contaTempoRelogio(){
  struct timeval tempo;
  gettimeofday(&tempo, NULL);
  return tempo;
  // return (double)(tempo.tv_sec+(tempo.tv_usec/1000000));
}

void apagaPosAnterior(Auto veiculo, int **mapa){
  if (veiculo.direcao == 'X'){
    printf("%d %d\n",veiculo.x,veiculo.y);
    mapa[veiculo.y-1][veiculo.x-1] = 0;
    mapa[veiculo.y-1][veiculo.x] = 0;
    if (veiculo.tamanho == '3')
      mapa[veiculo.y-1][veiculo.x+1] = 0;
  }
  else if (veiculo.direcao == 'Y'){
    printf("%d %d\n",veiculo.x,veiculo.y);
    mapa[veiculo.y-1][veiculo.x-1] = 0;
    mapa[veiculo.y][veiculo.x-1] = 0;
    if (veiculo.tamanho == '3')
      mapa[veiculo.y+1][veiculo.x-1] = 0;
  }
}

int movimentaVeiculo(Auto *veiculo, int **mapa, Movimento manobra, int id){
  if (manobra.direcao == 'X')
    veiculo[id].x +=  manobra.amplitude;
  else if (manobra.direcao == 'Y')
    veiculo[id].y += manobra.amplitude;

  if (veiculo[id].direcao == 'Y'){
    for (int i = veiculo[id].y-1; i < veiculo[id].y-1+convertCharToInt(veiculo[id].tamanho);i++){
      if (mapa[i][veiculo[id].x-1]!= 0){
        printf("Caralho teve uma batida alí, eita porra!\n");        //verifica se a posição do veiculo
        return 0;                                     //depois da manobra está livre
      }
      else                                          //se estiver, coloca a parte do veiculo no lugar
        mapa[i][veiculo[id].x-1] = 1;                 //se não, retorna 0
    }
  }
  else if (veiculo[id].direcao == 'X'){
    for (int i = veiculo[id].x-1; i < veiculo[id].x-1+convertCharToInt(veiculo[id].tamanho);i++){
      if (mapa[veiculo[id].y-1][i]!= 0){
        printf("Caralho teve uma batida alí, eita porra!\n");
        return 0;
    }
      else if (mapa[veiculo[id].y-1][i] == 0)
        mapa[veiculo[id].y-1][i] = 1;
    }
  }
  return 1;
}

int realizaManobra(int qtdVeiculos, Auto *veiculo, Movimento manobra, int **mapa){
  printf("\n");
  char flag = 0;
  for (int i=0; i<qtdVeiculos; i++){
    printf("%c e %c\n", veiculo[i].id, manobra.id);
    if (veiculo[i].id == manobra.id){
      flag = 1;
      apagaPosAnterior(veiculo[i], mapa);
      if(!movimentaVeiculo(veiculo, mapa, manobra, i))
        return 0;
    }
  }
  if (!flag){
    printf("INCOMPATIBILIDADE DE DADOS... Não há veículo identificado como %c.\n", manobra.id);
    return 0;
  }
}
