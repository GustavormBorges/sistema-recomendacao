#ifndef LISTACOMPRAS_H
#define LISTACOMPRAS_H

#include <vector>
#include <map>
#include <string>
using namespace std;

struct ListaCompras {
    vector<string> clientes;          
    map<string, int> mapCliente;      
    vector<string> produtos;         
    map<string, int> mapProduto;      
    vector<vector<int> > prodClientes;

   
    int **matrizCompras;
};

int lerDados(char *caminhoArquivo, ListaCompras &lista);

int buscarCliente(ListaCompras &lista, string codigo);

void imprimirCompras(ListaCompras &lista, int i);

#endif
