
#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include "ListaCompras.h"

int **criarMatrizInt(int linhas, int colunas);
double **criarMatrizDouble(int linhas, int colunas);

void liberarMatrizInt(int **M, int linhas);
void liberarMatrizDouble(double **M, int linhas);

void criarMatrizCompras(ListaCompras &lista);
void liberarMatrizCompras(ListaCompras &lista);

int **calcularTransposta(int **A, int linhas, int colunas);
int **multiplicarMatrizes(int **A, int m, int n, int **B, int p);

double **calcularSimilaridade(ListaCompras &lista, int algoritmo = 1);

double similaridadeDensa(double **S, int i, int j);

#endif

