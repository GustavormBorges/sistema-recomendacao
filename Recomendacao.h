#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include <vector>
#include "ListaCompras.h"
#include "Similaridade.h"
using namespace std;

// Uma posição do vetor de ranqueamento: qual produto e qual a sua nota.
// Quanto MENOR o valor, MELHOR o produto está ranqueado.
struct ItemRanking {
    int produto;   // índice interno do produto
    double valor;  // valor de ranqueamento
};

// Devolve os (até) k produtos mais bem ranqueados para o cliente (índice interno),
// do melhor para o pior. Pode devolver menos de k se não houver produtos suficientes.
vector<ItemRanking> recomendar(const ListaCompras &lc, const MatrizDouble &S,
                               int cliente, int k);

#endif
