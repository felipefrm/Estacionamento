#ifndef ENTRADASAIDA
#define ENTRADASAIDA

#define CAR 2
#define TRUCK 3


struct arquivos {
  FILE* manobras;     // estrutura que receberá o ponteiro dos arquivos
  FILE* veiculos;     // de veiculos e manobras
  int flag;   // flag para pode retornar 0 ou 1
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
void imprimeMapa(int **mapa);
int verificaArqVazio(FILE* arq, Auto *veiculo);
void liberaPonteiros(Auto *veiculo, Arquivos *arq, int **mapa);

//funções auxiliares
int verificaSinal(char sinal, int num);
int convertCharToInt(char num);

//funções de análise de tempo
void contaTempo(double *usertime, double *systemtime);
void contaTempoProcessador(double *utime, double *stime);
void imprimeTempo(double user_time, double system_time);

#endif
