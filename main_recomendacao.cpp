// ===========================================================================
// MAIN da ATIVIDADE 3 (programa testador do modulo Recomendacao)
//
// Pede a recomendacao de k produtos para alguns clientes, usando o CODIGO
// ORIGINAL do cliente, e mostra os nomes dos produtos recomendados.
//
// Como usar (no terminal):
//     recomendacao <arquivo.csv> <k> [cod_cliente1 cod_cliente2 ...]
//
// Se nenhum codigo for informado, usa os 3 primeiros clientes do arquivo.
// ===========================================================================

#include <iostream>
#include <stdlib.h>   // atoi
#include "ListaCompras.h"
#include "Similaridade.h"
#include "Recomendacao.h"

// Mostra as compras do cliente i e depois os k produtos recomendados para ele.
static void mostrarRecomendacao(ListaCompras &lista, vector<vector<double> > &S, int i, int k) {
    int r;
    vector<ItemRanking> recomendados;

    imprimirCompras(lista, i);

    // Chama o modulo Recomendacao.
    recomendados = recomendar(lista, S, i, k);

    if (recomendados.size() == 0) {
        cout << "Nenhum produto para recomendar (nenhum cliente parecido)." << endl;
    } else {
        cout << "Produtos recomendados:" << endl;
        for (r = 0; r < (int)recomendados.size(); r++) {
            // recomendados[r].produto e o indice do produto: pegamos o nome
            // dele no vetor de produtos. "valor" e a nota (menor = melhor).
            cout << "   " << r + 1 << ". " << lista.produtos[recomendados[r].produto]
                 << "  (valor " << recomendados[r].valor << ")" << endl;
        }
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    ListaCompras lista;
    int k, i, a;

    // Precisa do arquivo e do k.
    if (argc < 3) {
        cout << "Uso: " << argv[0] << " <arquivo.csv> <k> [cod_cliente ...]" << endl;
        return 1;
    }

    if (lerDados(argv[1], lista) == 0) {
        cout << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }

    k = atoi(argv[2]);   // texto "5" -> numero 5

    // A matriz de similaridade e calculada UMA vez e usada para todos os clientes.
    vector<vector<double> > S = calcularSimilaridade(lista);

    if (argc == 3) {
        // Sem codigos: usa os 3 primeiros clientes.
        for (i = 0; i < 3 && i < (int)lista.clientes.size(); i++)
            mostrarRecomendacao(lista, S, i, k);
    } else {
        // Com codigos: descobre o indice de cada um e mostra a recomendacao.
        for (a = 3; a < argc; a++) {
            i = buscarCliente(lista, argv[a]);

            if (i == -1)
                cout << "Cliente " << argv[a] << " nao encontrado." << endl << endl;
            else
                mostrarRecomendacao(lista, S, i, k);
        }
    }

    return 0;
}
