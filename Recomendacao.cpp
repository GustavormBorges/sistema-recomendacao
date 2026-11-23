#include <algorithm>
#include "Recomendacao.h"

static bool compararItens(const item &a, const item &b) {
    if (a.score != b.score)
        return a.score < b.score;

    return a.produto < b.produto;
}

vector<item> recomendar(ListaCompras &lista, int cliente, int k) {
    int n = lista.clientes.size();
    int m = lista.produtos.size();
    int v, p, i, j, idProduto, escolhidos;
    double s;
    vector<int> jaComprou;
    vector<item> R;
    vector<item> resultado;

    if ((int)lista.matrizSimilaridadeCSR.row_ptr.size() != n + 1) {
        calcularSimilaridadeCSR(lista);
    }

    for (p = 0; p < m; p++)
        jaComprou.push_back(0);

    for (j = 0; j < (int)lista.prodClientes[cliente].size(); j++) {
        idProduto = lista.prodClientes[cliente][j];
        jaComprou[idProduto] = 1;
    }

    for (p = 0; p < m; p++) {
        item novo;
        novo.produto = p;
        novo.score = 1.0;
        R.push_back(novo);
    }

    for (v = 0; v < n; v++) {
        s = similaridadeCSR(lista, cliente, v);

        if (v != cliente && s < 1.0) {
            for (j = 0; j < (int)lista.prodClientes[v].size(); j++) {
                idProduto = lista.prodClientes[v][j];

                if (jaComprou[idProduto] == 0)
                    R[idProduto].score = R[idProduto].score * s;
            }
        }
    }

    sort(R.begin(), R.end(), compararItens);

    i = 0;
    escolhidos = 0;

    while (i < m && escolhidos < k && R[i].score < 1.0) {
        resultado.push_back(R[i]);
        escolhidos++;
        i++;
    }

    return resultado;
}

