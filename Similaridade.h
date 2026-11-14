#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include <vector>
#include "ListaCompras.h"
using namespace std;

typedef vector<vector<int> > MatrizInt;
typedef vector<vector<double> > MatrizDouble;

// Matriz de compras A (n clientes x m produtos): A[i][p] = 1 se o cliente i comprou o produto p.
MatrizInt criarMatrizCompras(const ListaCompras &lc);

// Matriz transposta: T[j][i] = A[i][j].
MatrizInt transposta(const MatrizInt &A);

// Multiplicação de matrizes padrão (3 laços): C = A x B.
MatrizInt multiplicar(const MatrizInt &A, const MatrizInt &B);

// Matriz de similaridade S (n x n):  S[i][j] = 1 - |Pi interseção Pj| / |Pi|
// Quanto MENOR o valor, MAIS parecidos são os clientes i e j. Valor 1 = nada em comum.
// Atenção: a matriz é densa, então usa muita memória (cerca de 12 bytes x n x n).
// Para n = 3000 clientes são ~100 MB.
MatrizDouble calcularSimilaridade(const ListaCompras &lc);

#endif
