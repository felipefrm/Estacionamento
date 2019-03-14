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
    if (fgetc(arq) == '\n' || feof(arq))    // calcula a quantidade de veiculos
      qtdVeiculos++;              // por meio da quantidade de '\n'
  // printf("HOW MANY CARS WE HAVE? %d\n", qtdVeiculos);
  return qtdVeiculos;
}

int convertCharToInt(char num){     // conversão de char para inteiro
  return (num -48);
}

void setVeiculo(int qtdVeiculos, FILE *arq, Auto *veiculo){
  // printf("QTD: %d\n", qtdVeiculos);
  char c;
  for (int i=0; i<qtdVeiculos; i++){
    for(int j=0; (c = fgetc(arq)) != '\n' && !feof(arq); j++){
      if (j==0){

        veiculo[i].id = c;
        // printf("ID:%c\n",veiculo[i].id);
      }
      else if (j==2)
        veiculo[i].tamanho = convertCharToInt(c);     // atribuições das caracteristicas de cada
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
  // printf("ID: %c\n", manobra.id);
  // printf("DIRECAO: %c\n", manobra.direcao);
  // printf("SINAL: %c\n", manobra.sinal);
  // printf("AMPLITUDE: %d\n", manobra.amplitude);
  fscanf(arq_manobra, "\n");
  return manobra;
}

void imprimeTempo(double user_time, double system_time, double process_time, double clocktime){
  printf(" ~-~-~-~-~-~-~-~-~- ESTATÍSTICAS DE TEMPO DE EXECUÇÃO -~-~-~-~-~-~-~\n");
  printf("%fs (tempo de usuário) + %fs (tempo de sistema) = %fs (tempo total)\n", user_time, system_time, process_time);
  printf("%fs (tempo)\n", clocktime);
  printf("-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
}

int leituraExecucaoManobra(Auto *veiculo, int **mapa, FILE* arq_manobras){
  double utime_ant, utime_pos, stime_ant, stime_pos;
  struct timeval clocktime_ant, clocktime_pos;
  int flag;
  clocktime_ant = contaTempoRelogio();
  contaTempoProcessador(&utime_ant, &stime_ant); //marca tempo inicial
  while(!feof(arq_manobras))
    flag = realizaManobra(qtdVeiculos, veiculo, leituraManobra(arq_manobras), mapa);
  clocktime_pos = contaTempoRelogio();
  contaTempoProcessador(&utime_pos, &stime_pos); //marca tempo final
  imprimeTempo(utime_pos-utime_ant, stime_pos-stime_ant, (utime_pos-utime_ant)
  + (stime_pos-stime_ant), ((double)(clocktime_pos.tv_sec-clocktime_ant.tv_sec))
  + ((double)(clocktime_pos.tv_usec-clocktime_ant.tv_usec)/1000000));
  if (flag != 1){
    if (flag == 0)
      printf("INCOMPATIBILIDADE DE DADOS... Não há veículo identificado como %c.\n");
    else if (flag == 2)
      printf("Conjunto de manobras inviável!\nMOTIVO: Colisão com o muro.\n");
    else if (flag == 3)
      printf("Conjunto de manobras inviável!\nMOTIVO: Colisão com um veículo.\n");
    fclose(arq_manobras);
    free(veiculo);
    return 0;

  }
  else {
    if(!verificaSaidaZ(veiculo, qtdVeiculos)){
      printf("O veículo Z não chegou até à saída.\n");
      return 0;
    }
    printf("O veículo Z chegou até à saída.\n");
    fclose(arq_manobras);
    free(veiculo);
    return 1;
  }
}

void imprimeMapa(int **mapa){
  printf("-~-~-~-~-~-~-~-~-~-~-~\n");
  printf("MAPA DO ESTACIONAMENTO\n");
  for (int i=SIZEMAP-1; i>=0; i--){
    for (int j=0; j<SIZEMAP; j++){
      printf("%d ", mapa[i][j]);
    }
    free(mapa[i]);
    printf("\n");
  }
  free(mapa);
  printf("-~-~-~-~-~-~-~-~-~-~-~\n");
}
