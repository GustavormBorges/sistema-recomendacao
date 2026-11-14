// ===========================================================================
// ATIVIDADE 1 - Modulo ListaCompras
//
// Objetivo: ler o arquivo CSV de compras e guardar tudo em estruturas
// organizadas, para os outros modulos (Similaridade e Recomendacao) usarem.
//
// Cada linha do CSV e uma compra:  DATA_COMPRA,COD_CLIENTE,COD_PRODUTO,NOME_PRODUTO
// Exemplo:  20200515,05090301,025728,SMARTPHONE SAMSUNG GAL J4 CORE 16GB PRETO
//
// Ordem do arquivo:
//   1) tiraEspacos  - funcao auxiliar
//   2) contem       - funcao auxiliar
//   3) lerLinha     - le UMA linha do CSV
//   4) carregaCompras - usa as funcoes acima para montar a ListaCompras
//   5) buscaCliente e imprimeCompras - usadas pelos testadores
// ===========================================================================

#include <stdio.h>    // FILE, fopen, fgets, sscanf, fclose
#include <string.h>   // strlen
#include <iostream>   // cout
#include "ListaCompras.h"

// ---------------------------------------------------------------------------
// tiraEspacos: apaga os espacos que sobram no FINAL de um texto.
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
static void tiraEspacos(char *texto) {
    int tamanho = strlen(texto);   // quantos caracteres o texto tem

    // texto[tamanho - 1] e o ultimo caractere.
    while (tamanho > 0 && texto[tamanho - 1] == ' ') {
        texto[tamanho - 1] = '\0';   // "corta" o texto aqui
        tamanho--;                   // agora o texto tem 1 caractere a menos
    }
}

// ---------------------------------------------------------------------------
// contem: diz se um produto ja esta dentro de uma lista de produtos.
// Devolve 1 se esta e 0 se nao esta (mesmo estilo de "flag" do jogo da velha).
//
// "list<int> &lista": o & quer dizer que a funcao usa a lista original, sem
// fazer uma copia (copiar uma lista grande seria desperdicio).
//
// O "for (int x : lista)" e o mesmo que aparece no exemplo da aula sobre
// list: percorre todos os elementos da lista, um de cada vez, e x vale o
// elemento da vez. (Lista nao tem indice, entao nao da para usar lista[i].)
// ---------------------------------------------------------------------------
static int contem(list<int> &lista, int produto) {
    for (int x : lista) {
        if (x == produto)
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
            tiraEspacos(nome);   // limpa os espacos do final do nome
            return 1;            // leu uma linha valida: pronto
        }
        // Se chegou aqui, a linha estava vazia ou estragada.
        // O while volta e tenta a proxima linha.
    }

    return 0;   // o arquivo acabou
}

// ---------------------------------------------------------------------------
// carregaCompras: le o arquivo CSV e preenche a ListaCompras.
//
// Funciona em DUAS passagens pelo arquivo (veja a Figura 4 do enunciado):
//
//  1a passagem: descobre quem sao os clientes e os produtos.
//     - clientes[i]       : codigo do cliente numero i
//     - indiceCliente     : dado um codigo, devolve o numero (indice) do cliente
//     - produtos[p]       : nome do produto numero p
//     - indiceProduto     : dado um codigo, devolve o numero do produto
//
//  2a passagem: monta compras[i], a lista de produtos de cada cliente.
//     So da para criar "compras" depois da 1a passagem, porque precisamos
//     saber ANTES quantos clientes existem.
//
// Devolve 1 se deu certo e 0 se nao conseguiu abrir o arquivo.
// ---------------------------------------------------------------------------
int carregaCompras(char *nomeArquivo, ListaCompras &lc) {
    FILE *arquivo;
    char cliente[20];     // codigo do cliente da linha lida
    char produto[20];     // codigo do produto da linha lida
    char nome[200];       // nome do produto da linha lida
    char cabecalho[300];  // so para jogar fora a primeira linha do CSV
    int i, c, p;

    // Abre o arquivo para leitura. Se der erro, fopen devolve NULL.
    arquivo = fopen(nomeArquivo, "r");
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
        if (lc.indiceCliente.find(cliente) == lc.indiceCliente.end()) {
            // O indice do novo cliente e a posicao dele no vetor, ou seja,
            // o tamanho atual do vetor (se ja tem 5 clientes, o novo e o 5).
            lc.indiceCliente[cliente] = lc.clientes.size();
            lc.clientes.push_back(cliente);   // coloca no final do vetor
        }

        // Mesma ideia para o produto. Guardamos o NOME no vetor de produtos,
        // e o mapa liga o codigo original ao indice.
        if (lc.indiceProduto.find(produto) == lc.indiceProduto.end()) {
            lc.indiceProduto[produto] = lc.produtos.size();
            lc.produtos.push_back(nome);
        }
    }
    fclose(arquivo);   // terminou a 1a passagem

    // Agora que sabemos quantos clientes existem, criamos uma lista VAZIA de
    // compras para cada um. Assim compras[i] ja existe quando formos usar.
    for (i = 0; i < (int)lc.clientes.size(); i++) {
        list<int> vazia;
        lc.compras.push_back(vazia);
    }

    // ---------------- 2a PASSAGEM ----------------

    // Abrimos o arquivo de novo para ler tudo desde o comeco.
    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
        return 0;

    fgets(cabecalho, sizeof(cabecalho), arquivo);   // pula o cabecalho de novo

    while (lerLinha(arquivo, cliente, produto, nome) == 1) {
        // Descobre os indices a partir dos codigos originais usando os mapas.
        c = lc.indiceCliente[cliente];
        p = lc.indiceProduto[produto];

        // Problema dos dados: o mesmo cliente pode ter comprado o mesmo produto
        // mais de uma vez. Na matriz de compras so importa se comprou ou nao,
        // entao so adicionamos o produto se ele ainda nao estiver na lista.
        if (contem(lc.compras[c], p) == 0)
            lc.compras[c].push_back(p);
    }
    fclose(arquivo);

    return 1;
}

// ---------------------------------------------------------------------------
// buscaCliente: dado o codigo ORIGINAL de um cliente, devolve o indice dele.
// Se o codigo nao existe, devolve -1 (um valor que nunca e indice valido).
// Mesmo padrao do exemplo do professor: find(...) != end() quer dizer "achou".
// ---------------------------------------------------------------------------
int buscaCliente(ListaCompras &lc, string codigo) {
    if (lc.indiceCliente.find(codigo) != lc.indiceCliente.end())
        return lc.indiceCliente[codigo];

    return -1;
}

// ---------------------------------------------------------------------------
// imprimeCompras: mostra na tela o cliente i e os nomes dos produtos que ele
// comprou. Usada pelos programas testadores.
// ---------------------------------------------------------------------------
void imprimeCompras(ListaCompras &lc, int i) {
    // lc.compras[i].size() = quantos produtos tem na lista do cliente i
    cout << "Cliente " << i << " (codigo " << lc.clientes[i] << ") comprou "
         << lc.compras[i].size() << " produto(s):" << endl;

    // p vale, a cada volta, o indice de um produto comprado.
    // lc.produtos[p] e o nome desse produto.
    for (int p : lc.compras[i])
        cout << "   - " << lc.produtos[p] << endl;
}
