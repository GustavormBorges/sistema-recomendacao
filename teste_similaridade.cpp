// Testador da Atividade 2 (Similaridade).
// Para um cliente (índice interno), mostra o cliente mais similar a ele.
//
// Uso: teste_similaridade <arquivo.csv> [indice_cliente]
// Sem índice, usa o cliente 0.

#include <iostream>
#include <cstdlib>
#include "ListaCompras.h"
#include "Similaridade.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <arquivo.csv> [indice_cliente]" << endl;
        return 1;
    }

    ListaCompras lc;
    if (!carregarCompras(argv[1], lc)) {
        cout << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }

    int n = lc.clientes.size();
    int i = 0;
    if (argc >= 3) {
        i = atoi(argv[2]);
    }
    if (i < 0 || i >= n) {
        cout << "Indice invalido. Use um valor de 0 a " << n - 1 << endl;
        return 1;
    }

    MatrizDouble S = calcularSimilaridade(lc);

    // Procura o j com menor S[i][j], sem contar o próprio cliente.
    // Só vale se S < 1 (ou seja, se têm pelo menos um produto em comum).
    int maisSimilar = -1;
    for (int j = 0; j < n; j++) {
        if (j == i || S[i][j] >= 1.0) {
            continue;
        }
        if (maisSimilar == -1 || S[i][j] < S[i][maisSimilar]) {
            maisSimilar = j;
        }
    }

    imprimirCompras(lc, i);
    cout << endl;
    if (maisSimilar == -1) {
        cout << "Nenhum outro cliente comprou o mesmo que o cliente " << i << "." << endl;
    } else {
        cout << "Cliente mais similar (s = " << S[i][maisSimilar] << "):" << endl;
        imprimirCompras(lc, maisSimilar);
    }
    return 0;
}
