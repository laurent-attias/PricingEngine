import pybind11
from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

sources = [
    "src/bindings.cpp",
    "src/Option.cpp",
    "src/MonteCarloPricer.cpp",
    "src/MathUtils.cpp"
]

ext_modules = [
    Extension(
        "pricer_module",
        sources,
        include_dirs=["pricer/include", pybind11.get_include()],
        language="c++",
        extra_compile_args=["-std=c++20", "-Wa,-mbig-obj", "-O3"],
    ),
]

setup(
    name="pricer_module",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext}
)
