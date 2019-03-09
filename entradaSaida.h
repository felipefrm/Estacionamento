#ifndef ENTRADASAIDA
#define ENTRADASAIDA

struct automovel {
  char id, tamanho, direcao;    // estrutura que especifíca cada veiculo
  int x, y;
}; typedef struct automovel Auto;

struct movimento {
  char id, direcao, sinal;
  int amplitude;
}; typedef struct movimento Movimento;

int qtdVeiculos;

int convertCharToInt(char num);
int calculaQtdVeiculos(FILE* arq);
void setVeiculo(int qtdVeiculos, FILE *arq, Auto *veiculo);
Auto* leituraConfigInicial();
int verificaSinal(char sinal, int num);
Movimento leituraManobra(FILE *arq_manobra);
int leituraExecucaoManobra(Auto *veiculo, int **mapa);

#endif
