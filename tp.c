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

int main(void){
                                  // formação do estacionamento
  int **mapa = criaMapa();        // atraves de uma matriz

  FILE *config;
  config = fopen("entrada.txt", "r");

  int qtdVeiculos = 0;
  while (!feof(config))
    if (fgetc(config) == '\n')    // calcula a quantidade de veiculos
      qtdVeiculos++;              // por meio da quantidade de '\n'

  Auto *veiculo = malloc(sizeof(Auto)*qtdVeiculos);  // aloca memoria para cada veiculo

  rewind(config);    // seta o ponteiro do arquivo para o inicio do mesmo
  char c;
  for (int i=0; i<=qtdVeiculos; i++){
    for(int j=0; (c = fgetc(config)) != '\n'; j++){
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
      else if (c == EOF)
        break;
    }
  }
  fclose(config);

  for(int i=0; i<=qtdVeiculos; i++){

    if (veiculo[i].tamanho == '2' && veiculo[i].direcao == 'X'){
      printf("%d %d\n",veiculo[i].x,veiculo[i].y);
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;
      mapa[veiculo[i].y-1][veiculo[i].x] = 1;
    }
    else if (veiculo[i].tamanho == '2' && veiculo[i].direcao == 'Y'){
      printf("%d %d\n",veiculo[i].x,veiculo[i].y);
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;
      mapa[veiculo[i].y][veiculo[i].x-1] = 1;   // configuração inicial do mapa
                                                // ja com os veiculos
    }
    else if (veiculo[i].tamanho == '3' && veiculo[i].direcao == 'X'){
      printf("%d %d\n",veiculo[i].x,veiculo[i].y);
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;
      mapa[veiculo[i].y-1][veiculo[i].x] = 1;     // 1 para carro
      mapa[veiculo[i].y-1][veiculo[i].x+1] = 1;   // 1 para caminhão

    }
    else if (veiculo[i].tamanho == '3' && veiculo[i].direcao == 'Y'){
      printf("%d %d\n",veiculo[i].x,veiculo[i].y);
      mapa[veiculo[i].y-1][veiculo[i].x-1] = 1;
      mapa[veiculo[i].y][veiculo[i].x-1] = 1;
      mapa[veiculo[i].y+1][veiculo[i].x-1] = 1;

    }
  }


  FILE *arq_manobra;
  arq_manobra = fopen("manobras.txt", "r");
  Movimento manobra;
  while (!feof(arq_manobra)){
    for(int i=0; (c = fgetc(arq_manobra)) != '\n' && !feof(arq_manobra); i++){  //armazena as configurações
      manobra.sinal = '+';                                //das manobras linha por linha
      if (i == 0)
        manobra.id = c;
      else if (i == 2)
        manobra.direcao = c;
      else if (i == 4 && c == '-')
        manobra.sinal = c;
      else if ((i == 4 && c != '+' && c != '-') || i == 5)
        manobra.amplitude = verificaSinal(manobra.sinal, convertCharToInt(c));
      printf("%c", c);
    }
    printf("\n");
    for (int i=0; i<=qtdVeiculos; i++){
      if (veiculo[i].id == manobra.id){
        if (manobra.direcao == 'X'){
          mapa[veiculo[i].y-1][veiculo[i].x+manobra.amplitude] = 1;
        }
        if (manobra.direcao == 'Y'){
          mapa[veiculo[i].y-5+manobra.amplitude][veiculo[i].x-1] = 1;
        }

      }
    }
  }
  for (int i=SIZEMAP-1; i>=0; i--){
    for (int j=0; j<SIZEMAP; j++){
      printf("%d ", mapa[i][j]);
    }
    printf("\n");
  }
}
