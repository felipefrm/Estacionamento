#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "manobras.h"
#include "entradaSaida.h"


int verificaTrajeto(Auto veiculo, int **mapa, Movimento manobra){
  int inicio,final,i,j;
  if( veiculo.direcao == manobra.direcao){          //Se as direções forem iguais, verifica se o
      if( veiculo.direcao == 'X'){                  //caminho que o carro terá de percorrer na linha está livre
        if(manobra.sinal == '-'){                   //se não estiver livre, retorna 3 para colisão com outro veículo
          inicio = veiculo.x-2;
          final = veiculo.x-1 + manobra.amplitude;
        }else{
          inicio = veiculo.x-1 + veiculo.tamanho;
          final = veiculo.x-1 + manobra.amplitude + veiculo.tamanho-1;
      }
        //printf("\n inicio:%d, final:%d",inicio,final);
      }
      else if( veiculo.direcao == 'Y'){
        if(manobra.sinal == '-'){
          inicio = veiculo.y-2;
          final = veiculo.y-1 + manobra.amplitude;
        }else{
          inicio = veiculo.y-1 + veiculo.tamanho;
          final = veiculo.y-1 + manobra.amplitude + veiculo.tamanho-1;
        }
      }
      //printf("\n inicio:%d, final:%d",inicio,final);
      if( final < inicio ){
        i = final;
        final = inicio;
        inicio = i;
      }
      if(verificaColisaoParede(final) == 0)  // Se a manobra tentar avançar além da parede, retorna 2
        return 2;
      if(veiculo.direcao == 'X'){
        for( i=inicio;i<=final;i++){
          //printf("\n MAPA: %d x:%d y:%d inicio:%d final:%d\n",mapa[i][j],i,veiculo.y,inicio,final);
          if(mapa[veiculo.y-1][i] == 1)
            return 3;
        }
      }else{
        for( i=inicio;i<=final;i++){
          //printf("\n MAPA: %d x:%d y:%d inicio:%d final:%d\n",mapa[i][j],veiculo.x,i,inicio,final);
          if(mapa[i][veiculo.x-1] == 1)
            return 3;
        }
      }
  }else{
    if(veiculo.direcao == 'X'){   //Se a direção do veiculo for X, a da manobra será Y
      if(manobra.sinal == '-')
        inicio = veiculo.y-2;
      else
        inicio = veiculo.y;
      final = veiculo.y-1 + manobra.amplitude;

      if ( final < inicio ){
        i = final;
        final = inicio;
        inicio = i;
      }

      if(verificaColisaoParede(final) == 0)
        return 2;

      for(i=veiculo.x-1;i<=veiculo.x+veiculo.tamanho-2;i++){
        for(j=inicio;j<=final;j++){
          if(mapa[j][i] == 1)
            return 3;
        }
      }
    }else if(veiculo.direcao == 'Y'){
      if(manobra.sinal == '-')
        inicio = veiculo.x-2;
      else
        inicio = veiculo.x;
      final = veiculo.x-1 + manobra.amplitude;

      if(verificaColisaoParede(final) == 0)
        return 2;

      if ( final < inicio ){
        i = final;
        final = inicio;
        inicio = i;
      }
      int frente,traseira;
      traseira = veiculo.y-1;
      frente = veiculo.y-1+veiculo.tamanho-1;
      //printf("\nFrente:%d \n",frente);
      //printf("\nInicio:%d \n",inicio);
      //printf("\nFinal:%d \n",final);
      for(i=traseira;i<=frente;i++){
        for(j=inicio;j<=final;j++){
          //printf("\nMapa:%d x:%d y:%d\n",mapa[i][j],j,i);
          if (mapa[i][j] == 1)
            return 3;
        }
      }
    }
  }
  return 1;
}

int verificaColisaoParede(int i){
  if (i > 5 || i<0)
    return 0;
  else
    return 1;
}

int verificaSaidaZ(Auto *veiculo, int qtdVeiculos){
  for (int i=0; i<qtdVeiculos; i++){
    if (veiculo[i].id == 'Z'){
      printf("Veiculo: %c\n", veiculo[i].id);
      printf("X: %d e Y: %d\n", veiculo[i].x, veiculo[i].y);
      if (veiculo[i].direcao == 'X' && veiculo[i].tamanho == CAR){
        if((veiculo[i].x == 6 || veiculo[i].x == 5) && veiculo[i].y == 4){
          return 1;
        }
      }
      else if (veiculo[i].direcao == 'X' && veiculo[i].tamanho == TRUCK){
        if((veiculo[i].x == 6 || veiculo[i].x == 5 || veiculo[i].x == 4) && veiculo[i].y == 4){
          return 1;
        }
      }
      else if (veiculo[i].direcao == 'Y' && veiculo[i].tamanho == CAR){
        if((veiculo[i].y == 3 || veiculo[i].y == 4) && veiculo[i].x == 6){
          return 1;
        }
      }
      else if (veiculo[i].direcao == 'Y' && veiculo[i].tamanho == TRUCK){
        if((veiculo[i].y == 2 || veiculo[i].y == 3 || veiculo[i].y == 4) && veiculo[i].x == 6){
          return 1;
        }
      }
    }
  }
  return 0;
}

