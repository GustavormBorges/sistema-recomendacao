// Testador da Atividade 1 (ListaCompras).
// Mostra os produtos comprados por clientes, a partir dos códigos originais.
//
// Uso: teste_lista <arquivo.csv> [cod_cliente1 cod_cliente2 ...]
// Sem códigos, usa os 3 primeiros clientes do arquivo.

#include <iostream>
#include "ListaCompras.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <arquivo.csv> [cod_cliente ...]" << endl;
        return 1;
    }

    ListaCompras lc;
    if (!carregarCompras(argv[1], lc)) {
        cout << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }
    cout << lc.clientes.size() << " clientes e " << lc.produtos.size()
         << " produtos carregados." << endl << endl;

    if (argc == 2) {
        for (int i = 0; i < 3 && i < (int)lc.clientes.size(); i++) {
            imprimirCompras(lc, i);
        }
    } else {
        for (int a = 2; a < argc; a++) {
            int i = buscarCliente(lc, argv[a]);
            if (i == -1) {
                cout << "Cliente " << argv[a] << " nao encontrado." << endl;
            } else {
                imprimirCompras(lc, i);
            }
        }
    }
    return 0;
}
