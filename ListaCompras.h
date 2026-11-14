#ifndef LISTACOMPRAS_H
#define LISTACOMPRAS_H

#include <vector>
#include <list>
#include <map>
#include <string>
using namespace std;

// Histórico de compras da loja.
// Tudo é guardado com "índices internos" (0, 1, 2...) em vez dos códigos
// originais, porque assim dá para acessar os vetores diretamente.
struct ListaCompras {
    vector<string> clientes;         // índice do cliente -> código original
    map<string, int> indiceCliente;  // código original -> índice do cliente
    vector<string> produtos;         // índice do produto -> nome do produto
    map<string, int> indiceProduto;  // código original -> índice do produto
    vector<list<int> > compras;      // compras[i] = índices dos produtos do cliente i
};

// Lê o arquivo CSV e preenche a ListaCompras.
// Devolve false se não conseguir abrir o arquivo.
bool carregarCompras(const char *nomeArquivo, ListaCompras &lc);

// Devolve o índice interno do cliente com esse código, ou -1 se não existir.
int buscarCliente(const ListaCompras &lc, const string &codigo);

// Mostra o cliente e os nomes dos produtos que ele comprou.
void imprimirCompras(const ListaCompras &lc, int i);

#endif
