#include "Recomendacao.h"
#include <algorithm>

// Função de comparação para o sort: true se 'a' deve vir antes de 'b'.
// Ordem crescente do valor. Se empatar, o produto de menor índice vem primeiro
// (assim o resultado é sempre o mesmo).
static bool compararRanking(const ItemRanking &a, const ItemRanking &b) {
    if (a.valor != b.valor) {
        return a.valor < b.valor;
    }
    return a.produto < b.produto;
}

vector<ItemRanking> recomendar(const ListaCompras &lc, const MatrizDouble &S,
                               int cliente, int k) {
    int n = lc.clientes.size();
    int m = lc.produtos.size();

    // Marca os produtos que o cliente já comprou (não faz sentido recomendá-los).
    vector<bool> jaComprou(m, false);
    for (int p : lc.compras[cliente]) {
        jaComprou[p] = true;
    }

    // Passo 2: vetor de ranqueamento com valor inicial 1 para todos os produtos.
    vector<ItemRanking> R(m);
    for (int p = 0; p < m; p++) {
        R[p].produto = p;
        R[p].valor = 1.0;
    }

    // Passos 1 e 3: para cada vizinho v (similaridade < 1, que não seja o próprio cliente),
    // cada produto que v comprou e o cliente não comprou tem o valor multiplicado por S[cliente][v].
    // Quanto mais vizinhos parecidos compraram o produto, menor ele fica.
    for (int v = 0; v < n; v++) {
        if (v == cliente || S[cliente][v] >= 1.0) {
            continue;
        }
        for (int p : lc.compras[v]) {
            if (!jaComprou[p]) {
                R[p].valor = R[p].valor * S[cliente][v];
            }
        }
    }

    // Passo 4: ordena do menor para o maior valor...
    sort(R.begin(), R.end(), compararRanking);

    // ...e pega os k primeiros. Quando chegar em valor 1, o resto nunca foi alterado
    // (ou já foi comprado pelo cliente), então para.
    vector<ItemRanking> resultado;
    for (int i = 0; i < m && (int)resultado.size() < k; i++) {
        if (R[i].valor >= 1.0) {
            break;
        }
        resultado.push_back(R[i]);
    }
    return resultado;
}
