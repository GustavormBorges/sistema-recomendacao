// ===========================================================================
// MAIN do projeto (programa testador das Atividades 1, 2 e 3)
//
// Um unico programa. A opcao digitada no terminal escolhe o que executar:
//
//     sistema <arquivo.csv> 1 [cod_cliente ...]
//         Atividade 1: mostra os produtos comprados por alguns clientes,
//         pelo CODIGO ORIGINAL (o que aparece no CSV).
//         Sem codigos, usa os 3 primeiros clientes do arquivo.
//
//     sistema <arquivo.csv> 2 [indice_cliente]
//         Atividade 2: mostra o cliente mais parecido com um cliente, que nao
//         seja ele mesmo. O cliente e escolhido pelo INDICE interno (0, 1, 2...).
//         Sem indice, usa o cliente 0.
//
//     sistema <arquivo.csv> 3 <k> [cod_cliente ...]
//         Atividade 3: mostra os k produtos recomendados para alguns clientes,
//         pelo CODIGO ORIGINAL. Sem codigos, usa os 3 primeiros clientes.
//
// argc = quantos itens foram digitados no terminal (contando o nome do programa)
// argv = os itens digitados: argv[0] e o programa, argv[1] e o arquivo,
//        argv[2] e a opcao, e o resto depende da opcao.
// ===========================================================================

#include <iostream>
#include <stdlib.h>   // atoi
#include "ListaCompras.h"
#include "Similaridade.h"
#include "Recomendacao.h"

// ---------------------------------------------------------------------------
// ATIVIDADE 1: mostra as compras dos clientes pedidos (codigos a partir de argv[3]).
// ---------------------------------------------------------------------------
static void testarLista(ListaCompras &lista, int argc, char *argv[]) {
    int i, a;

    cout << lista.clientes.size() << " clientes e " << lista.produtos.size()
         << " produtos carregados." << endl << endl;

    if (argc == 3) {
        // Sem codigos: mostra os 3 primeiros clientes (indices 0, 1 e 2).
        for (i = 0; i < 3 && i < (int)lista.clientes.size(); i++)
            imprimirCompras(lista, i);
    } else {
        // Com codigos: para cada um, descobre o indice e mostra as compras.
        for (a = 3; a < argc; a++) {
            i = buscarCliente(lista, argv[a]);

            if (i == -1)
                cout << "Cliente " << argv[a] << " nao encontrado." << endl;
            else
                imprimirCompras(lista, i);
        }
    }
}

// ---------------------------------------------------------------------------
// ATIVIDADE 2: mostra o cliente mais parecido com o cliente de indice argv[3].
// ---------------------------------------------------------------------------
static void testarSimilaridade(ListaCompras &lista, int argc, char *argv[]) {
    int n = lista.clientes.size();
    int i, j, maisSimilar;

    // Cliente escolhido: o indice digitado, ou 0 se nao digitou nada.
    // atoi transforma o texto "153" no numero 153.
    i = 0;
    if (argc >= 4)
        i = atoi(argv[3]);

    if (i < 0 || i >= n) {
        cout << "Indice invalido. Use um valor de 0 a " << n - 1 << endl;
        return;
    }

    // Calcula a matriz de similaridade de todos os clientes (criada com malloc).
    double **S = calcularSimilaridade(lista);

    // Procura o cliente j com MENOR similaridade com i (o mais parecido).
    // maisSimilar guarda o melhor encontrado ate agora; -1 = ainda nao achou.
    maisSimilar = -1;
    for (j = 0; j < n; j++) {
        // Ignora o proprio cliente e quem nao comprou nada em comum (s = 1).
        if (j != i && similaridadeDensa(S, i, j) < 1.0) {
            if (maisSimilar == -1 || similaridadeDensa(S, i, j) < similaridadeDensa(S, i, maisSimilar))
                maisSimilar = j;
        }
    }

    imprimirCompras(lista, i);
    cout << endl;

    if (maisSimilar == -1) {
        cout << "Nenhum outro cliente comprou o mesmo que o cliente " << i << "." << endl;
    } else {
        cout << "Cliente mais similar (s = " << similaridadeDensa(S, i, maisSimilar) << "):" << endl;
        imprimirCompras(lista, maisSimilar);
    }

    liberarMatrizDouble(S, n);   // tudo que veio de malloc precisa de free
}

