#include <stdio.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "manobras.h"

int calculaQtdVeiculos(FILE* arq){
  while (!feof(arq))
    if (fgetc(arq) == '\n')    // calcula a quantidade de veiculos
      qtdVeiculos++;              // por meio da quantidade de '\n'
}

int convertCharToInt(char num){     // conversão de char para inteiro
  return (num -48);
}

void setVeiculo(int qtdVeiculos, FILE *arq, Auto *veiculo){
  char c;
  for (int i=0; i<=qtdVeiculos; i++){
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
  }
}

Auto* leituraConfigInicial(){
  FILE *arq;
  arq = fopen("entrada.txt", "r");
  int qtdVeiculos = calculaQtdVeiculos(arq);
  Auto *veiculo = malloc(sizeof(Auto)*qtdVeiculos);  // aloca memoria para cada veiculo
  rewind(arq);    // seta o ponteiro do arquivo para o inicio do mesmo
  setVeiculo(qtdVeiculos, arq, veiculo);
  fclose(arq);
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

int leituraExecucaoManobra(Auto *veiculo, int **mapa){
  FILE* arq;
  arq = fopen("manobras.txt", "r");
  double utime_ant, utime_pos, stime_ant, stime_pos, diftempo;
  contaTempo(&utime_ant, &stime_ant); //marca tempo inicial
  while(!feof(arq))
    if(!realizaManobra(qtdVeiculos, veiculo, leituraManobra(arq), mapa)){
      printf("Manobra impossível.\n");
      fclose(arq);
      return 0;
    }
  contaTempo(&utime_pos, &stime_pos); //marca tempo inicial
  printf("Tempo de usuário: %fs + Tempo de sistema %fs = %f.\n", utime_pos-utime_ant, stime_pos-stime_ant, (utime_pos-utime_ant) + (stime_pos-stime_ant));
  fclose(arq);
}
