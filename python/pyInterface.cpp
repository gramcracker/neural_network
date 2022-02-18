#include <net.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include <string>



// void netWrapper(py::array_t<int>& layer_size){
    
// }

using namespace::std;
namespace py = pybind11;
PYBIND11_MODULE(pyneuralnetwork, m) {
    py::class_<net>(m, "net")
    .def(py::init<>())
    .def("addNeurons", py::overload_cast<int , int , std::string >(&net::AddNeurons))
    .def("size", py::overload_cast<>(&net::get_size))
    .def("size", py::overload_cast<int>(&net::get_size))
    .def("output", py::overload_cast<>(&net::Output))
    .def("output", py::overload_cast<int, int>(&net::Output))
    .def("connect", py::overload_cast<int, int, int, int, float> (&net::Connect))
    .def("setVerboseMode", &net::SetVerboseMode)
    .def("fullyConnect", py::overload_cast<>(&net::FullyConnect))  
    .def("fullyConnect", py::overload_cast<int, int>(&net::FullyConnect))
    .def("fullyConnect", py::overload_cast<int, int>(&net::FullyConnect))
    .def("fullyConnect", py::overload_cast<int, int, int, int, int, int>(&net::FullyConnect))
    .def("fullyConnect", py::overload_cast<net*, int, int>(&net::FullyConnect))
    .def("getWeights", py::overload_cast<>(&net::GetWeights));
    }