#include "Similaridade.h"

MatrizInt criarMatrizCompras(const ListaCompras &lc) {
    int n = lc.clientes.size();
    int m = lc.produtos.size();

    MatrizInt A(n, vector<int>(m, 0));  // começa tudo zerado
    for (int i = 0; i < n; i++) {
        for (int p : lc.compras[i]) {
            A[i][p] = 1;
        }
    }
    return A;
}

MatrizInt transposta(const MatrizInt &A) {
    if (A.empty()) {
        return MatrizInt();
    }
    int linhas = A.size();
    int colunas = A[0].size();

    MatrizInt T(colunas, vector<int>(linhas, 0));
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            T[j][i] = A[i][j];
        }
    }
    return T;
}

MatrizInt multiplicar(const MatrizInt &A, const MatrizInt &B) {
    if (A.empty() || B.empty()) {
        return MatrizInt();
    }
    int m = A.size();     // linhas de A
    int n = B.size();     // colunas de A = linhas de B
    int p = B[0].size();  // colunas de B

    MatrizInt C(m, vector<int>(p, 0));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            int soma = 0;
            for (int k = 0; k < n; k++) {
                soma = soma + A[i][k] * B[k][j];
            }
            C[i][j] = soma;
        }
    }
    return C;
}

MatrizDouble calcularSimilaridade(const ListaCompras &lc) {
    int n = lc.clientes.size();

    MatrizInt A = criarMatrizCompras(lc);
    MatrizInt T = transposta(A);
    MatrizInt I = multiplicar(A, T);  // I[i][j] = quantos produtos i e j compraram em comum

    MatrizDouble S(n, vector<double>(n, 1.0));
    for (int i = 0; i < n; i++) {
        int totalI = lc.compras[i].size();  // |Pi|
        if (totalI == 0) {
            continue;  // cliente sem compras: fica tudo 1 (não é parecido com ninguém)
        }
        for (int j = 0; j < n; j++) {
            S[i][j] = 1.0 - (double)I[i][j] / totalI;
        }
    }
    return S;
}
