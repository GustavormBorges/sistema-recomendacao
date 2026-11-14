// ===========================================================================
// MAIN da ATIVIDADE 1 (programa testador do modulo ListaCompras)
//
// Mostra os produtos comprados por alguns clientes, usando o CODIGO ORIGINAL
// do cliente (o que aparece no CSV).
//
// Como usar (no terminal):
//     lista <arquivo.csv> [cod_cliente1 cod_cliente2 ...]
//
// Se nenhum codigo for informado, usa os 3 primeiros clientes do arquivo.
//
// argc = quantos itens foram digitados no terminal (contando o nome do programa)
// argv = os itens digitados: argv[0] e o programa, argv[1] e o arquivo, etc.
// ===========================================================================

#include <iostream>
#include "ListaCompras.h"

int main(int argc, char *argv[]) {
    ListaCompras lista;
    int i, a;

    // Precisa de pelo menos o nome do arquivo.
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <arquivo.csv> [cod_cliente ...]" << endl;
        return 1;
    }

    // Le o arquivo e monta as estruturas. Devolve 0 se nao conseguiu abrir.
    if (lerDados(argv[1], lista) == 0) {
        cout << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }

    cout << lista.clientes.size() << " clientes e " << lista.produtos.size()
         << " produtos carregados." << endl << endl;

    if (argc == 2) {
        // Sem codigos: mostra os 3 primeiros clientes (indices 0, 1 e 2).
        for (i = 0; i < 3 && i < (int)lista.clientes.size(); i++)
            imprimirCompras(lista, i);
    } else {
        // Com codigos: para cada um, descobre o indice e mostra as compras.
        for (a = 2; a < argc; a++) {
            i = buscarCliente(lista, argv[a]);

            if (i == -1)
                cout << "Cliente " << argv[a] << " nao encontrado." << endl;
            else
                imprimirCompras(lista, i);
        }
    }

    return 0;
}
