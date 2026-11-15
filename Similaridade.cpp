// ===========================================================================
// ATIVIDADE 2 - Modulo Similaridade (versao inicial, com matrizes densas)
//
// Objetivo: dizer o quanto dois clientes sao parecidos, olhando o que cada um
// comprou.
//
// Ideia (secao 2 do enunciado):
//   Pi = conjunto de produtos que o cliente i comprou
//   s(i, j) = 1 - |Pi interseccao Pj| / |Pi|
//
//   |Pi interseccao Pj| = quantos produtos os dois compraram em comum
//   |Pi|                = quantos produtos o cliente i comprou
//
//   s = 0  -> o cliente j comprou TUDO que o i comprou (muito parecidos)
//   s = 1  -> nao tem nenhum produto em comum (nada parecidos)
//   Quanto MENOR o s, MAIS parecidos.
//
// Como calcular para todos os pares de uma vez (matrizes):
//   A  = matriz de compras      (linha = cliente, coluna = produto, 1 = comprou)
//   I  = A x (A transposta)     I[i][j] = produtos em comum entre i e j
//   S  = 1 - I[i][j] / |Pi|     matriz de similaridade
//
// As matrizes sao criadas com malloc (estilo C) e liberadas com free.
//
// Este arquivo NAO tem main: o main fica em arquivo separado (main.cpp).
// ===========================================================================

#include <stdio.h>
#include <stdlib.h>   // malloc, free, exit
#include "Similaridade.h"

// ---------------------------------------------------------------------------
// criarMatrizInt: cria uma matriz de inteiros (linhas x colunas) cheia de 0.
//
// Com malloc, uma matriz e feita em duas etapas:
//   1) um vetor com "linhas" ponteiros (cada ponteiro vai apontar para uma linha);
//   2) para cada linha, um vetor com "colunas" inteiros.
// Depois acessamos matriz[i][j] normalmente.
//
// malloc(n * sizeof(int)) pede ao sistema espaco para n inteiros.
// Se nao houver memoria suficiente, malloc devolve NULL: nesse caso avisamos
// e encerramos o programa.
// ---------------------------------------------------------------------------
int **criarMatrizInt(int linhas, int colunas) {
    int i, j;
    int **matriz;

    matriz = (int **) malloc(linhas * sizeof(int *));   // o vetor de ponteiros
    if (matriz == NULL) {
        printf("Memoria insuficiente para a matriz.\n");
        exit(1);
    }

    for (i = 0; i < linhas; i++) {
        matriz[i] = (int *) malloc(colunas * sizeof(int));   // a linha i
        if (matriz[i] == NULL) {
            printf("Memoria insuficiente para a matriz.\n");
            exit(1);
        }
        for (j = 0; j < colunas; j++)
            matriz[i][j] = 0;   // malloc nao zera: preenchemos nos mesmos
    }
    return matriz;
}

// ---------------------------------------------------------------------------
// criarMatrizDouble: igual a anterior, mas de numeros com virgula (double)
// e cheia de 1.0. Usada para a matriz de similaridade, onde 1 quer dizer
// "nada parecido" (e o valor padrao ate calcularmos o resto).
// ---------------------------------------------------------------------------
double **criarMatrizDouble(int linhas, int colunas) {
    int i, j;
    double **matriz;

    matriz = (double **) malloc(linhas * sizeof(double *));
    if (matriz == NULL) {
        printf("Memoria insuficiente para a matriz.\n");
        exit(1);
    }

    for (i = 0; i < linhas; i++) {
        matriz[i] = (double *) malloc(colunas * sizeof(double));
        if (matriz[i] == NULL) {
            printf("Memoria insuficiente para a matriz.\n");
            exit(1);
        }
        for (j = 0; j < colunas; j++)
            matriz[i][j] = 1.0;
    }
    return matriz;
}

// ---------------------------------------------------------------------------
// liberarMatrizInt / liberarMatrizDouble: devolvem a memoria ao sistema.
// Tudo que foi criado com malloc precisa de um free. Liberamos na ordem
// inversa: primeiro cada linha, depois o vetor de ponteiros.
// ---------------------------------------------------------------------------
void liberarMatrizInt(int **M, int linhas) {
    int i;

    for (i = 0; i < linhas; i++)
        free(M[i]);
    free(M);
}

void liberarMatrizDouble(double **M, int linhas) {
    int i;

    for (i = 0; i < linhas; i++)
        free(M[i]);
    free(M);
}

// ---------------------------------------------------------------------------
// criarMatrizCompras: monta a matriz A (n clientes x m produtos) e guarda em
// lista.matrizCompras.
//   A[i][p] = 1 se o cliente i comprou o produto p
//   A[i][p] = 0 se nao comprou
//
// Exemplo (Joao comprou B, C e F; Fulano comprou A e C):
//              A  B  C  D  E  F
//     Joao     0  1  1  0  0  1
//     Fulano   1  0  1  0  0  0
// ---------------------------------------------------------------------------
void criarMatrizCompras(ListaCompras &lista) {
    int n = lista.clientes.size();   // numero de clientes (linhas)
    int m = lista.produtos.size();   // numero de produtos (colunas)
    int i, j, idProduto;

    lista.matrizCompras = criarMatrizInt(n, m);   // comeca tudo com 0

    // Para cada cliente i, passa pelos produtos que ele comprou (posicao j do
    // vetor dele) e marca 1 na coluna daquele produto.
    for (i = 0; i < n; i++) {
        for (j = 0; j < (int)lista.prodClientes[i].size(); j++) {
            idProduto = lista.prodClientes[i][j];
            lista.matrizCompras[i][idProduto] = 1;
        }
    }
}

