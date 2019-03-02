#include <stdio.h>
#include <stdlib.h>

#define SIZEMAP 6

struct automovel {
  char id, tamanho, direcao;    // estrutura que especifíca cada veiculo
  int x, y;
}; typedef struct automovel Auto;

struct movimento {
  char id, direcao, sinal;
  int amplitude;
}; typedef struct movimento Movimento;

int convertCharToInt(char num){     // conversão de char para inteiro
  return (num -48);
}

int** criaMapa(){
  int **mapa = malloc(sizeof(int*)*SIZEMAP);
  for (int i=0; i<SIZEMAP; i++){
    mapa[i] = malloc(sizeof(int)*SIZEMAP);    // formação do estacionamento
    for (int j=0; j<SIZEMAP; j++)             // atraves de uma matriz
      mapa[i][j] = 0;
    }
  return mapa;
}

int verificaSinal(char sinal, int num){
  if (sinal == '-')
    return -num;
  else
    return num;
}

int calculaQtdVeiculos(FILE* arq){
  int qtdVeiculos = 0;
  while (!feof(arq))
    if (fgetc(arq) == '\n')
      qtdVeiculos++;  // por meio da quantidade de '\n'
                      // calcula a quantidade de veiculos

  return qtdVeiculos;
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

void configInicialMapa(int qtdVeiculos, Auto* veiculo, int **mapa){
  for(int i=0; i<=qtdVeiculos; i++){

    if (veiculo[i].direcao == 'X'){
      printf("%d %d\n",veiculo[i].x,veiculo[i].y);
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;
      mapa[veiculo[i].y-1][veiculo[i].x] = 1;
      if (veiculo[i].tamanho == '3')
        mapa[veiculo[i].y-1][veiculo[i].x+1] = 1;   // 1 para caminhão
    }
    else if (veiculo[i].direcao == 'Y'){
      printf("%d %d\n",veiculo[i].x,veiculo[i].y);
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;  // configuração inicial do mapa
      mapa[veiculo[i].y][veiculo[i].x-1] = 1;   // ja com os veiculos
      if (veiculo[i].direcao == '3')
        mapa[veiculo[i].y+1][veiculo[i].x-1] = 1;
    }
  }
}

Movimento leituraManobras(FILE *arq_manobra){

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

void imprimeMapa(int **mapa){
  for (int i=SIZEMAP-1; i>=0; i--){
    for (int j=0; j<SIZEMAP; j++){
      printf("%d ", mapa[i][j]);
    }
    printf("\n");
  }
}

void realizaManobra(int qtdVeiculos, Auto *veiculo, Movimento manobra, int **mapa){
  printf("\n");
  char flag = 0;
  for (int i=0; i<=qtdVeiculos; i++){
    printf("%c e %c\n", veiculo[i].id, manobra.id);
    if (veiculo[i].id == manobra.id){
      flag = 1;
      if (manobra.direcao == 'X'){
        mapa[veiculo[i].y-1][veiculo[i].x-1] = 0;
        mapa[veiculo[i].y-1][veiculo[i].x] = 0;
        mapa[veiculo[i].y-1][veiculo[i].x-1+manobra.amplitude] = 1;
        mapa[veiculo[i].y-1][veiculo[i].x+manobra.amplitude] = 1;
        if (veiculo[i].tamanho == '3'){
          mapa[veiculo[i].y-1][veiculo[i].x+1] = 0;
          mapa[veiculo[i].y-1][veiculo[i].x+1+manobra.amplitude] = 1;
        }
      }
      else if (manobra.direcao == 'Y'){
        mapa[veiculo[i].y-1][veiculo[i].x-1] = 0;
        mapa[veiculo[i].y][veiculo[i].x-1] = 0;
        mapa[veiculo[i].y-1+manobra.amplitude][veiculo[i].x-1] = 1;
        mapa[veiculo[i].y+manobra.amplitude][veiculo[i].x-1] = 1;
        if (veiculo[i].tamanho == '3'){
          mapa[veiculo[i].y+1][veiculo[i].x-1] = 0;
          mapa[veiculo[i].y+1+manobra.amplitude][veiculo[i].x-1] = 1;
        }
      }
    }
  }
  if (flag == 0)
    printf("INCOMPATIBILIDADE DE DADOS... Não há veículo identificado como %c.\n", manobra.id);
}

int main(void){
                                  // formação do estacionamento
  int **mapa = criaMapa();        // atraves de uma matriz

  FILE *arq_config;
  arq_config = fopen("entrada.txt", "r");
  int qtdVeiculos = calculaQtdVeiculos(arq_config);
  printf("qtd: %d\n", qtdVeiculos);
  Auto *veiculo = malloc(sizeof(Auto)*qtdVeiculos);  // aloca memoria para cada veiculo
  rewind(arq_config);    // seta o ponteiro do arquivo para o inicio do mesmo
  setVeiculo(qtdVeiculos, arq_config, veiculo);
  fclose(arq_config);

  configInicialMapa(qtdVeiculos, veiculo, mapa);

  FILE *arq_manobra;
  arq_manobra = fopen("manobras.txt", "r");

  while (!feof(arq_manobra))
    realizaManobra(qtdVeiculos, veiculo, leituraManobras(arq_manobra), mapa);
  
  fclose(arq_manobra);
  imprimeMapa(mapa);
}
