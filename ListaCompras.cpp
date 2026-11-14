// ===========================================================================
// ATIVIDADE 1 - Modulo ListaCompras
//
// Objetivo: ler o arquivo CSV de compras e guardar tudo em estruturas
// organizadas, para os outros modulos (Similaridade e Recomendacao) usarem.
//
// Cada linha do CSV e uma compra:  DATA_COMPRA,COD_CLIENTE,COD_PRODUTO,NOME_PRODUTO
// Exemplo:  20200515,05090301,025728,SMARTPHONE SAMSUNG GAL J4 CORE 16GB PRETO
//
// Este arquivo NAO tem main: o main fica em arquivo separado (main.cpp).
//
// Ordem do arquivo:
//   1) tirarEspacos - funcao auxiliar
//   2) contem       - funcao auxiliar
//   3) lerLinha     - le UMA linha do CSV
//   4) lerDados     - usa as funcoes acima para montar a ListaCompras
//   5) buscarCliente e imprimirCompras - usadas pelo main
// ===========================================================================

#include <stdio.h>    // FILE, fopen, fgets, sscanf, fclose
#include <string.h>   // strlen
#include <iostream>   // cout
#include "ListaCompras.h"

// ---------------------------------------------------------------------------
// tirarEspacos: apaga os espacos que sobram no FINAL de um texto.
//
// Por que? No CSV os nomes vem "preenchidos" com espacos, assim:
//     "TABLET NB316 M7S 16GB QUAD CORE PRETO        "
// Ao mostrar o nome na tela, os espacos extras so atrapalham.
//
// Como funciona: em C, um texto termina no caractere '\0'. Se colocamos um
// '\0' no lugar do ultimo espaco, o texto "encurta". Repetimos enquanto o
// ultimo caractere for espaco.
//
// "static" significa que a funcao so existe dentro deste arquivo.
// ---------------------------------------------------------------------------
static void tirarEspacos(char *texto) {
    int tamanho = strlen(texto);   // quantos caracteres o texto tem

    // texto[tamanho - 1] e o ultimo caractere.
    while (tamanho > 0 && texto[tamanho - 1] == ' ') {
        texto[tamanho - 1] = '\0';   // "corta" o texto aqui
        tamanho--;                   // agora o texto tem 1 caractere a menos
    }
}

// ---------------------------------------------------------------------------
// contem: diz se um produto ja esta dentro da lista de produtos de um cliente.
// Devolve 1 se esta e 0 se nao esta (mesmo estilo de "flag" do jogo da velha).
//
// "list<int> &produtosCliente": o & quer dizer que a funcao usa a lista
// original, sem fazer uma copia (copiar uma lista grande seria desperdicio).
//
// Uma list NAO tem indice (nao existe lista[2]). Por isso percorremos com
// "for (int p : lista)", o mesmo jeito do exemplo de list do enunciado: a cada
// volta, "p" vale o proximo elemento da lista.
// ---------------------------------------------------------------------------
static int contem(list<int> &produtosCliente, int produto) {
    for (int p : produtosCliente) {
        if (p == produto)
            return 1;   // achou: ja esta na lista
    }
    return 0;           // olhou todos e nao achou
}

// ---------------------------------------------------------------------------
// lerLinha: le a PROXIMA linha de dados do CSV e separa os campos.
//
// Parametros:
//   arquivo  - o arquivo ja aberto
//   cliente, produto, nome - onde a funcao coloca o que leu (saidas)
//
// Devolve 1 se conseguiu ler uma linha, e 0 se o arquivo acabou.
//
// Por que nao usar o fscanf do exemplo do professor? Porque nos dados existem
// linhas "estranhas" e o fscanf pararia de ler no primeiro problema, perdendo
// o resto do arquivo sem avisar. Aqui lemos a linha inteira com fgets e depois
// separamos os campos com sscanf (que funciona igual ao fscanf, mas le de um
// texto que ja esta na memoria em vez de ler direto do arquivo).
// ---------------------------------------------------------------------------
static int lerLinha(FILE *arquivo, char *cliente, char *produto, char *nome) {
    char linha[300];   // guarda a linha inteira lida do arquivo
    char data[20];     // so serve para conferir se a linha esta certa
    int lidos;         // quantos campos o sscanf conseguiu ler
    int ok;            // 1 se a linha serviu, 0 se nao

    // fgets le UMA linha do arquivo para dentro de "linha".
    // Devolve NULL quando o arquivo acaba, e ai o while termina.
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        ok = 0;

        // Separa os 4 campos que estao divididos por virgula.
        //   %19[^,]   = le ate 19 caracteres, parando na virgula
        //   %199[^\r\n] = le ate o fim da linha (o nome pode ter virgula dentro,
        //               por isso o ultimo campo vai ate o fim, nao ate a virgula)
        // O \r e para arquivos criados no Windows, onde a linha termina em \r\n.
        // O sscanf devolve quantos campos leu: se deu certo, sao 4.
        lidos = sscanf(linha, "%19[^,],%19[^,],%19[^,],%199[^\r\n]",
                       data, cliente, produto, nome);

        if (lidos == 4) {
            if (strlen(data) == 8) {
                // A data normal tem exatamente 8 caracteres (aaaammdd,
                // como 20200515). Entao a linha esta certa.
                ok = 1;
            } else {
                // Problema dos dados: em 3 linhas a data veio corrompida
                // (por exemplo "2,02E+07"). Essa data tem uma virgula, entao
                // ela virou DOIS campos: "2" e "02E+07".
                // Solucao: ler de novo, mas descartando 2 campos no comeco.
                // O %* le o campo e joga fora (e o mesmo %* que o exemplo do
                // professor usa para descartar o cabecalho).
                lidos = sscanf(linha, "%*[^,],%*[^,],%19[^,],%19[^,],%199[^\r\n]",
                               cliente, produto, nome);

                // Agora so 3 campos sao guardados (cliente, produto e nome).
                if (lidos == 3)
                    ok = 1;
            }
        }

        if (ok == 1) {
            tirarEspacos(nome);   // limpa os espacos do final do nome
            return 1;             // leu uma linha valida: pronto
        }
        // Se chegou aqui, a linha estava vazia ou estragada.
        // O while volta e tenta a proxima linha.
    }

    return 0;   // o arquivo acabou
}

