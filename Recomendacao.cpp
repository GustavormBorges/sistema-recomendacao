// ===========================================================================
// ATIVIDADE 3 - Modulo Recomendacao
//
// Objetivo: dado um cliente, devolver os k produtos que mais combinam com ele.
//
// Ideia (secao 2.3 do enunciado):
//   - Os "vizinhos" do cliente sao os clientes parecidos com ele (S < 1).
//   - Cada produto comeca com nota 1.
//   - Cada vizinho que comprou o produto MULTIPLICA a nota por S (que e menor
//     que 1), entao a nota DIMINUI. Quanto mais vizinhos, e mais parecidos,
//     menor fica a nota.
//   - Nota MENOR = produto MELHOR. Ordenamos do menor para o maior e pegamos
//     os k primeiros.
//
// Este arquivo NAO tem main: o main fica em arquivo separado (main_recomendacao.cpp).
// ===========================================================================

#include <algorithm>   // sort
#include "Recomendacao.h"

// ---------------------------------------------------------------------------
// compararRanking: funcao que ensina o sort a ordenar os ItemRanking.
// E igual a "ordenarPorPreco" do exemplo do professor: devolve true se o
// item "a" deve vir ANTES do item "b".
//
// Queremos ordem crescente do valor (menor primeiro).
// Se os valores forem iguais, o de menor indice de produto vem primeiro.
// Isso so serve para o resultado ser sempre o mesmo (sem esse desempate, o
// sort poderia embaralhar produtos com a mesma nota).
// ---------------------------------------------------------------------------
static bool compararRanking(const ItemRanking &a, const ItemRanking &b) {
    if (a.valor != b.valor)
        return a.valor < b.valor;

    return a.produto < b.produto;
}

// ---------------------------------------------------------------------------
// recomendar: devolve os (ate) k melhores produtos para o cliente.
//
// Parametros:
//   lista   - o historico de compras (modulo ListaCompras)
//   S       - a matriz de similaridade (modulo Similaridade)
//   cliente - indice do cliente que vai receber a recomendacao
//   k       - quantos produtos recomendar
// ---------------------------------------------------------------------------
vector<ItemRanking> recomendar(ListaCompras &lista, vector<vector<double> > &S,
                               int cliente, int k) {
    int n = lista.clientes.size();   // numero de clientes
    int m = lista.produtos.size();   // numero de produtos
    int v, j, p, i, total, idProduto, escolhidos;
    vector<int> jaComprou;           // jaComprou[p] = 1 se o cliente ja comprou o produto p
    vector<ItemRanking> R;           // o vetor de ranqueamento (uma nota por produto)
    vector<ItemRanking> resultado;   // os k escolhidos, que a funcao devolve

    // Primeiro marcamos os produtos que o cliente JA comprou, porque nao faz
    // sentido recomendar o que ele ja tem. Comeca tudo 0 (nao comprou)...
    for (p = 0; p < m; p++)
        jaComprou.push_back(0);

    // ...e depois coloca 1 nos produtos da lista de compras dele.
    total = lista.prodClientes[cliente].size();
    for (j = 0; j < total; j++) {
        idProduto = lista.prodClientes[cliente][j];
        jaComprou[idProduto] = 1;
    }

    // PASSO 2 do enunciado: cria o vetor R com nota inicial 1 para todos os
    // produtos. Cada posicao guarda qual e o produto e qual a nota dele.
    // (Antes de ordenar, R[p] e o item do produto p.)
    for (p = 0; p < m; p++) {
        ItemRanking item;
        item.produto = p;
        item.valor = 1.0;
        R.push_back(item);
    }

    // PASSOS 1 e 3 do enunciado:
    // Passa por todos os clientes v. Ele e vizinho se:
    //   - nao e o proprio cliente, e
    //   - S[cliente][v] < 1 (tem pelo menos um produto em comum)
    for (v = 0; v < n; v++) {
        if (v != cliente && S[cliente][v] < 1.0) {

            // Para cada produto que o vizinho v comprou...
            total = lista.prodClientes[v].size();
            for (j = 0; j < total; j++) {
                idProduto = lista.prodClientes[v][j];

                // ...e que o cliente ainda NAO comprou, multiplica a nota por S.
                if (jaComprou[idProduto] == 0)
                    R[idProduto].valor = R[idProduto].valor * S[cliente][v];
            }
        }
    }

    // PASSO 4 do enunciado: ordena R do menor para o maior valor, usando a
    // funcao de comparacao de cima. sort(inicio, fim, funcao).
    sort(R.begin(), R.end(), compararRanking);

    // Pega os k primeiros. Paramos tambem quando a nota chega em 1: esses
    // produtos nunca foram alterados (nenhum vizinho comprou) ou ja eram do
    // cliente. Como R esta ordenado, depois do primeiro 1 so tem 1 (ou mais).
    i = 0;
    escolhidos = 0;
    while (i < m && escolhidos < k && R[i].valor < 1.0) {
        resultado.push_back(R[i]);
        escolhidos++;
        i++;
    }

    return resultado;
}
