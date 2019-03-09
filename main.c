#include <stdio.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "configEstacionamento.h"
#include "manobras.h"

int main(void){
                                  // formação do estacionamento
  int **mapa = criaMapa();        // atraves de uma matriz
  Auto *veiculo = leituraConfigInicial();
  configInicialMapa(qtdVeiculos, veiculo, mapa);
  leituraExecucaoManobra(veiculo, mapa);
  imprimeMapa(mapa);
}
