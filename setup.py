from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

modulo = Pybind11Extension(
    "sistema_recomendacao",
    ["bindings.cpp", "ListaCompras.cpp", "Similaridade.cpp", "Recomendacao.cpp"],
    cxx_std=14,
    define_macros=[("_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR", None)],
)

setup(
    name="sistema_recomendacao",
    ext_modules=[modulo],
    cmdclass={"build_ext": build_ext},
)