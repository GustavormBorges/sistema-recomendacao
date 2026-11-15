#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include <vector>
#include "ListaCompras.h"
#include "Similaridade.h"
using namespace std;

// Um item do ranqueamento: qual produto e qual a sua nota (score).
// Quanto MENOR o score, MELHOR o produto esta ranqueado.
struct item {
    int produto;    // indice do produto
    double score;   // nota de ranqueamento
};

// Devolve os (ate) k produtos mais bem ranqueados para o cliente, do melhor
// para o pior. Pode devolver menos de k se nao houver produtos suficientes.
// S e a matriz de similaridade (criada por calcularSimilaridade).
vector<item> recomendar(ListaCompras &lista, double **S, int cliente, int k);

#endif
