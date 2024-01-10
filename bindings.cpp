#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "file.h"
#include "image.h"

namespace py = pybind11;
using namespace py::literals;

PYBIND11_MODULE(mlxin, m)
{
    m.doc() = "mlx_in module for direct loading of data";

    m.def(
        "load_jpeg",
        &load_jpeg,
        "path"_a,
        py::pos_only(),
        py::kw_only(),
        R"pbdoc(
            load_jpeg(path: str) -> array
        )pbdoc");

    m.def(
        "load_png",
        &load_png,
        "path"_a,
        py::pos_only(),
        py::kw_only(),
        R"pbdoc(
            load_png(path: str) -> array
        )pbdoc");

    m.def(
        "save_png",
        &save_png,
        "path"_a,
        "img"_a,
        py::pos_only(),
        py::kw_only(),
        R"pbdoc(
            save_png(path: str, data: array) -> None
        )pbdoc"
    );

    m.def(
        "load_csv",
        &load_csv,
        "path"_a,
        py::pos_only(),
        py::kw_only(),
        R"pbdoc(
            load_csv(path: str) -> array
        )pbdoc");

    m.def(
        "save_csv",
        &save_csv,
        "path"_a,
        "header"_a,
        "data"_a,
        py::pos_only(),
        py::kw_only(),
        R"pbdoc(
            save_csv(path: str, header: List[str], data: array) -> None
        )pbdoc");
}