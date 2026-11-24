#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
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

    if (lista.matrizCompras != NULL) {
        return;
    }

    lista.matrizCompras = criarMatrizInt(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (int)lista.prodClientes[i].size(); j++) {
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

void criarMatrizCSR(ListaCompras &lista) {
    int n = lista.clientes.size();
    int i, j;

    lista.matrizCSR.values.clear();
    lista.matrizCSR.col_index.clear();
    lista.matrizCSR.row_ptr.clear();

    lista.matrizCSR.row_ptr.push_back(0);

    for (i = 0; i < n; i++) {
        vector<int> produtosLinha = lista.prodClientes[i];

        sort(produtosLinha.begin(), produtosLinha.end());

        for (j = 0; j < (int)produtosLinha.size(); j++) {
            lista.matrizCSR.values.push_back(1);
            lista.matrizCSR.col_index.push_back(produtosLinha[j]);
        }

        lista.matrizCSR.row_ptr.push_back(lista.matrizCSR.values.size());
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

int **calcularIntersecoesDireto(int **A, int n, int m) {
    int **I = criarMatrizInt(n, n);

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            for (int k = 0; k < m; k++) {
                I[i][j] += A[i][k] * A[j][k];
            }

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
        int **T = calcularTransposta(A, n, m);

        I = multiplicarMatrizes(A, n, m, T, n);

        liberarMatrizInt(T, m);
    }
    else if (algoritmo == 2) {
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
            S[i][j] = 1.0 - (double) I[i][j] / totalI;
        }
    }

    liberarMatrizInt(I, n);

    return S;
}

void calcularSimilaridadeCSR(ListaCompras &lista) {
    int n = lista.clientes.size();
    int i, j;
    int inicioI, fimI, inicioJ, fimJ;
    int p, q;
    int intersecao;
    int totalI;
    double valor;

    if ((int)lista.matrizCSR.row_ptr.size() != n + 1) {
        criarMatrizCSR(lista);
    }

    lista.matrizIntersecoes.values.clear();
    lista.matrizIntersecoes.col_index.clear();
    lista.matrizIntersecoes.row_ptr.clear();

    lista.matrizSimilaridadeCSR.values.clear();
    lista.matrizSimilaridadeCSR.col_index.clear();
    lista.matrizSimilaridadeCSR.row_ptr.clear();

    lista.matrizIntersecoes.row_ptr.push_back(0);
    lista.matrizSimilaridadeCSR.row_ptr.push_back(0);

    for (i = 0; i < n; i++) {
        inicioI = lista.matrizCSR.row_ptr[i];
        fimI = lista.matrizCSR.row_ptr[i + 1];
        totalI = fimI - inicioI;

        for (j = 0; j < n; j++) {
            inicioJ = lista.matrizCSR.row_ptr[j];
            fimJ = lista.matrizCSR.row_ptr[j + 1];

            p = inicioI;
            q = inicioJ;
            intersecao = 0;

            while (p < fimI && q < fimJ) {
                if (lista.matrizCSR.col_index[p] == lista.matrizCSR.col_index[q]) {
                    intersecao += lista.matrizCSR.values[p] * lista.matrizCSR.values[q];
                    p++;
                    q++;
                }
                else if (lista.matrizCSR.col_index[p] < lista.matrizCSR.col_index[q]) {
                    p++;
                }
                else {
                    q++;
                }
            }

            if (intersecao > 0) {
                lista.matrizIntersecoes.values.push_back(intersecao);
                lista.matrizIntersecoes.col_index.push_back(j);
            }

            valor = 1.0 - (double)intersecao / totalI;

            if (valor != 0.0) {
                lista.matrizSimilaridadeCSR.values.push_back(valor);
                lista.matrizSimilaridadeCSR.col_index.push_back(j);
            }
        }

        lista.matrizIntersecoes.row_ptr.push_back(lista.matrizIntersecoes.values.size());
        lista.matrizSimilaridadeCSR.row_ptr.push_back(lista.matrizSimilaridadeCSR.values.size());
    }
}

double similaridadeCSR(ListaCompras &lista, int i, int j) {
    int p;
    int inicio = lista.matrizSimilaridadeCSR.row_ptr[i];
    int fim = lista.matrizSimilaridadeCSR.row_ptr[i + 1];

    for (p = inicio; p < fim; p++) {
        if (lista.matrizSimilaridadeCSR.col_index[p] == j) {
            return lista.matrizSimilaridadeCSR.values[p];
        }
    }

    return 0.0;
}

double similaridadeDensa(double **S, int i, int j) {
    return S[i][j];
}

