#include <pybind11/pybind11.h>
#include "MonteCarloPricer.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pricer_module, m)
{
    m.doc() = "Module de pricing Monte-Carlo en C++";

    py::class_<MonteCarloResult>(m, "MonteCarloResult")
        .def_readonly("estimated_value", &MonteCarloResult::estimated_value)
        .def_readonly("lower_bound", &MonteCarloResult::lower_bound)
        .def_readonly("upper_bound", &MonteCarloResult::upper_bound);

    py::class_<MonteCarloPricer>(m, "MonteCarloPricer")
        .def(py::init<int>(),
             py::arg("N"))
        .def("price_v1", &MonteCarloPricer::price_v1,
             py::arg("option"),
             py::arg("spot"),
             py::arg("r"),
             py::arg("sigma"))
        .def("price_v2", &MonteCarloPricer::price_v2,
             py::arg("option"),
             py::arg("spot"),
             py::arg("r"),
             py::arg("sigma"))
        .def("price_v3", &MonteCarloPricer::price_v3,
             py::arg("option"),
             py::arg("spot"),
             py::arg("r"),
             py::arg("sigma"))
        .def("price_v4", &MonteCarloPricer::price_v4,
             py::arg("option"),
             py::arg("spot"),
             py::arg("r"),
             py::arg("sigma"),
             py::arg("n"))
        .def("price_v5", &MonteCarloPricer::price_v5,
             py::arg("option"),
             py::arg("spot"),
             py::arg("r"),
             py::arg("sigma"),
             py::arg("m"));

    py::class_<Option>(m, "Option");

    py::class_<OptionCallAsiat, Option>(m, "OptionCallAsiat")
        .def(py::init<double, double>(),
             py::arg("T"),
             py::arg("strike"));
}