// ---------------------------------------------------------------------------
// liberarMatrizCompras: libera a matriz de compras (se ela foi criada).
// ---------------------------------------------------------------------------
void liberarMatrizCompras(ListaCompras &lista) {
    if (lista.matrizCompras != NULL) {
        liberarMatrizInt(lista.matrizCompras, lista.clientes.size());
        lista.matrizCompras = NULL;   // avisa que nao existe mais
    }
}

// ---------------------------------------------------------------------------
// calcularTransposta: troca linhas por colunas.
// A primeira linha de A vira a primeira COLUNA de T, e assim por diante.
//   T[j][i] = A[i][j]          (e o pseudocodigo do enunciado)
//
// Se A tem "linhas" x "colunas", T tem "colunas" x "linhas". Como a matriz
// criada com malloc nao sabe o proprio tamanho, passamos as dimensoes.
// ---------------------------------------------------------------------------
int **calcularTransposta(int **A, int linhas, int colunas) {
    int i, j;

    // T tem as dimensoes invertidas.
    int **T = criarMatrizInt(colunas, linhas);

    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++)
            T[j][i] = A[i][j];
    }
    return T;
}

// ---------------------------------------------------------------------------
// multiplicarMatrizes: multiplicacao de matrizes do jeito padrao (C = A x B).
// E o pseudocodigo do enunciado:
//
//   C[i][j] = soma de  A[i][k] * B[k][j]   para k de 0 ate n-1
//
// A tem m linhas e n colunas; B tem n linhas e p colunas;
// o resultado C tem m linhas e p colunas. Sao 3 lacos um dentro do outro:
// i escolhe a linha, j escolhe a coluna e k percorre a soma.
// ---------------------------------------------------------------------------
int **multiplicarMatrizes(int **A, int m, int n, int **B, int p) {
    int i, j, k, soma;

    int **C = criarMatrizInt(m, p);

    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            soma = 0;
            for (k = 0; k < n; k++)
                soma = soma + A[i][k] * B[k][j];
            C[i][j] = soma;
        }
    }
    return C;
}

// ---------------------------------------------------------------------------
// calcularSimilaridade: junta tudo e devolve a matriz de similaridade S.
//
//   1) A = matriz de compras (criada se ainda nao existe)
//   2) T = transposta de A
//   3) I = A x T   -> I[i][j] = quantos produtos i e j compraram em comum
//                     (e o produto interno da linha i com a linha j de A:
//                      so soma 1 onde OS DOIS compraram)
//   4) S[i][j] = 1 - I[i][j] / |Pi|
//
// Repare: dividimos por |Pi| (produtos do cliente i), e nao por um valor
// igual para os dois. Por isso S e ASSIMETRICA: s(i,j) pode ser diferente
// de s(j,i). Isso e desejado (explicado no enunciado): evita que quem
// compra MUITO pareca parecido com todo mundo.
//
// T e I sao so intermediarias: depois de usadas, liberamos. A matriz de
// compras continua guardada em lista.matrizCompras.
// ---------------------------------------------------------------------------
double **calcularSimilaridade(ListaCompras &lista) {
    int n = lista.clientes.size();   // numero de clientes
    int m = lista.produtos.size();   // numero de produtos
    int i, j, totalI;

    if (lista.matrizCompras == NULL)
        criarMatrizCompras(lista);

    int **A = lista.matrizCompras;
    int **T = calcularTransposta(A, n, m);          // m x n
    int **I = multiplicarMatrizes(A, n, m, T, n);   // n x n
    double **S = criarMatrizDouble(n, n);           // comeca tudo 1.0

    for (i = 0; i < n; i++) {
        totalI = lista.prodClientes[i].size();   // |Pi| = produtos do cliente i

        // Se o cliente nao comprou nada, nao da para dividir por 0.
        // Ele fica com tudo 1 (nao e parecido com ninguem).
        if (totalI > 0) {
            for (j = 0; j < n; j++) {
                // (double) transforma I[i][j] em numero com virgula antes de
                // dividir. Sem isso, 2 / 3 daria 0 (divisao de inteiros)
                // em vez de 0.666...
                S[i][j] = 1.0 - (double)I[i][j] / totalI;
            }
        }
    }

    liberarMatrizInt(T, m);   // T tem m linhas
    liberarMatrizInt(I, n);   // I tem n linhas
    return S;
}

// ---------------------------------------------------------------------------
// similaridadeDensa: devolve s(i, j), o valor da similaridade entre o cliente i
// e o cliente j. Os dois sao indices internos (nao os codigos originais).
// Quanto MENOR o valor, MAIS parecidos eles sao.
//
// Atencao: s(i, j) e diferente de s(j, i), porque S nao e simetrica.
// ---------------------------------------------------------------------------
double similaridadeDensa(double **S, int i, int j) {
    return S[i][j];
}
