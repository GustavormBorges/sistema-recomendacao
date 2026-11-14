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
| `teste_lista.cpp` | A1 | Testador: mostra os produtos comprados por clientes |
| `teste_similaridade.cpp` | A2 | Testador: mostra o cliente mais similar a um cliente |
| `teste_recomendacao.cpp` | A3 | Testador: mostra as recomendações para clientes |

## Como compilar

Precisa do `g++`. Na pasta do projeto:

```
g++ -o teste_lista teste_lista.cpp ListaCompras.cpp
g++ -o teste_similaridade teste_similaridade.cpp ListaCompras.cpp Similaridade.cpp
g++ -o teste_recomendacao teste_recomendacao.cpp ListaCompras.cpp Similaridade.cpp Recomendacao.cpp
```

## Como executar

Exemplos de uso com os dados do professor (pasta `dados/`):

```
teste_lista dados/dados_venda_cluster_0.csv 99EIP501 99PXGH01 48743701
teste_similaridade dados/dados_venda_cluster_0.csv 153
teste_recomendacao dados/dados_venda_cluster_0.csv 5 99EIP501 99PXGH01 48743701
```

No Windows, use `teste_lista.exe` etc. (ou `.\teste_lista`).

## Observações sobre os dados

- Compras repetidas (mesmo cliente, mesmo produto) são contadas uma vez só, pois a
  matriz de compras só diz se o cliente comprou ou não.
- Algumas datas vieram corrompidas (`2,02E+07`) e viraram dois campos por causa da
  vírgula; a leitura detecta isso e continua normalmente.
- A matriz de similaridade densa usa muita memória (cerca de 12 bytes × n × n). Para
  clusters com poucos milhares de clientes funciona bem; os maiores (10 mil clientes)
  exigem a versão esparsa (Atividade 5).
