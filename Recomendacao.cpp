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
// Este arquivo NAO tem main: o main fica em arquivo separado (main.cpp).
// ===========================================================================

#include <algorithm>   // sort
#include "Recomendacao.h"

// ---------------------------------------------------------------------------
// compararItens: funcao que ensina o sort a ordenar os item.
// E igual a "ordenarPorPreco" do exemplo do professor: devolve true se o
// item "a" deve vir ANTES do item "b".
//
// Queremos ordem crescente do score (menor primeiro).
// Se os valores forem iguais, o de menor indice de produto vem primeiro.
// Isso so serve para o resultado ser sempre o mesmo (sem esse desempate, o
// sort poderia embaralhar produtos com a mesma nota).
// ---------------------------------------------------------------------------
static bool compararItens(const item &a, const item &b) {
    if (a.score != b.score)
        return a.score < b.score;

    return a.produto < b.produto;
}

// ---------------------------------------------------------------------------
// recomendar: devolve os (ate) k melhores produtos para o cliente.
//
// Parametros:
//   lista   - o historico de compras (modulo ListaCompras)
//   S       - a matriz de similaridade (modulo Similaridade), criada com malloc
//   cliente - indice do cliente que vai receber a recomendacao
//   k       - quantos produtos recomendar
// ---------------------------------------------------------------------------
vector<item> recomendar(ListaCompras &lista, double **S, int cliente, int k) {
    int n = lista.clientes.size();   // numero de clientes
    int m = lista.produtos.size();   // numero de produtos
    int v, p, i, j, idProduto, escolhidos;
    double s;                        // similaridade entre o cliente e o vizinho v
    vector<int> jaComprou;           // jaComprou[p] = 1 se o cliente ja comprou o produto p
    vector<item> R;                  // o vetor de ranqueamento (uma nota por produto)
    vector<item> resultado;          // os k escolhidos, que a funcao devolve

    // Primeiro marcamos os produtos que o cliente JA comprou, porque nao faz
    // sentido recomendar o que ele ja tem. Comeca tudo 0 (nao comprou)...
    for (p = 0; p < m; p++)
        jaComprou.push_back(0);

    // ...e depois coloca 1 nos produtos da lista de compras dele.
    for (j = 0; j < (int)lista.prodClientes[cliente].size(); j++) {
        idProduto = lista.prodClientes[cliente][j];
        jaComprou[idProduto] = 1;
    }

    // PASSO 2 do enunciado: cria o vetor R com nota inicial 1 para todos os
    // produtos. Cada posicao guarda qual e o produto e qual a nota dele.
    // (Antes de ordenar, R[p] e o item do produto p.)
    for (p = 0; p < m; p++) {
        item novo;
        novo.produto = p;
        novo.score = 1.0;
        R.push_back(novo);
    }

    // PASSOS 1 e 3 do enunciado:
    // Passa por todos os clientes v. Ele e vizinho se:
    //   - nao e o proprio cliente, e
    //   - similaridade(cliente, v) < 1 (tem pelo menos um produto em comum)
    for (v = 0; v < n; v++) {
        s = similaridadeDensa(S, cliente, v);

        if (v != cliente && s < 1.0) {

            // Para cada produto da lista do vizinho v...
            for (j = 0; j < (int)lista.prodClientes[v].size(); j++) {
                idProduto = lista.prodClientes[v][j];

                // ...que o cliente ainda NAO comprou, multiplica a nota por s.
                if (jaComprou[idProduto] == 0)
                    R[idProduto].score = R[idProduto].score * s;
            }
        }
    }

    // PASSO 4 do enunciado: ordena R do menor para o maior valor, usando a
    // funcao de comparacao de cima. sort(inicio, fim, funcao).
    sort(R.begin(), R.end(), compararItens);

    // Pega os k primeiros. Paramos tambem quando a nota chega em 1: esses
    // produtos nunca foram alterados (nenhum vizinho comprou) ou ja eram do
    // cliente. Como R esta ordenado, depois do primeiro 1 so tem 1 (ou mais).
    i = 0;
    escolhidos = 0;
    while (i < m && escolhidos < k && R[i].score < 1.0) {
        resultado.push_back(R[i]);
        escolhidos++;
        i++;
    }

    return resultado;
}
