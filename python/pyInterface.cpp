#include <net.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include <string>


using namespace::std;
namespace py = pybind11;
PYBIND11_MODULE(pyneuralnetwork, m) {
    py::class_<net>(m, "net")
    .def(py::init<>())
    .def("addNeurons", py::overload_cast<int , int , std::string >(&net::AddNeurons))
    .def("size", py::overload_cast<>(&net::get_size));
}