#ifndef LISTACOMPRAS_H
#define LISTACOMPRAS_H

#include <vector>
#include <list>
#include <map>
#include <string>
using namespace std;

// Historico de compras da loja.
// Clientes e produtos sao guardados por "indice interno" (0, 1, 2...)
// em vez do codigo original, para acessar os vetores direto pela posicao.
struct ListaCompras {
    vector<string> clientes;         // indice -> codigo original do cliente
    map<string, int> indiceCliente;  // codigo original do cliente -> indice
    vector<string> produtos;         // indice -> nome do produto
    map<string, int> indiceProduto;  // codigo original do produto -> indice
    vector<list<int> > compras;      // compras[i] = produtos comprados pelo cliente i
};

// Le o arquivo CSV e preenche a ListaCompras.
// Devolve 1 se deu certo e 0 se nao conseguiu abrir o arquivo.
int carregaCompras(char *nomeArquivo, ListaCompras &lc);

// Devolve o indice do cliente com esse codigo, ou -1 se ele nao existir.
int buscaCliente(ListaCompras &lc, string codigo);

// Mostra o cliente i e o nome dos produtos que ele comprou.
void imprimeCompras(ListaCompras &lc, int i);

#endif
