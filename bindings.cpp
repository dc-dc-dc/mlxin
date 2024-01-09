#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "image.h"

namespace py = pybind11;
using namespace py::literals;

PYBIND11_MODULE(mlxin, m) {
    m.doc() = "mlx_in module for direct loading of data";
    
    m.def(
        "load_jpeg", 
        &load_jpeg, 
        "path"_a, 
        py::pos_only(),
        py::kw_only(),
        R"pbdoc(
            load_jpeg(path: str) -> array
        )pbdoc"
    );

    m.def(
        "load_png",
        &load_png,
        "path"_a,
        py::pos_only(),
        py::kw_only(),
        R"pbdoc(
            load_png(path: str) -> array
        )pbdoc"
    );
}