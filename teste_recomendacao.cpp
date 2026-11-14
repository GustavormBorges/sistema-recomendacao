// Testador da Atividade 3 (Recomendacao).
// Pede a recomendação de k produtos para clientes, a partir dos códigos originais.
//
// Uso: teste_recomendacao <arquivo.csv> <k> [cod_cliente1 cod_cliente2 cod_cliente3 ...]
// Sem códigos, usa os 3 primeiros clientes do arquivo.

#include <iostream>
#include <cstdlib>
#include "ListaCompras.h"
#include "Similaridade.h"
#include "Recomendacao.h"

// Mostra as compras do cliente e os k produtos recomendados para ele.
static void mostrarRecomendacao(const ListaCompras &lc, const MatrizDouble &S, int i, int k) {
    imprimirCompras(lc, i);

    vector<ItemRanking> recomendados = recomendar(lc, S, i, k);
    if (recomendados.empty()) {
        cout << "Nenhum produto para recomendar (nenhum cliente parecido)." << endl;
    } else {
        cout << "Produtos recomendados:" << endl;
        for (size_t r = 0; r < recomendados.size(); r++) {
            cout << "   " << r + 1 << ". " << lc.produtos[recomendados[r].produto]
                 << "  (valor " << recomendados[r].valor << ")" << endl;
        }
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Uso: " << argv[0] << " <arquivo.csv> <k> [cod_cliente ...]" << endl;
        return 1;
    }

    ListaCompras lc;
    if (!carregarCompras(argv[1], lc)) {
        cout << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }
    int k = atoi(argv[2]);

    MatrizDouble S = calcularSimilaridade(lc);

    if (argc == 3) {
        for (int i = 0; i < 3 && i < (int)lc.clientes.size(); i++) {
            mostrarRecomendacao(lc, S, i, k);
        }
    } else {
        for (int a = 3; a < argc; a++) {
            int i = buscarCliente(lc, argv[a]);
            if (i == -1) {
                cout << "Cliente " << argv[a] << " nao encontrado." << endl << endl;
            } else {
                mostrarRecomendacao(lc, S, i, k);
            }
        }
    }
    return 0;
}
