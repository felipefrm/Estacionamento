#include <stdio.h>
#include <stdlib.h>
#include "manobras.h"
#include "configEstacionamento.h"

int verificaTrajeto(Auto veiculo, int **mapa, Movimento manobra){

  if(manobra.amplitude == 0)
    return 1;

  int inicio, final;
  if( veiculo.direcao == manobra.direcao){          //Se as direções forem iguais, verifica se o
      if( veiculo.direcao == 'X'){                  //caminho que o carro terá de percorrer na linha está livre
        if(manobra.sinal == '-'){                   //se não estiver livre, retorna 3 para colisão com outro veículo
          inicio = veiculo.x-2;
          final = veiculo.x-1 + manobra.amplitude;
        }
        else {
          inicio = veiculo.x-1 + veiculo.tamanho;
          final = veiculo.x-1 + manobra.amplitude + veiculo.tamanho-1;
      }
        }
      else if( veiculo.direcao == 'Y'){
        if(manobra.sinal == '-'){
          inicio = veiculo.y-2;
          final = veiculo.y-1 + manobra.amplitude;
        }
        else {
          inicio = veiculo.y-1 + veiculo.tamanho;
          final = veiculo.y-1 + manobra.amplitude + veiculo.tamanho-1;
        }
      }
      verificaInicioeFinal(&inicio,&final);

      if(!verificaColisaoParede(final) || !verificaColisaoParede(inicio))  // Se a manobra tentar avançar além da parede, retorna 2
        return 2;

      if(!verificaLinhaLivre(veiculo, veiculo.direcao, inicio, final, mapa)) // Se houver algo no trajeto linear, retorna 3
        return 3;
  }
  else {
    if(veiculo.direcao == 'X'){   //Se a direção do veiculo for X, a da manobra será Y
      if(manobra.sinal == '-')
        inicio = veiculo.y-2;
      else
        inicio = veiculo.y;
      final = veiculo.y-1 + manobra.amplitude;

      verificaInicioeFinal(&inicio, &final);

      if(!verificaColisaoParede(final) || !verificaColisaoParede(inicio))
        return 2;

      int frente = veiculo.x-1 + veiculo.tamanho-1,
          traseira = veiculo.x-1;

      if(!verificaMatrizLivre(veiculo, inicio, final, traseira, frente, mapa))
        return 3;

    }
    else if (veiculo.direcao == 'Y'){
      if (manobra.sinal == '-')
        inicio = veiculo.x-2;
      else
        inicio = veiculo.x;
      final = veiculo.x-1 + manobra.amplitude;

      if(!verificaColisaoParede(final) || !verificaColisaoParede(inicio))
        return 2;

      verificaInicioeFinal(&inicio, &final);

      int frente = veiculo.y-1+veiculo.tamanho-1,
          traseira = veiculo.y-1;
      if(!verificaMatrizLivre(veiculo, inicio, final, traseira, frente, mapa))
        return 3;
    }
  }
  return 1;
}


int verificaInicioeFinal(int *inicio, int *final){
  int aux;
  if ( *final < *inicio ){
    aux = *final;
    *final = *inicio;
    *inicio = aux;
  }
  return 1;
}


int verificaLinhaLivre(Auto veiculo, char direcao, int inicio, int final, int** mapa){
  if(direcao == 'X'){
    for(int i = inicio; i <= final; i++){
      if(mapa[veiculo.y-1][i] == 1)
        return 0;
    }
  }
  else {
    for(int i = inicio; i <= final; i++){
      if(mapa[i][veiculo.x-1] == 1)
        return 0;
    }
  }
}


int verificaMatrizLivre(Auto veiculo, int inicio, int final, int traseira, int frente, int** mapa){
  if(veiculo.direcao == 'X'){
    for(int i = traseira; i <= frente; i++){
      for(int j = inicio; j <= final; j++){
        if(mapa[j][i] == 1)
          return 0;
      }
    }
  }
  else {
    for(int i = traseira; i <= frente; i++){
      for(int j = inicio; j <= final; j++){
        if (mapa[i][j] == 1)
          return 0;
      }
    }
  }
}


int verificaColisaoParede(int i){
  if (i > SIZEMAP-1 || i < 0)
    return 0;
  else
    return 1;
}


int verificaSaidaZ(Auto *veiculo, int qtdVeiculos){
  for (int i=0; i<qtdVeiculos; i++){
    if (veiculo[i].id == 'Z'){
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


void apagaPosAnterior(Auto veiculo, int **mapa){
  if (veiculo.direcao == 'X'){
    mapa[veiculo.y-1][veiculo.x-1] = 0;
    mapa[veiculo.y-1][veiculo.x] = 0;
    if (veiculo.tamanho == TRUCK)
      mapa[veiculo.y-1][veiculo.x+1] = 0;
  }
  else if (veiculo.direcao == 'Y'){
    mapa[veiculo.y-1][veiculo.x-1] = 0;
    mapa[veiculo.y][veiculo.x-1] = 0;
    if (veiculo.tamanho == TRUCK)
      mapa[veiculo.y+1][veiculo.x-1] = 0;
  }
}


int movimentaVeiculo(Auto *veiculo, int **mapa, Movimento manobra){
  if (manobra.direcao == 'X'){
    veiculo->x +=  manobra.amplitude;
  }

  else if (manobra.direcao == 'Y'){
    veiculo->y += manobra.amplitude;
  }

  if (veiculo->direcao == 'Y'){
    for (int i = veiculo->y-1; i < veiculo->y-1+veiculo->tamanho; i++){
      if(!verificaColisaoParede(i)){
        return 2;   // retorna 2 se bater for bater no muro
      }
      if (mapa[i][veiculo->x-1]!= 0){
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
        return 2;
      }
      if (mapa[veiculo->y-1][i]!= 0){
        return 3;
    }
      else if (mapa[veiculo->y-1][i] == 0)
        mapa[veiculo->y-1][i] = 1;
    }
  }
  return 1;
}


int realizaManobra(int qtdVeiculos, Auto *veiculo, Movimento manobra, int **mapa){
  int flag = 0;
  for (int i=0; i<qtdVeiculos; i++){
    if (veiculo[i].id == manobra.id){
      flag = verificaTrajeto(veiculo[i], mapa, manobra);
      if (flag == 2 || flag == 3){
        return flag;
      }
      apagaPosAnterior(veiculo[i], mapa);
      return movimentaVeiculo(&veiculo[i], mapa, manobra);
    }
  }
  return manobra.id; // se chegar até aqui é porque não existe nenhum veiculo com o id igual ao da manobra.
}                    // retorna-se o id para poder ser especificado na mensagem de erro qual é a manobra
                     // que está causando o conflito
