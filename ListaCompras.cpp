#include <stdio.h>   
#include <iostream>   
#include "ListaCompras.h"

static int contem(vector<int> &produtosCliente, int produto) {
    int j;
    int total = produtosCliente.size();

    for (j = 0; j < total; j++) {
        if (produtosCliente[j] == produto)
            return 1; 
    }
    return 0;        
}

static int lerLinha(FILE *arquivo, char *cliente, char *produto, char *nome) {
    char linha[300];
    char data[20];
    int lidos;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        lidos = sscanf(linha, "%19[^,],%19[^,],%19[^,],%199[^\r\n]",
                       data, cliente, produto, nome);

        if (lidos == 4) {
            return 1;
        }
    }

    return 0;
}

int lerDados(char *caminhoArquivo, ListaCompras &lista) {
    FILE *arquivo;
    char cliente[20];     
    char produto[20];     
    char nome[200];     
    char cabecalho[300];  
    int c, p;

    
    lista.matrizCompras = NULL;

   
    arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL)
        return 0;

    
    fgets(cabecalho, sizeof(cabecalho), arquivo);

    
    while (lerLinha(arquivo, cliente, produto, nome) == 1) {

        
        if (lista.mapCliente.find(cliente) == lista.mapCliente.end()) {
            
            lista.mapCliente[cliente] = lista.clientes.size();
            lista.clientes.push_back(cliente);  

            
            lista.prodClientes.push_back({});
        }

        
        if (lista.mapProduto.find(produto) == lista.mapProduto.end()) {
            lista.mapProduto[produto] = lista.produtos.size();
            lista.produtos.push_back(nome);
        }
    }
    fclose(arquivo);   

   
    arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL)
        return 0;

    fgets(cabecalho, sizeof(cabecalho), arquivo);  

    while (lerLinha(arquivo, cliente, produto, nome) == 1) {
        
        c = lista.mapCliente[cliente];
        p = lista.mapProduto[produto];

        
        if (contem(lista.prodClientes[c], p) == 0)
            lista.prodClientes[c].push_back(p);
    }
    fclose(arquivo);

    return 1;
}


int buscarCliente(ListaCompras &lista, string codigo) {
    if (lista.mapCliente.find(codigo) != lista.mapCliente.end())
        return lista.mapCliente[codigo];

    return -1;
}


void imprimirCompras(ListaCompras &lista, int i) {
    int j, p;
    int total = lista.prodClientes[i].size();   

    cout << "Cliente " << i << " (codigo " << lista.clientes[i] << ") comprou "
         << total << " produto(s):" << endl;

    
    for (j = 0; j < total; j++) {
        p = lista.prodClientes[i][j];
        cout << "   - " << lista.produtos[p] << endl;
    }
}
