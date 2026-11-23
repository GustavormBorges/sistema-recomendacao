
#include <iostream>
#include <ctime>
#include <iomanip>
#include "TesteCSR.h"
#include "Similaridade.h"

using namespace std;

static int compararSimilaridadesCSR(double **S1, ListaCompras &lista, int n) {
    int i, j;
    double diferenca;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            diferenca = S1[i][j] - similaridadeCSR(lista, i, j);

            if (diferenca > 0.000000001 || diferenca < -0.000000001) {
                return 0;
            }
        }
    }

    return 1;
}

void testarCSR(ListaCompras &lista) {
    int n = lista.clientes.size();
    int repeticao;
    int totalElementos;
    int totalDenso;
    int totalIntersecoes;
    int totalSimilaridades;
    int totalDensoSimilaridades;
    double somaDenso = 0.0;
    double somaCSR = 0.0;
    double tempoDenso, tempoCSR;
    const int REPETICOES = 5;

    clock_t inicio, fim;

    double **S1;

    criarMatrizCompras(lista);
    criarMatrizCSR(lista);
    calcularSimilaridadeCSR(lista);

    totalElementos = lista.matrizCSR.values.size();
    totalDenso = n * lista.produtos.size();

    cout << "Comparacao entre matriz densa e CSR" << endl;
    cout << "Clientes: " << n << endl;
    cout << "Produtos: " << lista.produtos.size() << endl;

    cout << endl << "Matriz de compras:" << endl;
    cout << "Elementos da matriz densa: " << totalDenso << endl;
    cout << "Elementos armazenados na CSR: " << totalElementos << endl;

    if (totalDenso > 0) {
        cout << "Porcentagem de elementos armazenados: "
             << fixed << setprecision(2)
             << (double)totalElementos * 100.0 / totalDenso << "%" << endl;
    }

    totalIntersecoes = lista.matrizIntersecoes.values.size();

    cout << endl << "Matriz de intersecoes:" << endl;
    cout << "Elementos da matriz densa: " << n * n << endl;
    cout << "Elementos armazenados na CSR: " << totalIntersecoes << endl;

    if (n * n > 0) {
        cout << "Porcentagem de elementos armazenados: "
             << (double)totalIntersecoes * 100.0 / (n * n) << "%" << endl;
    }

    totalSimilaridades = lista.matrizSimilaridadeCSR.values.size();
    totalDensoSimilaridades = n * n;

    cout << endl << "Matriz de similaridade:" << endl;
    cout << "Elementos da matriz densa: " << totalDensoSimilaridades << endl;
    cout << "Elementos armazenados na CSR: " << totalSimilaridades << endl;

    if (totalDensoSimilaridades > 0) {
        cout << "Porcentagem de elementos armazenados: "
             << (double)totalSimilaridades * 100.0 / totalDensoSimilaridades << "%" << endl;
    }

    cout << endl;

    for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
        inicio = clock();

        S1 = calcularSimilaridade(lista, 2);

        fim = clock();

        somaDenso += (double)(fim - inicio) / CLOCKS_PER_SEC;

        liberarMatrizDouble(S1, n);
    }

    for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
        inicio = clock();

        calcularSimilaridadeCSR(lista);

        fim = clock();

        somaCSR += (double)(fim - inicio) / CLOCKS_PER_SEC;
    }

    tempoDenso = somaDenso / REPETICOES;
    tempoCSR = somaCSR / REPETICOES;

    cout << fixed << setprecision(6);

    cout << "Tempo medio da matriz densa: "
         << tempoDenso << " segundos" << endl;

    cout << "Tempo medio da CSR: "
         << tempoCSR << " segundos" << endl;

    cout << endl << "Verificando os resultados..." << endl;

    S1 = calcularSimilaridade(lista, 2);

    if (compararSimilaridadesCSR(S1, lista, n)) {
        cout << "Os resultados da matriz densa e da CSR sao iguais." << endl;
    } else {
        cout << "Os resultados da matriz densa e da CSR sao diferentes." << endl;
    }

    liberarMatrizDouble(S1, n);
}