void contaTempoProcessador(double *utime, double *stime){
  struct rusage resources;
  getrusage(RUSAGE_SELF, &resources);
  *utime = (double) resources.ru_utime.tv_sec + 1.e-6 * (double) resources.ru_utime.tv_usec;
  *stime = (double) resources.ru_stime.tv_sec + 1.e-6 * (double) resources.ru_stime.tv_usec;
}

struct timeval contaTempoRelogio(){
  struct timeval tempo;
  gettimeofday(&tempo, NULL);
  return tempo;
}

void apagaPosAnterior(Auto veiculo, int **mapa){
  if (veiculo.direcao == 'X'){
    // printf("%d %d\n",veiculo.x,veiculo.y);
    mapa[veiculo.y-1][veiculo.x-1] = 0;
    mapa[veiculo.y-1][veiculo.x] = 0;
    if (veiculo.tamanho == TRUCK)
      mapa[veiculo.y-1][veiculo.x+1] = 0;
  }
  else if (veiculo.direcao == 'Y'){
    // printf("%d %d\n",veiculo.x,veiculo.y);
    mapa[veiculo.y-1][veiculo.x-1] = 0;
    mapa[veiculo.y][veiculo.x-1] = 0;
    if (veiculo.tamanho == TRUCK)
      mapa[veiculo.y+1][veiculo.x-1] = 0;
  }
}

int movimentaVeiculo(Auto *veiculo, int **mapa, Movimento manobra){
  if (manobra.direcao == 'X'){
    veiculo->x +=  manobra.amplitude;
    printf("X: %d\n", veiculo->x);
  }

  else if (manobra.direcao == 'Y'){
    // printf("Y: %d\n", veiculo.y);
    veiculo->y += manobra.amplitude;
  }

  if (veiculo->direcao == 'Y'){
    for (int i = veiculo->y-1; i < veiculo->y-1+veiculo->tamanho; i++){
      if(!verificaColisaoParede(i)){
        // printf("Colisão com a parede.\n");
        return 2;   // retorna 2 se bater for bater no muro
      }
      if (mapa[i][veiculo->x-1]!= 0){
        // printf("Colisão com um veículo.\n");

        return 3;     //retorna 3 se for bater em outro veiculo
      }                                             //verifica se a posição do veiculo
                                                    //depois da manobra está livre
      else                                          //se estiver, coloca a parte do veiculo no lugar
        mapa[i][veiculo->x-1] = 1;                 //se não, retorna 2 ou 3, dependendo do tipo de colisão
    }
  }
  else if (veiculo->direcao == 'X'){
    for (int i = veiculo->x-1; i < veiculo->x-1+veiculo->tamanho; i++){
      if(!verificaColisaoParede(i)){
        // printf("MOTIVO: Colisão com a parede.\n");
        return 2;
      }
      if (mapa[veiculo->y-1][i]!= 0){
        // printf("MOTIVO: Colisão com um veículo.\n");
        return 3;
    }
      else if (mapa[veiculo->y-1][i] == 0)
        mapa[veiculo->y-1][i] = 1;
    }
  }
  return 1;
}

int realizaManobra(int qtdVeiculos, Auto *veiculo, Movimento manobra, int **mapa){
  // printf("\n");
  int flag = 0;
  for (int i=0; i<qtdVeiculos; i++){
    // printf("%c e %c\n", veiculo[i].id, manobra.id);
    if (veiculo[i].id == manobra.id){
      // flag = 1;
      flag = verificaTrajeto(veiculo[i], mapa, manobra);
      if (flag == 2 || flag == 3){
          printf("%d\n", flag);
        return flag;
      }
      apagaPosAnterior(veiculo[i], mapa);
      return movimentaVeiculo(&veiculo[i], mapa, manobra);
    }
  }
  // if (!flag){
    // fprintf(stderr, "INCOMPATIBILIDADE DE DADOS... Não há veículo identificado como %c.\n", manobra.id);
    // return 0;
  // }
  return 1;
}
