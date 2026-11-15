# Sistema de Recomendação

Projeto da disciplina de Programação Estruturada: sistema de recomendação de produtos
para clientes de uma loja de varejo, usando o **método da vizinhança** (filtragem
colaborativa baseada em usuário) com a **distância de Jaccard assimétrica**.

Clientes que compraram produtos parecidos são "vizinhos". Os produtos que os vizinhos
compraram (e o cliente ainda não) são recomendados, e quanto mais parecido o vizinho,
mais bem ranqueado o produto fica.

## Estrutura

| Arquivos | Atividade | O que faz |
|---|---|---|
| `ListaCompras.h/.cpp` | A1 | Lê o CSV e organiza clientes, produtos e a lista de compras de cada cliente |
| `Similaridade.h/.cpp` | A2 | Monta a matriz de compras e calcula a matriz de similaridade entre clientes |
| `Recomendacao.h/.cpp` | A3 | Gera os `k` produtos mais bem ranqueados para um cliente |
| `main.cpp` | A1, A2 e A3 | Programa testador: um `switch` escolhe a atividade a executar |

Os módulos não têm `main`: o programa testador fica em um arquivo separado, o `main.cpp`.

## Como compilar

Precisa do `g++`. Na pasta do projeto:

```
g++ -o sistema main.cpp ListaCompras.cpp Similaridade.cpp Recomendacao.cpp
```

## Como executar

O programa recebe o arquivo CSV, o número da atividade (1, 2 ou 3) e, depois, os dados
daquela atividade. Exemplos com os dados do professor (pasta `dados/`):

```
sistema dados/dados_venda_cluster_0.csv 1 99EIP501 99PXGH01 48743701
sistema dados/dados_venda_cluster_0.csv 2 153
sistema dados/dados_venda_cluster_0.csv 3 5 99EIP501 99PXGH01 48743701
```

- `1`: A1. Depois vêm os códigos originais dos clientes (sem códigos, usa os 3 primeiros).
- `2`: A2. Depois vem o índice do cliente (sem índice, usa o 0).
- `3`: A3. Depois vem o `k` e os códigos originais dos clientes (sem códigos, usa os 3 primeiros).

No Windows, use `.\sistema` no PowerShell.

## Observações sobre os dados

- Compras repetidas (mesmo cliente, mesmo produto) são contadas uma vez só, pois a
  matriz de compras só diz se o cliente comprou ou não.
- Algumas datas vieram corrompidas (`2,02E+07`) e viraram dois campos por causa da
  vírgula; a leitura detecta isso e continua normalmente.
- A matriz de similaridade densa usa muita memória (cerca de 12 bytes × n × n). Para
  clusters com poucos milhares de clientes funciona bem; os maiores (10 mil clientes)
  exigem a versão esparsa (Atividade 5).
