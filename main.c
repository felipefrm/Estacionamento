#include <stdio.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "configEstacionamento.h"
#include "manobras.h"


int main(int argc, char *argv[]){

  double utime_ant, utime_pos, stime_ant, stime_pos;
  struct timeval runtime_ant, runtime_pos;
  contaTempo(&utime_ant, &stime_ant, &runtime_ant);
  Arquivos* arq = argumentosEntrada(argc, argv);
  int **mapa = criaMapa();
  Auto *veiculo = leituraConfigInicial(arq->veiculos);
  if(!(configInicialMapa(qtdVeiculos, veiculo, mapa) && verificaArqVazio(arq->manobras, veiculo))){
    liberaPonteiros(veiculo, arq, mapa);
    return 1;
  }
  leituraExecucaoManobra(veiculo, mapa, arq->manobras);
  free(arq);
  contaTempo(&utime_pos, &stime_pos, &runtime_pos);
  imprimeTempo(utime_pos-utime_ant, stime_pos-stime_ant, ((double)(runtime_pos.tv_sec
   -runtime_ant.tv_sec)) + ((double)(runtime_pos.tv_usec-runtime_ant.tv_usec)/1000000));
  imprimeMapa(mapa);
  return 0;
}
