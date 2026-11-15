#ifndef LISTACOMPRAS_H
#define LISTACOMPRAS_H

#include <vector>
#include <map>
#include <string>
using namespace std;

// Historico de compras da loja (Atividade 1).
// Clientes e produtos sao guardados por "indice interno" (0, 1, 2...)
// em vez do codigo original, para acessar os vetores direto pela posicao.
struct ListaCompras {
    vector<string> clientes;          // indice -> codigo original do cliente
    map<string, int> mapCliente;      // codigo original do cliente -> indice
    vector<string> produtos;          // indice -> nome do produto
    map<string, int> mapProduto;      // codigo original do produto -> indice
    vector<vector<int> > prodClientes; // prodClientes[i] = indices dos produtos comprados pelo cliente i

    // Matriz de compras (n clientes x m produtos), criada com malloc pela funcao
    // criarMatrizCompras (modulo Similaridade): matrizCompras[i][p] = 1 se o
    // cliente i comprou o produto p. Fica NULL ate ela ser criada.
    int **matrizCompras;
};

// Le o arquivo CSV e preenche a ListaCompras.
// Devolve 1 se deu certo e 0 se nao conseguiu abrir o arquivo.
int lerDados(char *caminhoArquivo, ListaCompras &lista);

// Devolve o indice do cliente com esse codigo, ou -1 se ele nao existir.
int buscarCliente(ListaCompras &lista, string codigo);

// Mostra o cliente i e o nome dos produtos que ele comprou.
void imprimirCompras(ListaCompras &lista, int i);

#endif
