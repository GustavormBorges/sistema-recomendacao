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
| `ListaCompras.h/.cpp` | A1 | Lê o CSV e organiza clientes, produtos e a lista de compras de cada cliente. O `.h` também define as estruturas da matriz CSR |
| `Similaridade.h/.cpp` | A2, A4 e A5 | Monta a matriz de compras e calcula a similaridade entre clientes: versão densa (A2), multiplicação adaptada sem transposta (A4) e versão esparsa em CSR (A5) |
| `Recomendacao.h/.cpp` | A3 | Gera os `k` produtos mais bem ranqueados para um cliente, usando a similaridade em CSR |
| `TesteCSR.h/.cpp` | A5 | Testador que compara a versão densa com a CSR |
| `main.cpp` | A1 a A5 | Programa testador: um `switch` escolhe a atividade a executar |
| `bindings.cpp` | A6 | Liga o C++ ao Python com o pybind11: expõe a similaridade e a recomendação como o módulo `sistema_recomendacao` |
| `setup.py` | A6 | Compila o módulo Python com os arquivos C++ |
| `leitura_e_execucao.py` | A6 | Testador em Python: lê o CSV (A1 em Python) e chama o C++ através do módulo |

Os módulos não têm `main`: o programa testador fica em um arquivo separado, o `main.cpp`.
O `main.cpp` e o `TesteCSR.cpp` não fazem parte do módulo Python da A6.

## Como compilar

Precisa do `g++`. Na pasta do projeto:

```
g++ -o sistema main.cpp ListaCompras.cpp Similaridade.cpp Recomendacao.cpp TesteCSR.cpp
```

## Como executar

O programa recebe o arquivo CSV, o número da atividade (1 a 5) e, depois, os dados
daquela atividade. Se algum dado não for digitado, o programa pergunta na hora.
Exemplos com os dados do professor (pasta `dados/`):

```
sistema dados/dados_venda_cluster_0.csv 1 99EIP501 99PXGH01 48743701
sistema dados/dados_venda_cluster_0.csv 2 153
sistema dados/dados_venda_cluster_0.csv 3 5 99EIP501 99PXGH01 48743701
sistema dados/dados_venda_cluster_0.csv 4 2
sistema dados/dados_venda_cluster_0.csv 5
```

- `1`: A1. Depois vêm os códigos originais dos clientes. Sem códigos, o programa pergunta
  um código (ENTER usa os 3 primeiros).
- `2`: A2. Depois vem o índice do cliente. Sem índice, o programa pergunta (ENTER usa o
  0). Usa a matriz densa.
- `3`: A3. Depois vem o `k` (quantos produtos recomendar) e os códigos originais dos
  clientes. O que faltar, o programa pergunta (ENTER usa os 3 primeiros clientes). A
  similaridade vem da matriz CSR.
- `4`: A4 e A5. Mede o tempo de resposta do cálculo da similaridade. Depois vem o
  algoritmo: `1` padrão (com transposta), `2` adaptado (sem transposta, aproveitando a
  simetria) ou `3` CSR. Sem número, o programa pergunta (ENTER roda os três, aponta o
  mais rápido e confere se o padrão e o adaptado dão o mesmo resultado).
- `5`: A5. Compara a matriz densa com a CSR: mostra quantos elementos cada matriz
  guarda, o tempo médio de cada versão e se os resultados são iguais.

As opções 4 e 5 repetem os cálculos várias vezes para tirar a média, então podem demorar
nos clusters maiores.

No Windows, use `.\sistema` no PowerShell.

## Atividade 6: integração com Python

O testador em Python (`leitura_e_execucao.py`) lê o CSV e organiza clientes, produtos e
compras (A1 em Python). A similaridade e a recomendação continuam sendo calculadas pelo
código C++ das atividades 2 a 5, ligado ao Python pelo módulo `sistema_recomendacao`.

Funções expostas ao Python (`bindings.cpp`):

| Função | O que faz |
|---|---|
| `carregar(clientes, produtos, compras)` | Guarda os dados e monta as matrizes de compras. `compras[i]` é a lista de índices dos produtos do cliente `i` |
| `calcular_similaridade(algoritmo)` | Calcula a similaridade: `1` padrão, `2` adaptado ou `3` CSR |
| `similaridade(i, j)` | Valor da similaridade entre os clientes `i` e `j` (menor = mais parecido) |
| `recomendar_produtos(cliente_id, k)` | Lista de `(produto, score)` com os `k` produtos recomendados |
| `liberar()` | Libera a memória das matrizes |

### Como preparar e compilar

É preciso ter o Python e o compilador C++ da Microsoft (Microsoft C++ Build Tools, com a
opção "Desenvolvimento para desktop com C++"). Depois, na pasta do projeto:

```
pip install pybind11 setuptools wheel
python setup.py build_ext --inplace
```

Isso cria o módulo (no Windows, um arquivo `sistema_recomendacao.cp313-win_amd64.pyd`, com
o número variando conforme a versão do Python). Se der erro de compilador, abra o
"x64 Native Tools Command Prompt for Visual Studio" ou o "Developer PowerShell for Visual
Studio" e rode os comandos de lá.

### Como executar

O programa recebe o arquivo CSV, a opção (1 a 4) e os dados daquela opção. O que não for
digitado, o programa pergunta, inclusive o arquivo e o algoritmo:

```
python leitura_e_execucao.py dados/dados_venda_cluster_0.csv 1 99EIP501 99PXGH01 48743701
python leitura_e_execucao.py dados/dados_venda_cluster_0.csv 2 153
python leitura_e_execucao.py dados/dados_venda_cluster_0.csv 3 5 99EIP501 99PXGH01 48743701
python leitura_e_execucao.py dados/dados_venda_cluster_0.csv 4 2
python leitura_e_execucao.py
```

- `1`: mostra a lista de compras de clientes (ENTER mostra os 3 primeiros).
- `2`: mostra o cliente e o mais similar a ele (ENTER usa o cliente 0).
- `3`: recomenda `k` produtos para clientes (ENTER usa os 3 primeiros).
- `4`: mede o tempo médio (5 repetições) do cálculo da similaridade. O algoritmo pode ser
  `1`, `2` ou `3`; sem número, o programa pergunta e ENTER roda os três.

## Observações sobre os dados

- Compras repetidas (mesmo cliente, mesmo produto) são contadas uma vez só, pois a
  matriz de compras só diz se o cliente comprou ou não.
- A matriz de similaridade densa usa muita memória (cerca de 12 bytes × n × n). Para
  clusters com poucos milhares de clientes funciona bem, mas os maiores (10 mil clientes)
  passam de 1 GB.
- A matriz de compras é bem esparsa: nos clusters testados, menos de 5% dos elementos
  são diferentes de zero, então a CSR economiza bastante memória nela. Já as matrizes de
  interseções e de similaridade ficam bem preenchidas, porque nos clusters testados
  todo par de clientes tem pelo menos um produto em comum. Nelas o ganho da CSR é
  principalmente de tempo, e a vantagem cresce com o tamanho do cluster.
