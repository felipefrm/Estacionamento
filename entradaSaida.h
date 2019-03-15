#ifndef ENTRADASAIDA
#define ENTRADASAIDA

#define CAR 2
#define TRUCK 3

struct arquivos {
  FILE* manobras;     // estrutura que receberá o ponteiro dos arquivos
  FILE* veiculos;     // de veiculos e manobras
}; typedef struct arquivos Arquivos;

struct automovel {
  char id, direcao;    // estrutura que especifíca cada veiculo
  int x, y, tamanho;
}; typedef struct automovel Auto;

struct movimento {
  char id, direcao, sinal;   // estrutura com as configurações das manobras
  int amplitude;
}; typedef struct movimento Movimento;

int qtdVeiculos;

Arquivos* argumentosEntrada(int argc, char* argv[]);
int calculaQtdVeiculos(FILE* arq_veiculos);
void setVeiculo(int qtdVeiculos, FILE *arq_veiculos, Auto *veiculo);
Auto* leituraConfigInicial(FILE* arq_veiculos);
Movimento leituraManobra(FILE *arq_manobra);
int leituraExecucaoManobra(Auto *veiculo, int **mapa, FILE* arq_manobras);
void imprimeTempo(double user_time, double system_time, double process_time, double clocktime);
void imprimeMapa(int **mapa);

//funções auxiliares
int verificaSinal(char sinal, int num);
int convertCharToInt(char num);

#endif
