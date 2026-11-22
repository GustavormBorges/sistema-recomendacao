#include <stdio.h>
#include <stdlib.h>
#include "Similaridade.h"

int **criarMatrizInt(int linhas, int colunas) {
    int **M = (int **) malloc(linhas * sizeof(int *));

    for (int i = 0; i < linhas; i++) {
        M[i] = (int *) malloc(colunas * sizeof(int));

        for (int j = 0; j < colunas; j++) {
            M[i][j] = 0;
        }
    }

    return M;
}

double **criarMatrizDouble(int linhas, int colunas) {
    double **M = (double **) malloc(linhas * sizeof(double *));

    for (int i = 0; i < linhas; i++) {
        M[i] = (double *) malloc(colunas * sizeof(double));

        for (int j = 0; j < colunas; j++) {
            M[i][j] = 1.0;
        }
    }

    return M;
}

void liberarMatrizInt(int **M, int linhas) {
    if (M == NULL) {
        return;
    }

    for (int i = 0; i < linhas; i++) {
        free(M[i]);
    }

    free(M);
}

void liberarMatrizDouble(double **M, int linhas) {
    if (M == NULL) {
        return;
    }

    for (int i = 0; i < linhas; i++) {
        free(M[i]);
    }

    free(M);
}

void criarMatrizCompras(ListaCompras &lista) {
    int n = lista.clientes.size();
    int m = lista.produtos.size();

    lista.matrizCompras = criarMatrizInt(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < lista.prodClientes[i].size(); j++) {
            int idProduto = lista.prodClientes[i][j];
            lista.matrizCompras[i][idProduto] = 1;
        }
    }
}

void liberarMatrizCompras(ListaCompras &lista) {
    if (lista.matrizCompras != NULL) {
        liberarMatrizInt(lista.matrizCompras, lista.clientes.size());
        lista.matrizCompras = NULL;
    }
}

int **calcularTransposta(int **A, int linhas, int colunas) {
    int **T = criarMatrizInt(colunas, linhas);

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            T[j][i] = A[i][j];
        }
    }

    return T;
}

int **multiplicarMatrizes(int **A, int m, int n, int **B, int p) {
    int **C = criarMatrizInt(m, p);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

// Calcula as intersecoes sem criar a matriz transposta.
int **calcularIntersecoesDireto(int **A, int n, int m) {
    int **I = criarMatrizInt(n, n);

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            for (int k = 0; k < m; k++) {
                I[i][j] += A[i][k] * A[j][k];
            }

            // A matriz de intersecoes e simetrica.
            I[j][i] = I[i][j];
        }
    }

    return I;
}

double **calcularSimilaridade(ListaCompras &lista, int algoritmo) {
    int n = lista.clientes.size();
    int m = lista.produtos.size();

    if (lista.matrizCompras == NULL) {
        criarMatrizCompras(lista);
    }

    int **A = lista.matrizCompras;
    int **I = NULL;

    if (algoritmo == 1) {
        // Algoritmo padrao: cria a transposta e multiplica.
        int **T = calcularTransposta(A, n, m);

        I = multiplicarMatrizes(A, n, m, T, n);

        liberarMatrizInt(T, m);
    }
    else if (algoritmo == 2) {
        // Algoritmo otimizado: calcula diretamente as intersecoes.
        I = calcularIntersecoesDireto(A, n, m);
    }
    else {
        printf("Algoritmo invalido. Escolha 1 ou 2.\n");
        return NULL;
    }

    double **S = criarMatrizDouble(n, n);

    for (int i = 0; i < n; i++) {
        int totalI = lista.prodClientes[i].size();

        for (int j = 0; j < n; j++) {
            if (totalI > 0) {
                S[i][j] = 1.0 - (double) I[i][j] / totalI;
            }
        }
    }

    liberarMatrizInt(I, n);

    return S;
}

double similaridadeDensa(double **S, int i, int j) {
    return S[i][j];
}

