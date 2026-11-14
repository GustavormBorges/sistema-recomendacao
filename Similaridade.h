#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include <vector>
#include "ListaCompras.h"
using namespace std;

// Matriz de compras A (n clientes x m produtos): A[i][p] = 1 se o cliente i comprou o produto p.
vector<vector<int> > criarMatrizCompras(ListaCompras &lista);

// Matriz transposta: T[j][i] = A[i][j].
vector<vector<int> > calcularTransposta(vector<vector<int> > &A);

// Multiplicacao de matrizes padrao (3 lacos): C = A x B.
vector<vector<int> > multiplicarMatrizes(vector<vector<int> > &A, vector<vector<int> > &B);

// Matriz de similaridade S (n x n):  S[i][j] = 1 - |Pi interseccao Pj| / |Pi|
// Quanto MENOR o valor, MAIS parecidos sao os clientes i e j.
// Valor 1 significa que nao compraram nada em comum.
// Atencao: a matriz e densa e usa muita memoria (cerca de 12 bytes x n x n).
// Para n = 3000 clientes sao uns 100 MB.
vector<vector<double> > calcularSimilaridade(ListaCompras &lista);

// Devolve a similaridade s(i, j) entre o cliente i e o cliente j (indices internos),
// lida da matriz densa S. Quem usa o modulo chama esta funcao em vez de acessar
// S[i][j] direto. O "Densa" no nome e porque a Atividade 5 vai ter uma versao
// para a matriz esparsa (CSR).
double similaridadeDensa(vector<vector<double> > &S, int i, int j);

#endif
