#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <sys/time.h>
#include "entradaSaida.h"
#include "manobras.h"
#include "configEstacionamento.h"

Arquivos* argumentosEntrada(int argc, char* argv[]){

  Arquivos* arq = malloc(sizeof(Arquivos));
  int opcao;
  if (argc < 5){
    printf("Use: ./tp -m [ARQUIVO DE MANOBRA] -v [ARQUIVO DE CONFIGURAÇÃO INICIAL]\n");
    return 0;
  }

  while((opcao = getopt(argc, argv, "m:v:"))!= -1)
  {
    switch(opcao)
    {
      case 'm':
        if(!(arq->manobras = fopen(optarg, "r"))) {
          printf("Erro na abertura do arquivo.\n");
          return 0;
        }
        break;
      case 'v':
        if(!(arq->veiculos = fopen(optarg, "r"))) {
          printf("Erro na abertura do arquivo.\n");
          return 0;
        }
        break;
    }
  }
  return arq;
}

int calculaQtdVeiculos(FILE* arq){
  qtdVeiculos = 0;
  while (!feof(arq))
    if (fgetc(arq) == '\n')    // calcula a quantidade de veiculos
      qtdVeiculos++;              // por meio da quantidade de '\n'
  return qtdVeiculos;
}

int convertCharToInt(char num){     // conversão de char para inteiro
  return (num -48);
}

void setVeiculo(int qtdVeiculos, FILE *arq, Auto *veiculo){
  printf("QTD: %d\n", qtdVeiculos);
  char c;
  for (int i=0; i<qtdVeiculos; i++){
    for(int j=0; (c = fgetc(arq)) != '\n' && !feof(arq); j++){
      if (j==0)
        veiculo[i].id = c;
      else if (j==2)
        veiculo[i].tamanho = c;     // atribuições das caracteristicas de cada
      else if (j==4)                // automovel na struct
        veiculo[i].direcao = c;
      else if (j==7)
        veiculo[i].x = convertCharToInt(c);
      else if (j==9)
        veiculo[i].y = convertCharToInt(c);
    }
    fscanf(arq, "\n");
  }
}

Auto* leituraConfigInicial(FILE* arq_veiculos){
  int qtdVeiculos = calculaQtdVeiculos(arq_veiculos);
  Auto *veiculo = malloc(sizeof(Auto)*qtdVeiculos);  // aloca memoria para cada veiculo
  rewind(arq_veiculos);    // seta o ponteiro do arquivo para o inicio do mesmo
  setVeiculo(qtdVeiculos, arq_veiculos, veiculo);
  fclose(arq_veiculos);
  return veiculo;
}

int verificaSinal(char sinal, int num){
  if (sinal == '-')
    return -num;
  else
    return num;
}

Movimento leituraManobra(FILE *arq_manobra){

  Movimento manobra; char c;
  manobra.sinal = '+';                                //das manobras linha por linha
  for(int i=0; (c = fgetc(arq_manobra)) != '\n' && !feof(arq_manobra); i++){  //armazena as configurações
    if (i == 0)
      manobra.id = c;
    else if (i == 2)
      manobra.direcao = c;
    else if (i == 4 && c == '-')
      manobra.sinal = c;
    else if ((i == 4 && c != '+' && c != '-') || i == 5)
      manobra.amplitude = verificaSinal(manobra.sinal, convertCharToInt(c));
  }
  printf("ID: %c\n", manobra.id);
  printf("DIRECAO: %c\n", manobra.direcao);
  printf("SINAL: %c\n", manobra.sinal);
  printf("AMPLITUDE: %d\n", manobra.amplitude);
  fscanf(arq_manobra, "\n");
  return manobra;
}

int leituraExecucaoManobra(Auto *veiculo, int **mapa, FILE* arq_manobras){
  double utime_ant, utime_pos, stime_ant, stime_pos;
  struct timeval clocktime_ant, clocktime_pos;
  clocktime_ant = contaTempoRelogio();
  contaTempoProcessador(&utime_ant, &stime_ant); //marca tempo inicial
  printf("%f\n", clocktime_ant);
  while(!feof(arq_manobras))
    if(!realizaManobra(qtdVeiculos, veiculo, leituraManobra(arq_manobras), mapa)){
      clocktime_pos = contaTempoRelogio();
      contaTempoProcessador(&utime_pos, &stime_pos); //marca tempo final
      printf("Tempo de usuário: %fs + Tempo de sistema: %fs = %f.\n", utime_pos-utime_ant, stime_pos-stime_ant, (utime_pos-utime_ant) + (stime_pos-stime_ant));
      printf("Tempo de relogio: %fs\n", ((double)(clocktime_pos.tv_sec-clocktime_ant.tv_sec))
       + ((double)(clocktime_pos.tv_usec-clocktime_ant.tv_usec)/1000000));
      printf("Manobra impossível.\n");
      fclose(arq_manobras);
      return 0;
    }
    clocktime_pos = contaTempoRelogio();
  contaTempoProcessador(&utime_pos, &stime_pos); //marca tempo final
  printf("Tempo de usuário: %fs + Tempo de sistema: %fs = %f.\n", utime_pos-utime_ant, stime_pos-stime_ant, (utime_pos-utime_ant) + (stime_pos-stime_ant));
  printf("Tempo de relogio: %fs\n", ((double)(clocktime_pos.tv_sec-clocktime_ant.tv_sec))
   + ((double)(clocktime_pos.tv_usec-clocktime_ant.tv_usec)/1000000));
  fclose(arq_manobras);
  free(veiculo);
}

void imprimeMapa(int **mapa){
  for (int i=SIZEMAP-1; i>=0; i--){
    for (int j=0; j<SIZEMAP; j++){
      printf("%d ", mapa[i][j]);
    }
    free(mapa[i]);
    printf("\n");
  }
  free(mapa);
}
