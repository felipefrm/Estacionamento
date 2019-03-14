#include <stdio.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "configEstacionamento.h"
#include "manobras.h"


int main(int argc, char *argv[]){

  Arquivos* arq = argumentosEntrada(argc, argv);
  int **mapa = criaMapa();
  Auto *veiculo = leituraConfigInicial(arq->veiculos);
  if(!configInicialMapa(qtdVeiculos, veiculo, mapa)){
    printf("Configuração física impossível (dois veículos ocupando a mesma posição inicial).\n");
    return 1;
  }
  leituraExecucaoManobra(veiculo, mapa, arq->manobras);
  free(arq);
  imprimeMapa(mapa);
  return 0;
}