// ---------------------------------------------------------------------------
// ATIVIDADE 3 (auxiliar): mostra as compras do cliente i e os k produtos
// recomendados para ele.
// ---------------------------------------------------------------------------
static void mostrarRecomendacao(ListaCompras &lista, double **S, int i, int k) {
    int r;
    vector<item> recomendados;

    imprimirCompras(lista, i);

    // Chama o modulo Recomendacao.
    recomendados = recomendar(lista, S, i, k);

    if (recomendados.size() == 0) {
        cout << "Nenhum produto para recomendar (nenhum cliente parecido)." << endl;
    } else {
        cout << "Produtos recomendados:" << endl;
        for (r = 0; r < (int)recomendados.size(); r++) {
            // recomendados[r].produto e o indice do produto: pegamos o nome
            // dele no vetor de produtos. "score" e a nota (menor = melhor).
            cout << "   " << r + 1 << ". " << lista.produtos[recomendados[r].produto]
                 << "  (valor " << recomendados[r].score << ")" << endl;
        }
    }
    cout << endl;
}

// ---------------------------------------------------------------------------
// ATIVIDADE 3: recomenda k produtos (k = argv[3]) para os clientes pedidos
// (codigos a partir de argv[4]).
// ---------------------------------------------------------------------------
static void testarRecomendacao(ListaCompras &lista, int argc, char *argv[]) {
    int n = lista.clientes.size();
    int k, i, a;

    if (argc < 4) {
        cout << "Falta o k. Uso: " << argv[0] << " <arquivo.csv> 3 <k> [cod_cliente ...]" << endl;
        return;
    }

    k = atoi(argv[3]);   // texto "5" -> numero 5

    double **S = calcularSimilaridade(lista);

    if (argc == 4) {
        // Sem codigos: recomenda para os 3 primeiros clientes.
        for (i = 0; i < 3 && i < n; i++)
            mostrarRecomendacao(lista, S, i, k);
    } else {
        // Com codigos: para cada um, descobre o indice e recomenda.
        for (a = 4; a < argc; a++) {
            i = buscarCliente(lista, argv[a]);

            if (i == -1)
                cout << "Cliente " << argv[a] << " nao encontrado." << endl << endl;
            else
                mostrarRecomendacao(lista, S, i, k);
        }
    }

    liberarMatrizDouble(S, n);
}

// ---------------------------------------------------------------------------
// main: le o arquivo e usa um switch para executar a opcao escolhida.
// ---------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    ListaCompras lista;
    int opcao;

    // Precisa de pelo menos o arquivo e a opcao.
    if (argc < 3) {
        cout << "Uso:" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 1 [cod_cliente ...]" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 2 [indice_cliente]" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 3 <k> [cod_cliente ...]" << endl;
        return 1;
    }

    // Le o arquivo e monta as estruturas. Devolve 0 se nao conseguiu abrir.
    if (lerDados(argv[1], lista) == 0) {
        cout << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }

    opcao = atoi(argv[2]);   // texto "2" -> numero 2

    // O switch compara "opcao" com cada case e executa o que combina.
    // O break encerra o case (sem ele, o programa "cairia" no case seguinte).
    switch (opcao) {
        case 1:
            testarLista(lista, argc, argv);
            break;
        case 2:
            testarSimilaridade(lista, argc, argv);
            break;
        case 3:
            testarRecomendacao(lista, argc, argv);
            break;
        default:
            cout << "Opcao invalida: " << argv[2] << " (use 1, 2 ou 3)" << endl;
            liberarMatrizCompras(lista);
            return 1;
    }

    liberarMatrizCompras(lista);   // libera a matriz de compras (se foi criada)
    return 0;
}
