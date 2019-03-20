#include <stdio.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "configEstacionamento.h"
#include "manobras.h"


int main(int argc, char *argv[]){

  double utime_ant, utime_pos, stime_ant, stime_pos;
  struct timeval runtime_ant, runtime_pos;
  contaTempoProcessador(&utime_ant, &stime_ant);
  Arquivos* arq = argumentosEntrada(argc, argv);
  if (!arq->flag){      // se a flag for 0, é porque houve erro na abertura dos arquivo
    free(arq);          // ou então o ./tp foi executado com falta de argumentos
    return 1;
  }
  int **mapa = criaMapa();
  Auto *veiculo = leituraConfigInicial(arq->veiculos);
  if(!(configInicialMapa(qtdVeiculos, veiculo, mapa) && verificaArqVazio(arq->manobras, veiculo))){
    liberaPonteiros(veiculo, arq, mapa);
    return 1;
  }
  leituraExecucaoManobra(veiculo, mapa, arq->manobras);
  free(arq);
  contaTempoProcessador(&utime_pos, &stime_pos);
  imprimeTempo(utime_pos-utime_ant, stime_pos-stime_ant);
  imprimeMapa(mapa);
  return 0;
}