// ---------------------------------------------------------------------------
// lerDados: le o arquivo CSV e preenche a ListaCompras.
//
// O enunciado manda fazer em DUAS fases (Figura 4):
//
//  1a passagem pelo arquivo: descobre quem sao os clientes e os produtos.
//     - clientes[i]    : codigo do cliente numero i
//     - mapCliente     : dado um codigo, devolve o numero (indice) do cliente
//     - produtos[p]    : nome do produto numero p
//     - mapProduto     : dado um codigo, devolve o numero do produto
//     - prodClientes   : cada vez que aparece um cliente NOVO, ja criamos uma
//                        lista vazia para ele (ainda sem produtos)
//
//  2a passagem pelo arquivo: preenche prodClientes[i], os produtos que cada
//     cliente comprou.
//
// Devolve 1 se deu certo e 0 se nao conseguiu abrir o arquivo.
// ---------------------------------------------------------------------------
int lerDados(char *caminhoArquivo, ListaCompras &lista) {
    FILE *arquivo;
    char cliente[20];     // codigo do cliente da linha lida
    char produto[20];     // codigo do produto da linha lida
    char nome[200];       // nome do produto da linha lida
    char cabecalho[300];  // so para jogar fora a primeira linha do CSV
    int c, p;

    // Abre o arquivo para leitura. Se der erro, fopen devolve NULL.
    arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL)
        return 0;

    // ---------------- 1a PASSAGEM ----------------

    // A primeira linha do CSV e o cabecalho (DATA_COMPRA,COD_CLIENTE,...),
    // que nao e uma compra. Le e joga fora.
    fgets(cabecalho, sizeof(cabecalho), arquivo);

    // Repete enquanto conseguir ler uma linha de compra.
    while (lerLinha(arquivo, cliente, produto, nome) == 1) {

        // O cliente ainda nao esta no mapa? Entao e a primeira vez que ele aparece.
        // (find devolve end() quando a chave NAO existe - igual no exemplo
        // do professor sobre map.)
        if (lista.mapCliente.find(cliente) == lista.mapCliente.end()) {
            // O indice do novo cliente e a posicao dele no vetor, ou seja,
            // o tamanho atual do vetor (se ja tem 5 clientes, o novo e o 5).
            lista.mapCliente[cliente] = lista.clientes.size();
            lista.clientes.push_back(cliente);   // coloca no final do vetor

            // Cria a lista de produtos desse cliente, ainda vazia.
            // {} quer dizer "lista vazia".
            lista.prodClientes.push_back({});
        }

        // Mesma ideia para o produto. Guardamos o NOME no vetor de produtos,
        // e o mapa liga o codigo original ao indice.
        if (lista.mapProduto.find(produto) == lista.mapProduto.end()) {
            lista.mapProduto[produto] = lista.produtos.size();
            lista.produtos.push_back(nome);
        }
    }
    fclose(arquivo);   // terminou a 1a passagem

    // ---------------- 2a PASSAGEM ----------------

    // Abrimos o arquivo de novo para ler tudo desde o comeco.
    arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL)
        return 0;

    fgets(cabecalho, sizeof(cabecalho), arquivo);   // pula o cabecalho de novo

    while (lerLinha(arquivo, cliente, produto, nome) == 1) {
        // Descobre os indices a partir dos codigos originais usando os mapas.
        c = lista.mapCliente[cliente];
        p = lista.mapProduto[produto];

        // Problema dos dados: o mesmo cliente pode ter comprado o mesmo produto
        // mais de uma vez. Na matriz de compras so importa se comprou ou nao,
        // entao so adicionamos o produto se ele ainda nao estiver na lista.
        if (contem(lista.prodClientes[c], p) == 0)
            lista.prodClientes[c].push_back(p);
    }
    fclose(arquivo);

    return 1;
}

// ---------------------------------------------------------------------------
// buscarCliente: dado o codigo ORIGINAL de um cliente, devolve o indice dele.
// Se o codigo nao existe, devolve -1 (um valor que nunca e indice valido).
// Mesmo padrao do exemplo do professor: find(...) != end() quer dizer "achou".
// ---------------------------------------------------------------------------
int buscarCliente(ListaCompras &lista, string codigo) {
    if (lista.mapCliente.find(codigo) != lista.mapCliente.end())
        return lista.mapCliente[codigo];

    return -1;
}

// ---------------------------------------------------------------------------
// imprimirCompras: mostra na tela o cliente i e os nomes dos produtos que ele
// comprou. Usada pelo main.
// ---------------------------------------------------------------------------
void imprimirCompras(ListaCompras &lista, int i) {
    int total = lista.prodClientes[i].size();   // quantos produtos o cliente i comprou

    cout << "Cliente " << i << " (codigo " << lista.clientes[i] << ") comprou "
         << total << " produto(s):" << endl;

    // p percorre a lista do cliente: a cada volta vale o indice de um produto.
    // lista.produtos[p] e o nome desse produto.
    for (int p : lista.prodClientes[i])
        cout << "   - " << lista.produtos[p] << endl;
}
