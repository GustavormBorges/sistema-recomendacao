#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <iomanip>
#include "ListaCompras.h"
#include "Similaridade.h"
#include "Recomendacao.h"

using namespace std;

static void testarLista(ListaCompras &lista, int argc, char *argv[]) {
    int i, a;

    cout << lista.clientes.size() << " clientes e " << lista.produtos.size()
         << " produtos carregados." << endl << endl;

    if (argc == 3) {
        for (i = 0; i < 3 && i < (int)lista.clientes.size(); i++)
            imprimirCompras(lista, i);
    } else {
        for (a = 3; a < argc; a++) {
            i = buscarCliente(lista, argv[a]);

            if (i == -1)
                cout << "Cliente " << argv[a] << " nao encontrado." << endl;
            else
                imprimirCompras(lista, i);
        }
    }
}

static void testarSimilaridade(ListaCompras &lista, int argc, char *argv[]) {
    int n = lista.clientes.size();
    int i, j, maisSimilar;

    i = 0;
    if (argc >= 4)
        i = atoi(argv[3]);

    if (i < 0 || i >= n) {
        cout << "Indice invalido. Use um valor de 0 a " << n - 1 << endl;
        return;
    }

    double **S = calcularSimilaridade(lista);

    maisSimilar = -1;
    for (j = 0; j < n; j++) {
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

    liberarMatrizDouble(S, n);
}

static void mostrarRecomendacao(ListaCompras &lista, double **S, int i, int k) {
    int r;
    vector<item> recomendados;

    imprimirCompras(lista, i);

    recomendados = recomendar(lista, S, i, k);

    if (recomendados.size() == 0) {
        cout << "Nenhum produto para recomendar (nenhum cliente parecido)." << endl;
    } else {
        cout << "Produtos recomendados:" << endl;

        for (r = 0; r < (int)recomendados.size(); r++) {
            cout << "   " << r + 1 << ". " << lista.produtos[recomendados[r].produto]
                 << "  (valor " << recomendados[r].score << ")" << endl;
        }
    }

    cout << endl;
}

static void testarRecomendacao(ListaCompras &lista, int argc, char *argv[]) {
    int n = lista.clientes.size();
    int k, i, a;

    if (argc < 4) {
        cout << "Falta o k. Uso: " << argv[0] << " <arquivo.csv> 3 <k> [cod_cliente ...]" << endl;
        return;
    }

    k = atoi(argv[3]);

    double **S = calcularSimilaridade(lista);

    if (argc == 4) {
        for (i = 0; i < 3 && i < n; i++)
            mostrarRecomendacao(lista, S, i, k);
    } else {
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

// Compara duas matrizes de similaridade.
static int compararSimilaridades(double **S1, double **S2, int n) {
    int i, j;
    double diferenca;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            diferenca = S1[i][j] - S2[i][j];

            if (diferenca > 0.000000001 || diferenca < -0.000000001) {
                return 0;
            }
        }
    }

    return 1;
}

// Compara o desempenho dos dois algoritmos.
static void testarDesempenho(ListaCompras &lista) {
    int n = lista.clientes.size();
    int repeticao;
    const int REPETICOES = 5;

    clock_t inicio, fim;

    double somaPadrao = 0.0;
    double somaOtimizado = 0.0;

    double tempoPadrao, tempoOtimizado;

    double **S1;
    double **S2;

    // Garante que a matriz de compras ja esteja pronta.
    if (lista.matrizCompras == NULL) {
        criarMatrizCompras(lista);
    }

    cout << "Comparacao dos algoritmos de similaridade" << endl;
    cout << "Clientes: " << n << endl;
    cout << "Produtos: " << lista.produtos.size() << endl;
    cout << "Repeticoes: " << REPETICOES << endl << endl;

    // Executa cinco vezes o algoritmo padrao.
    for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
        inicio = clock();

        S1 = calcularSimilaridade(lista, 1);

        fim = clock();

        tempoPadrao = (double)(fim - inicio) / CLOCKS_PER_SEC;

        somaPadrao += tempoPadrao;

        liberarMatrizDouble(S1, n);
    }

    // Executa cinco vezes o algoritmo otimizado.
    for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
        inicio = clock();

        S2 = calcularSimilaridade(lista, 2);

        fim = clock();

        tempoOtimizado = (double)(fim - inicio) / CLOCKS_PER_SEC;

        somaOtimizado += tempoOtimizado;

        liberarMatrizDouble(S2, n);
    }

    // Calcula as medias.
    tempoPadrao = somaPadrao / REPETICOES;
    tempoOtimizado = somaOtimizado / REPETICOES;

    cout << fixed << setprecision(6);

    cout << "Tempo medio do algoritmo 1 - Padrao: "
         << tempoPadrao << " segundos" << endl;

    cout << "Tempo medio do algoritmo 2 - Otimizado: "
         << tempoOtimizado << " segundos" << endl;

    cout << endl;

    if (tempoPadrao > tempoOtimizado) {
        cout << "O algoritmo otimizado foi mais rapido." << endl;
    } else if (tempoOtimizado > tempoPadrao) {
        cout << "O algoritmo padrao foi mais rapido." << endl;
    } else {
        cout << "Os dois algoritmos tiveram o mesmo tempo." << endl;
    }

    // Calcula novamente as matrizes para verificar os resultados.
    cout << endl << "Verificando os resultados..." << endl;

    S1 = calcularSimilaridade(lista, 1);
    S2 = calcularSimilaridade(lista, 2);

    if (compararSimilaridades(S1, S2, n)) {
        cout << "Os resultados dos dois algoritmos sao iguais." << endl;
    } else {
        cout << "Os resultados dos dois algoritmos sao diferentes." << endl;
    }

    liberarMatrizDouble(S1, n);
    liberarMatrizDouble(S2, n);
}

int main(int argc, char *argv[]) {
    ListaCompras lista;
    int opcao;

    if (argc < 3) {
        cout << "Uso:" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 1 [cod_cliente ...]" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 2 [indice_cliente]" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 3 <k> [cod_cliente ...]" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 4" << endl;
        return 1;
    }

    if (lerDados(argv[1], lista) == 0) {
        cout << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }

    opcao = atoi(argv[2]);

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

        case 4:
            testarDesempenho(lista);
            break;

        default:
            cout << "Opcao invalida: " << argv[2] << " (use 1, 2, 3 ou 4)" << endl;
            liberarMatrizCompras(lista);
            return 1;
    }

    liberarMatrizCompras(lista);

    return 0;
}



