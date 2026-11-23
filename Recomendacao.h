#ifndef RECOMENDACAO_H
#define RECOMENDACAO_H

#include <vector>
#include "ListaCompras.h"
#include "Similaridade.h"
using namespace std;

struct item {
    int produto;
    double score;
};

vector<item> recomendar(ListaCompras &lista, int cliente, int k);

#endif

