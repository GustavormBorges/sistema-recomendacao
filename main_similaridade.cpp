// ===========================================================================
// MAIN da ATIVIDADE 2 (programa testador do modulo Similaridade)
//
// Para um cliente (identificado pelo INDICE interno, 0, 1, 2...), encontra e
// mostra o cliente mais parecido com ele, que nao seja ele mesmo.
//
// Como usar (no terminal):
//     similaridade <arquivo.csv> [indice_cliente]
//
// Se o indice nao for informado, usa o cliente 0.
// ===========================================================================

#include <iostream>
#include <stdlib.h>   // atoi
#include "ListaCompras.h"
#include "Similaridade.h"

int main(int argc, char *argv[]) {
    ListaCompras lista;
    int n, i, j, maisSimilar;

    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <arquivo.csv> [indice_cliente]" << endl;
        return 1;
    }

    if (lerDados(argv[1], lista) == 0) {
        cout << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }

    n = lista.clientes.size();

    // Cliente escolhido: o indice digitado, ou 0 se nao digitou nada.
    // atoi transforma o texto "153" no numero 153.
    i = 0;
    if (argc >= 3)
        i = atoi(argv[2]);

    if (i < 0 || i >= n) {
        cout << "Indice invalido. Use um valor de 0 a " << n - 1 << endl;
        return 1;
    }

    // Calcula a matriz de similaridade de todos os clientes.
    vector<vector<double> > S = calcularSimilaridade(lista);

    // Procura o cliente j com MENOR S[i][j] (o mais parecido com i).
    // maisSimilar guarda o melhor encontrado ate agora; -1 = ainda nao achou.
    maisSimilar = -1;
    for (j = 0; j < n; j++) {
        // Ignora o proprio cliente e quem nao comprou nada em comum (S = 1).
        if (j != i && S[i][j] < 1.0) {
            if (maisSimilar == -1 || S[i][j] < S[i][maisSimilar])
                maisSimilar = j;
        }
    }

    imprimirCompras(lista, i);
    cout << endl;

    if (maisSimilar == -1) {
        cout << "Nenhum outro cliente comprou o mesmo que o cliente " << i << "." << endl;
    } else {
        cout << "Cliente mais similar (s = " << S[i][maisSimilar] << "):" << endl;
        imprimirCompras(lista, maisSimilar);
    }

    return 0;
}
