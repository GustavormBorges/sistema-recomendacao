#ifndef SIMILARIDADE_H
#define SIMILARIDADE_H

#include "ListaCompras.h"
using namespace std;

// As matrizes deste modulo sao criadas com malloc (estilo C): um "int **" e um
// vetor de ponteiros, cada um apontando para uma linha. Quem cria tambem precisa
// liberar com free, usando as funcoes de liberar abaixo.

// Cria uma matriz de inteiros (linhas x colunas) cheia de 0.
int **criarMatrizInt(int linhas, int colunas);

// Cria uma matriz de numeros com virgula (linhas x colunas) cheia de 1.0.
double **criarMatrizDouble(int linhas, int colunas);

// Liberam (free) uma matriz criada pelas funcoes acima.
void liberarMatrizInt(int **M, int linhas);
void liberarMatrizDouble(double **M, int linhas);

// Cria a matriz de compras A (n clientes x m produtos) e guarda em lista.matrizCompras:
// A[i][p] = 1 se o cliente i comprou o produto p.
void criarMatrizCompras(ListaCompras &lista);

// Libera a matriz de compras guardada em lista.matrizCompras.
void liberarMatrizCompras(ListaCompras &lista);

// Matriz transposta: T[j][i] = A[i][j]. A tem "linhas" x "colunas"; T tem colunas x linhas.
int **calcularTransposta(int **A, int linhas, int colunas);

// Multiplicacao de matrizes padrao (3 lacos): C = A x B.
// A tem m x n, B tem n x p, e o resultado C tem m x p.
int **multiplicarMatrizes(int **A, int m, int n, int **B, int p);

// Matriz de similaridade S (n x n):  S[i][j] = 1 - |Pi interseccao Pj| / |Pi|
// Quanto MENOR o valor, MAIS parecidos sao os clientes i e j.
// Valor 1 significa que nao compraram nada em comum.
// Se a matriz de compras ainda nao existe, ela e criada aqui.
// Atencao: a matriz e densa e usa muita memoria (cerca de 12 bytes x n x n).
// Para n = 3000 clientes sao uns 100 MB. Quem chama deve liberar com liberarMatrizDouble.
double **calcularSimilaridade(ListaCompras &lista);

// Devolve a similaridade s(i, j) entre o cliente i e o cliente j (indices internos),
// lida da matriz densa S. Quem usa o modulo chama esta funcao em vez de acessar
// S[i][j] direto. O "Densa" no nome e porque a Atividade 5 vai ter uma versao
// para a matriz esparsa (CSR).
double similaridadeDensa(double **S, int i, int j);

#endif
