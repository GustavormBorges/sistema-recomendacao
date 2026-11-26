#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iomanip>
#include "ListaCompras.h"
#include "Similaridade.h"
#include "Recomendacao.h"
#include "TesteCSR.h"

using namespace std;

static void perguntar(const char *pergunta, char *resposta, int tamanho) {
    int i;

    cout << pergunta;
    cout.flush();

    resposta[0] = '\0';
    if (fgets(resposta, tamanho, stdin) == NULL)
        resposta[0] = '\0';

    for (i = 0; resposta[i] != '\0'; i++) {
        if (resposta[i] == '\n' || resposta[i] == '\r') {
            resposta[i] = '\0';
            break;
        }
    }
}

static void mostrarCliente(ListaCompras &lista, char *codigo) {
    int i = buscarCliente(lista, codigo);

    if (i == -1)
        cout << "Cliente " << codigo << " nao encontrado." << endl;
    else
        imprimirCompras(lista, i);
}

static void testarLista(ListaCompras &lista, int argc, char *argv[]) {
    int i, a;
    char codigo[50];

    if (argc > 3) {
        for (a = 3; a < argc; a++)
            mostrarCliente(lista, argv[a]);
        return;
    }

    perguntar("Codigo do cliente (ENTER para ver os 3 primeiros): ", codigo, 50);
    cout << endl;

    if (codigo[0] == '\0') {
        for (i = 0; i < 3 && i < (int)lista.clientes.size(); i++)
            imprimirCompras(lista, i);
    } else {
        mostrarCliente(lista, codigo);
    }
}

static void testarSimilaridade(ListaCompras &lista, int argc, char *argv[]) {
    int n = lista.clientes.size();
    int i, j, maisSimilar;
    char texto[50];

    i = 0;
    if (argc >= 4) {
        i = atoi(argv[3]);
    } else {
        perguntar("Indice do cliente (ENTER para o cliente 0): ", texto, 50);
        cout << endl;

        if (texto[0] != '\0')
            i = atoi(texto);
    }

    if (i < 0 || i >= n) {
        cout << "Indice invalido. Use um valor de 0 a " << n - 1 << endl;
        return;
    }

    cout << "Calculando a similaridade entre os clientes..." << endl << endl;
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
        cout << "Cliente mais similar (s = " << similaridadeDensa(S, i, maisSimilar)
             << "; quanto menor, mais parecido):" << endl;
        imprimirCompras(lista, maisSimilar);
    }

    liberarMatrizDouble(S, n);
}

static void mostrarRecomendacao(ListaCompras &lista, int i, int k) {
    int r;
    vector<item> recomendados;

    imprimirCompras(lista, i);

    recomendados = recomendar(lista, i, k);

    if (recomendados.size() == 0) {
        cout << "Nenhum produto para recomendar (nenhum cliente parecido)." << endl;
    } else {
        cout << "Produtos recomendados (ate " << k << "; quanto menor o valor, mais indicado):" << endl;

        for (r = 0; r < (int)recomendados.size(); r++) {
            cout << "   " << r + 1 << ". " << lista.produtos[recomendados[r].produto]
                 << "  (valor " << recomendados[r].score << ")" << endl;
        }
    }

    cout << endl;
}

static void recomendarPorCodigo(ListaCompras &lista, char *codigo, int k) {
    int i = buscarCliente(lista, codigo);

    if (i == -1)
        cout << "Cliente " << codigo << " nao encontrado." << endl << endl;
    else
        mostrarRecomendacao(lista, i, k);
}

static void testarRecomendacao(ListaCompras &lista, int argc, char *argv[]) {
    int n = lista.clientes.size();
    int k, i, a;
    char texto[50];
    char codigo[50];

    if (argc >= 4) {
        k = atoi(argv[3]);
    } else {
        perguntar("Quantos produtos recomendar (k)? ", texto, 50);
        k = atoi(texto);
    }

    if (k < 1) {
        cout << "k invalido. Use um numero maior que 0." << endl;
        return;
    }

    codigo[0] = '\0';
    if (argc <= 4) {
        perguntar("Codigo do cliente (ENTER para os 3 primeiros): ", codigo, 50);
        cout << endl;
    }

    cout << "Calculando a similaridade entre os clientes..." << endl << endl;
    calcularSimilaridadeCSR(lista);

    if (argc > 4) {
        for (a = 4; a < argc; a++)
            recomendarPorCodigo(lista, argv[a], k);
    } else if (codigo[0] == '\0') {
        for (i = 0; i < 3 && i < n; i++)
            mostrarRecomendacao(lista, i, k);
    } else {
        recomendarPorCodigo(lista, codigo, k);
    }
}

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

