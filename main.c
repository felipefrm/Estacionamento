#include <stdio.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "configEstacionamento.h"
#include "manobras.h"


void main(int argc, char *argv[]){

  double utime_ant, utime_pos, stime_ant, stime_pos;
  struct timeval runtime_ant, runtime_pos;
  contaTempoProcessador(&utime_ant, &stime_ant);
  Arquivos* arq = argumentosEntrada(argc, argv);
  if (arq->flag){             // se a flag for 0, é porque houve erro na abertura dos arquivos
    int **mapa = criaMapa();  // ou então o ./tp foi executado com falta de argumentos
    Auto *veiculo = leituraConfigInicial(arq->veiculos);
    if(configInicialMapa(qtdVeiculos, veiculo, mapa) && verificaArqVazio(arq->manobras)){
      leituraExecucaoManobra(veiculo, mapa, arq->manobras);
      contaTempoProcessador(&utime_pos, &stime_pos);
      imprimeTempo(utime_pos-utime_ant, stime_pos-stime_ant);
      imprimeMapa(mapa);
    }
    liberaPonteiros(veiculo, arq, mapa);
  }
  else{
    free(arq);
  }
}
