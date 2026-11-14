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
// Este arquivo NAO tem main: o main fica em arquivo separado (main_similaridade.cpp).
// ===========================================================================

#include "Similaridade.h"

// ---------------------------------------------------------------------------
// criarMatrizInt: cria uma matriz de inteiros (linhas x colunas) cheia de 0.
//
// Uma matriz aqui e um "vetor de vetores": cada linha e um vector<int>, e a
// matriz e um vector dessas linhas. Montamos com push_back, o mesmo que o
// professor mostra nos exemplos de vector. Depois acessamos matriz[i][j].
// ---------------------------------------------------------------------------
static vector<vector<int> > criarMatrizInt(int linhas, int colunas) {
    int i, j;
    vector<vector<int> > matriz;

    for (i = 0; i < linhas; i++) {
        vector<int> linha;              // uma linha nova, vazia
        for (j = 0; j < colunas; j++)
            linha.push_back(0);         // coloca "colunas" zeros nela
        matriz.push_back(linha);        // coloca a linha pronta na matriz
    }
    return matriz;
}

// ---------------------------------------------------------------------------
// criarMatrizDouble: igual a anterior, mas de numeros com virgula (double)
// e cheia de 1.0. Usada para a matriz de similaridade, onde 1 quer dizer
// "nada parecido" (e o valor padrao ate calcularmos o resto).
// ---------------------------------------------------------------------------
static vector<vector<double> > criarMatrizDouble(int linhas, int colunas) {
    int i, j;
    vector<vector<double> > matriz;

    for (i = 0; i < linhas; i++) {
        vector<double> linha;
        for (j = 0; j < colunas; j++)
            linha.push_back(1.0);
        matriz.push_back(linha);
    }
    return matriz;
}

// ---------------------------------------------------------------------------
// criarMatrizCompras: monta a matriz A (n clientes x m produtos).
//   A[i][p] = 1 se o cliente i comprou o produto p
//   A[i][p] = 0 se nao comprou
//
// Exemplo (Joao comprou B, C e F; Fulano comprou A e C):
//              A  B  C  D  E  F
//     Joao     0  1  1  0  0  1
//     Fulano   1  0  1  0  0  0
// ---------------------------------------------------------------------------
vector<vector<int> > criarMatrizCompras(ListaCompras &lista) {
    int n = lista.clientes.size();   // numero de clientes (linhas)
    int m = lista.produtos.size();   // numero de produtos (colunas)
    int i, j, total, idProduto;
    vector<vector<int> > A = criarMatrizInt(n, m);   // comeca tudo com 0

    // Para cada cliente i, passa pelos produtos que ele comprou (posicao j da
    // lista dele) e marca 1 na coluna daquele produto.
    for (i = 0; i < n; i++) {
        total = lista.prodClientes[i].size();
        for (j = 0; j < total; j++) {
            idProduto = lista.prodClientes[i][j];
            A[i][idProduto] = 1;
        }
    }
    return A;
}

// ---------------------------------------------------------------------------
// calcularTransposta: troca linhas por colunas.
// A primeira linha de A vira a primeira COLUNA de T, e assim por diante.
//   T[j][i] = A[i][j]          (e o pseudocodigo do enunciado)
//
// Se A tem "linhas" x "colunas", T tem "colunas" x "linhas".
// ---------------------------------------------------------------------------
vector<vector<int> > calcularTransposta(vector<vector<int> > &A) {
    int linhas = A.size();
    int colunas = 0;
    int i, j;

    // Se A nao esta vazia, o numero de colunas e o tamanho da primeira linha.
    if (linhas > 0)
        colunas = A[0].size();

    // T tem as dimensoes invertidas.
    vector<vector<int> > T = criarMatrizInt(colunas, linhas);

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
vector<vector<int> > multiplicarMatrizes(vector<vector<int> > &A, vector<vector<int> > &B) {
    int m = A.size();   // linhas de A
    int n = B.size();   // colunas de A = linhas de B
    int p = 0;          // colunas de B
    int i, j, k, soma;

    if (n > 0)
        p = B[0].size();

    vector<vector<int> > C = criarMatrizInt(m, p);

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
//   1) A = matriz de compras
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
// ---------------------------------------------------------------------------
vector<vector<double> > calcularSimilaridade(ListaCompras &lista) {
    int n = lista.clientes.size();
    int i, j, totalI;

    vector<vector<int> > A = criarMatrizCompras(lista);
    vector<vector<int> > T = calcularTransposta(A);
    vector<vector<int> > I = multiplicarMatrizes(A, T);
    vector<vector<double> > S = criarMatrizDouble(n, n);   // comeca tudo 1.0

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
    return S;
}
