#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <sys/time.h>
#include "entradaSaida.h"
#include "manobras.h"
#include "configEstacionamento.h"

Arquivos* argumentosEntrada(int argc, char* argv[]){

  Arquivos* arq = malloc(sizeof(Arquivos));   // armazena os ponteiros dos arquivos
  int opcao;                                  // passados por linha de comando em uma struct
  if (argc < 5){
    printf("Use: ./tp -m [ARQUIVO DE MANOBRA] -v [ARQUIVO DE CONFIGURAÇÃO INICIAL]\n");
    return 0;
  }

  while((opcao = getopt(argc, argv, "m:v:"))!= -1)    // recebe-se os argumentos por meio
  {                                                   // da funcao getopt()
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

int calculaQtdVeiculos(FILE* arq_veiculos){
  qtdVeiculos = 0;
  while (!feof(arq_veiculos))
    if (fgetc(arq_veiculos) == '\n' || feof(arq_veiculos))    // calcula a quantidade de veiculos
      qtdVeiculos++;                                          // por meio da quantidade de '\n'
  return qtdVeiculos;
}

int convertCharToInt(char num){     // conversão de char para inteiro
  return (num -48);
}

void setVeiculo(int qtdVeiculos, FILE *arq_veiculos, Auto *veiculo){
  char c;
  for (int i=0; i<qtdVeiculos; i++){
    for(int j=0; (c = fgetc(arq_veiculos)) != '\n' && !feof(arq_veiculos); j++){
      if (j==0){

        veiculo[i].id = c;
      }
      else if (j==2)
        veiculo[i].tamanho = convertCharToInt(c);     // atribuições das caracteristicas de cada
      else if (j==4)                                  // automovel na struct
        veiculo[i].direcao = c;
      else if (j==7)
        veiculo[i].x = convertCharToInt(c);
      else if (j==9)
        veiculo[i].y = convertCharToInt(c);
    }
    fscanf(arq_veiculos, "\n");
  }
}

Auto* leituraConfigInicial(FILE* arq_veiculos){
  int qtdVeiculos = calculaQtdVeiculos(arq_veiculos);
  Auto *veiculo = malloc(sizeof(Auto)*qtdVeiculos);  // aloca memoria para cada veiculo
  rewind(arq_veiculos);         // seta o ponteiro do arquivo para o inicio do mesmo
  setVeiculo(qtdVeiculos, arq_veiculos, veiculo);
  fclose(arq_veiculos);
  return veiculo;
}

int verificaSinal(char sinal, int num){   // recebe o sinal matematico no tipo char
  if (sinal == '-')                       // e um número e retorna este número com
    return -num;                          // seu respectivo sinal
  else
    return num;
}

Movimento leituraManobra(FILE *arq_manobra){

  Movimento manobra; char c;
  manobra.sinal = '+';
  for(int i=0; (c = fgetc(arq_manobra)) != '\n' && !feof(arq_manobra); i++){  //armazena as configurações
    if (i == 0)                                                               //de manobra de apenas uma linha
      manobra.id = c;
    else if (i == 2)
      manobra.direcao = c;
    else if (i == 4 && c == '-')
      manobra.sinal = c;
    else if ((i == 4 && c != '+' && c != '-') || i == 5)
      manobra.amplitude = verificaSinal(manobra.sinal, convertCharToInt(c));
  }
  fscanf(arq_manobra, "\n");
  return manobra;
}

void imprimeTempo(double user_time, double system_time, double process_time, double clocktime){
  printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~- ESTATÍSTICAS DE TEMPO DE EXECUÇÃO -~-~-~-~-~-~-~-~-~-~-~\n");
  printf("%fs (tempo de usuário) + %fs (tempo de sistema) = %fs (tempo total)\n", user_time, system_time, process_time);
  printf("%fs (tempo de execução)\n", clocktime);
  printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
}

int leituraExecucaoManobra(Auto *veiculo, int **mapa, FILE* arq_manobras){

  double utime_ant, utime_pos, stime_ant, stime_pos;
  struct timeval clocktime_ant, clocktime_pos;
  int flag;

  clocktime_ant = contaTempoRelogio(); //marca tempo inicial
  contaTempoProcessador(&utime_ant, &stime_ant); //marca tempo inicial

  while(!feof(arq_manobras)){
    if((flag = realizaManobra(qtdVeiculos, veiculo, leituraManobra(arq_manobras), mapa)) != 1)
      break;
  }
  clocktime_pos = contaTempoRelogio(); //marca tempo final
  contaTempoProcessador(&utime_pos, &stime_pos); //marca tempo final
  imprimeTempo(utime_pos-utime_ant, stime_pos-stime_ant, (utime_pos-utime_ant)
  + (stime_pos-stime_ant), ((double)(clocktime_pos.tv_sec-clocktime_ant.tv_sec))
  + ((double)(clocktime_pos.tv_usec-clocktime_ant.tv_usec)/1000000));
                                    // o retorno da função realizaManobra() é o responsável por avisar
  if (flag != 1){                   // se houver problemas na execução das manobras
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
    if(!verificaSaidaZ(veiculo, qtdVeiculos)){            // verifica se o veiculo chegou
      printf("O veículo Z não chegou até à saída.\n");    // até a saída do estacionamento
      return 0;
    }
    printf("O veículo Z chegou até à saída.\n");
    fclose(arq_manobras);
    free(veiculo);
    return 1;
  }
}

void imprimeMapa(int **mapa){
  printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
  printf("MAPA DO ESTACIONAMENTO\n");
  for (int i=SIZEMAP-1; i>=0; i--){
    for (int j=0; j<SIZEMAP; j++){
      printf("%d ", mapa[i][j]);
    }
    free(mapa[i]);
    printf("\n");
  }
  free(mapa);
  printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
}
