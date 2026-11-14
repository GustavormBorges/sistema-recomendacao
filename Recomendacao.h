#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include <vector>
#include "ListaCompras.h"
#include "Similaridade.h"
using namespace std;

// Uma posicao do vetor de ranqueamento: qual produto e qual a sua nota.
// Quanto MENOR o valor, MELHOR o produto esta ranqueado.
struct ItemRanking {
    int produto;   // indice do produto
    double valor;  // valor de ranqueamento
};

// Devolve os (ate) k produtos mais bem ranqueados para o cliente, do melhor
// para o pior. Pode devolver menos de k se nao houver produtos suficientes.
vector<ItemRanking> recomenda(ListaCompras &lc, vector<vector<double> > &S,
                              int cliente, int k);

#endif
