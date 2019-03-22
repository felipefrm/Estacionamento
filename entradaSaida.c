#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "entradaSaida.h"
#include "manobras.h"
#include "configEstacionamento.h"


Arquivos* argumentosEntrada(int argc, char* argv[]){

  Arquivos* arq = malloc(sizeof(Arquivos));   // armazena os ponteiros dos arquivos
  int opcao;
   arq->flag = 1;                                  // passados por linha de comando em uma struct
  if (argc < 5){
    printf("Use: ./tp -m [ARQUIVO DE MANOBRA] -v [ARQUIVO DE CONFIGURAÇÃO INICIAL]\n");
    arq->flag = 0;
    return arq;
  }

  while((opcao = getopt(argc, argv, "m:v:"))!= -1)    // recebe-se os argumentos por meio
  {                                                   // da funcao getopt()
    switch(opcao)
    {
      case 'm':
        if(!(arq->manobras = fopen(optarg, "r"))) {
          printf("Erro na abertura do arquivo.\n");
          arq->flag = 0;
          return arq;
        }
        break;
      case 'v':
        if(!(arq->veiculos = fopen(optarg, "r"))) {
          printf("Erro na abertura do arquivo.\n");
          arq->flag = 0;
          return arq;
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


void setVeiculo(int qtdVeiculos, FILE *arq_veiculos, Auto *veiculo){
  char c;
  for (int i=0; i<qtdVeiculos; i++){
    for(int j=0; (c = fgetc(arq_veiculos)) != '\n' && !feof(arq_veiculos); j++){
      if (j==0)
        veiculo[i].id = c;
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
  return veiculo;
}


Movimento leituraManobra(FILE *arq_manobras){

  Movimento manobra; char c;
  manobra.sinal = '+';
  for(int i=0; (c = fgetc(arq_manobras)) != '\n' && !feof(arq_manobras); i++){  //armazena as configurações
    if (i == 0)                                                               //de manobra de apenas uma linha
      manobra.id = c;
    else if (i == 2)
      manobra.direcao = c;
    else if (i == 4 && c == '-')
      manobra.sinal = c;
    else if ((i == 4 && c != '+' && c != '-') || i == 5)
      manobra.amplitude = verificaSinal(manobra.sinal, convertCharToInt(c));
  }
  fscanf(arq_manobras, "\n");
  return manobra;
}


int leituraExecucaoManobra(Auto *veiculo, int **mapa, FILE* arq_manobras){

  int flag;
  printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
  while(!feof(arq_manobras)){
    if((flag = realizaManobra(qtdVeiculos, veiculo, leituraManobra(arq_manobras), mapa)) != 1)
    break;
  }
                  // o retorno da função realizaManobra() é o responsável por avisar
  if (flag != 1){                   // se houver problemas na execução das manobras
    if (flag == WALL_COLLISION)
    printf("Conjunto de manobras inviável!\nMOTIVO: Colisão com o muro.\n");
    else if (flag == CAR_COLLISION)
    printf("Conjunto de manobras inviável!\nMOTIVO: Colisão com um veículo.\n");
    else
    printf("INCOMPATIBILIDADE DE DADOS... Não há veículo identificado como %c.\n", flag);
    return 0;
  }

  else {
    if(!verificaSaidaZ(veiculo, qtdVeiculos)){                              // verifica se o veiculo chegou
      printf("O veículo Z não obteve sucesso em chegar até à saída.\n");    // até a saída do estacionamento
      return 0;
    }
    printf("O veículo Z obteve sucesso em chegar até à saída.\n");
    return 1;
  }
}


void imprimeMapa(int **mapa){
  printf("MAPA DO ESTACIONAMENTO\n");
  for (int i=SIZEMAP-1; i>=0; i--){
    for (int j=0; j<SIZEMAP; j++){
      printf("%d ", mapa[i][j]);
    }
    printf("\n");
  }
  printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
}


int verificaArqVazio(FILE* arq){
  int tamanho_arq;
  fseek (arq, 0, SEEK_END);               // aponta para o fim do arquivo com fseek()
  if((tamanho_arq = ftell (arq)) == 0){   // retorna o valor da posição do ponteiro com ftell()
    printf("O arquivo de manobras está vazio!\n");
    return 0;
  }
  rewind(arq);   // retorna o ponteiro para o inicio do arquivo, para os proximos
  return 1;      // procedimentos
}


void liberaPonteiros(Auto *veiculo, Arquivos *arq, int **mapa){
  free(veiculo);
  fclose(arq->manobras);
  fclose(arq->veiculos);
  free(arq);
  for (int i=0; i<SIZEMAP; i++)
    free(mapa[i]);
  free(mapa);
}


int verificaSinal(char sinal, int num){   // recebe o sinal matematico no tipo char
  if (sinal == '-')                       // e um número e retorna este número com
  return -num;                          // seu respectivo sinal
  else
  return num;
}


int convertCharToInt(char num){     // conversão de char para inteiro
  return (num -48);
}


void contaTempoProcessador(double *utime, double *stime){
  struct rusage resources;
  getrusage(RUSAGE_SELF, &resources);
  *utime = (double) resources.ru_utime.tv_sec + 1.e-6 * (double) resources.ru_utime.tv_usec;
  *stime = (double) resources.ru_stime.tv_sec + 1.e-6 * (double) resources.ru_stime.tv_usec;
}


void imprimeTempo(double user_time, double system_time){
  printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
  printf("%fs (tempo de usuário) + %fs (tempo de sistema) = %fs (tempo total)\n", user_time, system_time, user_time+system_time);
  printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
}
