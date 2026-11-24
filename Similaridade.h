#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include "ListaCompras.h"

void liberarMatrizDouble(double **M, int linhas);

void criarMatrizCompras(ListaCompras &lista);
void liberarMatrizCompras(ListaCompras &lista);

void criarMatrizCSR(ListaCompras &lista);

double **calcularSimilaridade(ListaCompras &lista, int algoritmo = 1);
void calcularSimilaridadeCSR(ListaCompras &lista);

double similaridadeCSR(ListaCompras &lista, int i, int j);
double similaridadeDensa(double **S, int i, int j);

#endif
