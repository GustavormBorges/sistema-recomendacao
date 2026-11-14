#include "ListaCompras.h"
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>

// Lê a próxima linha de dados do CSV (data,cliente,produto,nome).
// Devolve false quando o arquivo acabar.
static bool lerLinha(FILE *arquivo, char *cliente, char *produto, char *nome) {
    char linha[300];
    char data[20];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // O nome é lido até o fim da linha, então vírgulas dentro dele não atrapalham.
        // O \r no [^\r\n] serve para arquivos com fim de linha do Windows.
        if (sscanf(linha, "%19[^,],%19[^,],%19[^,],%199[^\r\n]",
                   data, cliente, produto, nome) != 4) {
            continue;  // linha vazia ou estragada: ignora
        }

        // A data tem sempre 8 caracteres (aaaammdd). Em algumas linhas ela veio
        // corrompida (ex.: 2,02E+07) e virou DOIS campos por causa da vírgula.
        // Nesse caso pulamos um campo a mais.
        if (strlen(data) != 8) {
            if (sscanf(linha, "%*[^,],%*[^,],%19[^,],%19[^,],%199[^\r\n]",
                       cliente, produto, nome) != 3) {
                continue;
            }
        }

        // Os nomes vêm com espaços sobrando no final: tira.
        int tamanho = strlen(nome);
        while (tamanho > 0 && nome[tamanho - 1] == ' ') {
            nome[tamanho - 1] = '\0';
            tamanho--;
        }
        return true;
    }
    return false;
}

bool carregarCompras(const char *nomeArquivo, ListaCompras &lc) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        return false;
    }

    char cliente[20];
    char produto[20];
    char nome[200];
    char cabecalho[300];

    // 1ª passagem: descobre quais são os clientes e os produtos.
    fgets(cabecalho, sizeof(cabecalho), arquivo);  // descarta o cabeçalho
    while (lerLinha(arquivo, cliente, produto, nome)) {
        if (lc.indiceCliente.find(cliente) == lc.indiceCliente.end()) {
            lc.indiceCliente[cliente] = lc.clientes.size();
            lc.clientes.push_back(cliente);
        }
        if (lc.indiceProduto.find(produto) == lc.indiceProduto.end()) {
            lc.indiceProduto[produto] = lc.produtos.size();
            lc.produtos.push_back(nome);
        }
    }

    // Agora que sabemos quantos clientes existem, criamos uma lista vazia para cada um.
    lc.compras.resize(lc.clientes.size());

    // 2ª passagem: preenche a lista de compras de cada cliente.
    rewind(arquivo);                               // volta ao começo do arquivo
    fgets(cabecalho, sizeof(cabecalho), arquivo);  // descarta o cabeçalho de novo
    while (lerLinha(arquivo, cliente, produto, nome)) {
        int c = lc.indiceCliente[cliente];
        int p = lc.indiceProduto[produto];

        // Se o cliente comprou o mesmo produto mais de uma vez, guardamos só uma.
        // (A matriz de compras só diz "comprou" ou "não comprou".)
        list<int> &lista = lc.compras[c];
        if (find(lista.begin(), lista.end(), p) == lista.end()) {
            lista.push_back(p);
        }
    }

    fclose(arquivo);
    return true;
}

int buscarCliente(const ListaCompras &lc, const string &codigo) {
    map<string, int>::const_iterator it = lc.indiceCliente.find(codigo);
    if (it == lc.indiceCliente.end()) {
        return -1;
    }
    return it->second;
}

void imprimirCompras(const ListaCompras &lc, int i) {
    cout << "Cliente " << i << " (codigo " << lc.clientes[i] << ") comprou "
         << lc.compras[i].size() << " produto(s):" << endl;
    for (int p : lc.compras[i]) {
        cout << "   - " << lc.produtos[p] << endl;
    }
}
