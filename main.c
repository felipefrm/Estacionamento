#include <stdio.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "configEstacionamento.h"
#include "manobras.h"


int main(int argc, char *argv[]){

  Arquivos* arq = argumentosEntrada(argc, argv);
  int **mapa = criaMapa();
  Auto *veiculo = leituraConfigInicial(arq->veiculos);
  configInicialMapa(qtdVeiculos, veiculo, mapa);
  if(leituraExecucaoManobra(veiculo, mapa, arq->manobras))
    verificaSaidaZ(veiculo, qtdVeiculos);
  free(arq);
  imprimeMapa(mapa);
}
