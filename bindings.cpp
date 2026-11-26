#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <tuple>
#include "ListaCompras.h"
#include "Similaridade.h"
#include "Recomendacao.h"

namespace py = pybind11;

static ListaCompras lista;

static void carregar(vector<string> clientes, vector<string> produtos, vector<vector<int> > compras) {
    int i;

    liberarMatrizCompras(lista);

    lista.clientes = clientes;
    lista.produtos = produtos;
    lista.prodClientes = compras;

    for (i = 0; i < (int)clientes.size(); i++)
        lista.mapCliente[clientes[i]] = i;

    for (i = 0; i < (int)produtos.size(); i++)
        lista.mapProduto[produtos[i]] = i;

    criarMatrizCompras(lista);
    criarMatrizCSR(lista);
}

static void calcular_similaridade(int algoritmo) {
    int n = lista.clientes.size();

    if (algoritmo == 3) {
        calcularSimilaridadeCSR(lista);
    } else {
        double **S = calcularSimilaridade(lista, algoritmo);
        liberarMatrizDouble(S, n);
    }
}

static double similaridade(int i, int j) {
    int n = lista.clientes.size();

    if ((int)lista.matrizSimilaridadeCSR.row_ptr.size() != n + 1)
        calcularSimilaridadeCSR(lista);

    return similaridadeCSR(lista, i, j);
}

static vector<tuple<int, double> > recomendar_produtos(int cliente_id, int k) {
    int r;
    vector<item> recomendados = recomendar(lista, cliente_id, k);
    vector<tuple<int, double> > resultado;

    for (r = 0; r < (int)recomendados.size(); r++)
        resultado.push_back(make_tuple(recomendados[r].produto, recomendados[r].score));

    return resultado;
}

static void liberar() {
    liberarMatrizCompras(lista);
}

PYBIND11_MODULE(sistema_recomendacao, m) {
    m.def("carregar", &carregar, py::arg("clientes"), py::arg("produtos"), py::arg("compras"));
    m.def("calcular_similaridade", &calcular_similaridade, py::arg("algoritmo"));
    m.def("similaridade", &similaridade, py::arg("i"), py::arg("j"));
    m.def("recomendar_produtos", &recomendar_produtos, py::arg("cliente_id"), py::arg("k"));
    m.def("liberar", &liberar);
}