static const char *nomeAlgoritmo(int algoritmo) {
    if (algoritmo == 1)
        return "Padrao (com transposta)";
    if (algoritmo == 2)
        return "Adaptado (sem transposta)";
    return "CSR (matriz esparsa)";
}

static double medirAlgoritmo(ListaCompras &lista, int algoritmo, int repeticoes) {
    int n = lista.clientes.size();
    int r;
    double soma = 0.0;
    double **S = NULL;
    clock_t inicio, fim;

    for (r = 0; r < repeticoes; r++) {
        inicio = clock();

        if (algoritmo == 3)
            calcularSimilaridadeCSR(lista);
        else
            S = calcularSimilaridade(lista, algoritmo);

        fim = clock();

        soma += (double)(fim - inicio) / CLOCKS_PER_SEC;

        if (algoritmo != 3)
            liberarMatrizDouble(S, n);
    }

    return soma / repeticoes;
}

static void testarDesempenho(ListaCompras &lista, int argc, char *argv[]) {
    int n = lista.clientes.size();
    int algoritmo, a, primeiro, ultimo, maisRapido;
    const int REPETICOES = 5;
    char texto[50];
    double tempo[4];
    double **S1;
    double **S2;

    if (argc >= 4) {
        algoritmo = atoi(argv[3]);
    } else {
        perguntar("Algoritmo (1 = padrao, 2 = adaptado, 3 = CSR, ENTER = todos): ", texto, 50);
        cout << endl;

        algoritmo = 0;
        if (texto[0] != '\0')
            algoritmo = atoi(texto);
    }

    if (algoritmo < 0 || algoritmo > 3) {
        cout << "Algoritmo invalido. Use 1, 2 ou 3." << endl;
        return;
    }

    primeiro = 1;
    ultimo = 3;
    if (algoritmo != 0) {
        primeiro = algoritmo;
        ultimo = algoritmo;
    }

    // as matrizes de compras ficam prontas antes: so a similaridade e medida
    criarMatrizCompras(lista);
    criarMatrizCSR(lista);

    cout << "Tempo de resposta do calculo da similaridade" << endl;
    cout << "Clientes: " << n << endl;
    cout << "Produtos: " << lista.produtos.size() << endl;
    cout << "Repeticoes: " << REPETICOES << endl << endl;

    cout << fixed << setprecision(6);

    maisRapido = primeiro;
    for (a = primeiro; a <= ultimo; a++) {
        tempo[a] = medirAlgoritmo(lista, a, REPETICOES);

        cout << "Algoritmo " << a << " - " << nomeAlgoritmo(a)
             << ": " << tempo[a] << " segundos" << endl;
    }

    if (algoritmo == 0) {
        cout << endl << "Verificando os resultados do padrao e do adaptado..." << endl;

        S1 = calcularSimilaridade(lista, 1);
        S2 = calcularSimilaridade(lista, 2);

        if (compararSimilaridades(S1, S2, n))
            cout << "Os resultados sao iguais." << endl;
        else
            cout << "Os resultados sao diferentes." << endl;

        liberarMatrizDouble(S1, n);
        liberarMatrizDouble(S2, n);
    }
}

int main(int argc, char *argv[]) {
    ListaCompras lista;
    int opcao;

    if (argc < 3) {
        cout << "Uso:" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 1 [cod_cliente ...]" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 2 [indice_cliente]" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 3 [k] [cod_cliente ...]" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 4 [algoritmo]" << endl;
        cout << "  " << argv[0] << " <arquivo.csv> 5" << endl;
        cout << "O que estiver entre [ ] e nao for digitado sera perguntado." << endl;
        return 1;
    }

    if (lerDados(argv[1], lista) == 0) {
        cout << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }

    cout << lista.clientes.size() << " clientes e " << lista.produtos.size()
         << " produtos carregados." << endl << endl;

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
            testarDesempenho(lista, argc, argv);
            break;

        case 5:
            testarCSR(lista);
            break;

        default:
            cout << "Opcao invalida: " << argv[2] << " (use 1, 2, 3, 4 ou 5)" << endl;
            liberarMatrizCompras(lista);
            return 1;
    }

    liberarMatrizCompras(lista);

    return 0;
}

