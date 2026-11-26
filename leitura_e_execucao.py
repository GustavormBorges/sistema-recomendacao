import sys
import time

import sistema_recomendacao

REPETICOES = 5

clientes = []
produtos = []
cod_cliente = {}
cod_produto = {}
compras = []


# A1 em Python: le o CSV e guarda clientes, produtos e as compras de cada cliente
def ler_dados(caminho):
    arquivo = open(caminho, "r")
    arquivo.readline()

    for linha in arquivo:
        campos = linha.rstrip("\r\n").split(",", 3)

        if len(campos) < 4:
            continue

        cliente = campos[1]
        produto = campos[2]
        nome = campos[3]

        if cliente not in cod_cliente:
            cod_cliente[cliente] = len(clientes)
            clientes.append(cliente)
            compras.append([])

        if produto not in cod_produto:
            cod_produto[produto] = len(produtos)
            produtos.append(nome)

        c = cod_cliente[cliente]
        p = cod_produto[produto]

        if p not in compras[c]:
            compras[c].append(p)

    arquivo.close()


def imprimir_compras(i):
    print(f"Cliente {i} (codigo {clientes[i]}) comprou {len(compras[i])} produto(s):")

    for p in compras[i]:
        print(f"   - {produtos[p]}")


def mostrar_cliente(codigo):
    if codigo not in cod_cliente:
        print(f"Cliente {codigo} nao encontrado.")
    else:
        imprimir_compras(cod_cliente[codigo])


def testar_lista(argumentos):
    if len(argumentos) > 0:
        for codigo in argumentos:
            mostrar_cliente(codigo)
        return

    codigo = input("Codigo do cliente (ENTER para ver os 3 primeiros): ")
    print()

    if codigo == "":
        for i in range(3):
            imprimir_compras(i)
    else:
        mostrar_cliente(codigo)


def testar_similaridade(argumentos):
    n = len(clientes)
    i = 0

    if len(argumentos) > 0:
        i = int(argumentos[0])
    else:
        texto = input("Indice do cliente (ENTER para o cliente 0): ")
        print()

        if texto != "":
            i = int(texto)

    if i < 0 or i >= n:
        print(f"Indice invalido. Use um valor de 0 a {n - 1}")
        return

    print("Calculando a similaridade entre os clientes...")
    print()

    mais_similar = -1
    for j in range(n):
        s = sistema_recomendacao.similaridade(i, j)

        if j != i and s < 1.0:
            if mais_similar == -1 or s < sistema_recomendacao.similaridade(i, mais_similar):
                mais_similar = j

    imprimir_compras(i)
    print()

    if mais_similar == -1:
        print(f"Nenhum outro cliente comprou o mesmo que o cliente {i}.")
    else:
        s = sistema_recomendacao.similaridade(i, mais_similar)
        print(f"Cliente mais similar (s = {s:g}; quanto menor, mais parecido):")
        imprimir_compras(mais_similar)


def mostrar_recomendacao(i, k):
    imprimir_compras(i)

    recomendados = sistema_recomendacao.recomendar_produtos(i, k)

    if len(recomendados) == 0:
        print("Nenhum produto para recomendar (nenhum cliente parecido).")
    else:
        print(f"Produtos recomendados (ate {k}; quanto menor o valor, mais indicado):")

        for r in range(len(recomendados)):
            produto, score = recomendados[r]
            print(f"   {r + 1}. {produtos[produto]}  (valor {score:g})")

    print()


def recomendar_por_codigo(codigo, k):
    if codigo not in cod_cliente:
        print(f"Cliente {codigo} nao encontrado.")
        print()
    else:
        mostrar_recomendacao(cod_cliente[codigo], k)


def testar_recomendacao(argumentos):
    if len(argumentos) > 0:
        k = int(argumentos[0])
    else:
        k = int(input("Quantos produtos recomendar (k)? "))

    if k < 1:
        print("k invalido. Use um numero maior que 0.")
        return

    codigo = ""
    if len(argumentos) <= 1:
        codigo = input("Codigo do cliente (ENTER para os 3 primeiros): ")
        print()

    print("Calculando a similaridade entre os clientes...")
    print()
    sistema_recomendacao.calcular_similaridade(3)

    if len(argumentos) > 1:
        for codigo in argumentos[1:]:
            recomendar_por_codigo(codigo, k)
    elif codigo == "":
        for i in range(3):
            mostrar_recomendacao(i, k)
    else:
        recomendar_por_codigo(codigo, k)


def medir_algoritmo(algoritmo):
    soma = 0.0

    for repeticao in range(REPETICOES):
        inicio = time.perf_counter()
        sistema_recomendacao.calcular_similaridade(algoritmo)
        fim = time.perf_counter()

        soma = soma + (fim - inicio)

    return soma / REPETICOES


def testar_desempenho(argumentos):
    nomes = ["", "Padrao (com transposta)", "Adaptado (sem transposta)", "CSR (matriz esparsa)"]

    if len(argumentos) > 0:
        algoritmo = int(argumentos[0])
    else:
        texto = input("Algoritmo (1 = padrao, 2 = adaptado, 3 = CSR, ENTER = todos): ")
        print()

        algoritmo = 0
        if texto != "":
            algoritmo = int(texto)

    if algoritmo < 0 or algoritmo > 3:
        print("Algoritmo invalido. Use 1, 2 ou 3.")
        return

    primeiro = 1
    ultimo = 3
    if algoritmo != 0:
        primeiro = algoritmo
        ultimo = algoritmo

    print("Tempo de resposta do calculo da similaridade")
    print(f"Clientes: {len(clientes)}")
    print(f"Produtos: {len(produtos)}")
    print(f"Repeticoes: {REPETICOES}")
    print()

    mais_rapido = primeiro
    menor_tempo = -1.0

    for a in range(primeiro, ultimo + 1):
        tempo = medir_algoritmo(a)

        print(f"Algoritmo {a} - {nomes[a]}: {tempo:.6f} segundos")

        if menor_tempo < 0 or tempo < menor_tempo:
            menor_tempo = tempo
            mais_rapido = a

    if algoritmo == 0:
        print()
        print(f"Mais rapido: algoritmo {mais_rapido} - {nomes[mais_rapido]}")


def main():
    if len(sys.argv) > 1:
        caminho = sys.argv[1]
    else:
        caminho = input("Arquivo CSV (ex.: dados/dados_venda_cluster_17.csv): ")

    if len(sys.argv) > 2:
        opcao = int(sys.argv[2])
    else:
        print("1 = mostrar clientes, 2 = cliente mais similar, 3 = recomendar, 4 = tempo")
        opcao = int(input("Opcao: "))

    try:
        ler_dados(caminho)
    except FileNotFoundError:
        print(f"Erro ao abrir o arquivo {caminho}")
        return

    sistema_recomendacao.carregar(clientes, produtos, compras)

    print(f"{len(clientes)} clientes e {len(produtos)} produtos carregados.")
    print()

    argumentos = sys.argv[3:]

    if opcao == 1:
        testar_lista(argumentos)
    elif opcao == 2:
        testar_similaridade(argumentos)
    elif opcao == 3:
        testar_recomendacao(argumentos)
    elif opcao == 4:
        testar_desempenho(argumentos)
    else:
        print("Opcao invalida (use 1, 2, 3 ou 4)")

    sistema_recomendacao.liberar()


main()
