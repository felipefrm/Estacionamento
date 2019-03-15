#include <stdio.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "configEstacionamento.h"
#include "manobras.h"

void contaTempo(double *usertime, double *systemtime, struct timeval *timeofday){
  contaTempoRelogio(&(*timeofday));
  contaTempoProcessador(&(*usertime), &(*systemtime));
}

int main(int argc, char *argv[]){

  double utime_ant, utime_pos, stime_ant, stime_pos;
  struct timeval clocktime_ant, clocktime_pos;
  contaTempo(&utime_ant, &stime_ant, &clocktime_ant);
  // clocktime_ant = contaTempoRelogio(); //marca tempo inicial
  // contaTempoProcessador(&utime_ant, &stime_ant); //marca tempo inicial
  Arquivos* arq = argumentosEntrada(argc, argv);
  int **mapa = criaMapa();
  Auto *veiculo = leituraConfigInicial(arq->veiculos);
  if(!configInicialMapa(qtdVeiculos, veiculo, mapa))
    return 1;
  leituraExecucaoManobra(veiculo, mapa, arq->manobras);
  free(arq);
  contaTempo(&utime_pos, &stime_pos, &clocktime_pos);
  imprimeTempo(utime_pos-utime_ant, stime_pos-stime_ant, (utime_pos-utime_ant)
  + (stime_pos-stime_ant), ((double)(clocktime_pos.tv_sec-clocktime_ant.tv_sec))
  + ((double)(clocktime_pos.tv_usec-clocktime_ant.tv_usec)/1000000));
  imprimeMapa(mapa);

  // clocktime_pos = contaTempoRelogio(); //marca tempo final
  // contaTempoProcessador(&utime_pos, &stime_pos); //marca tempo final

  return 0;
}
