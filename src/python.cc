#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "onnx/py_utils.h"
#include "util.h"
#include "optimizer.h"

namespace py = pybind11;
using namespace pybind11::literals;
PYBIND11_MODULE(frcc_cpp2py_export, frcc_cpp2py_export) {
  frcc_cpp2py_export.doc() = "Flow Rider Compiler Collection";

  frcc_cpp2py_export.def(
      "optimize",
      [](const py::bytes& bytes, const std::vector<std::string>& names, const bool inference_shape=true) {
        onnx::ModelProto proto{};
        onnx::ParseProtoFromPyBytes(&proto, bytes);
        auto const result = frcc::Optimize(proto, names, inference_shape);
        std::string out;
        result.SerializeToString(&out);
        return py::bytes(out);
      });

  frcc_cpp2py_export.def(
      "optimize_from_path", [](const std::string& import_model_path,
                               const std::string& export_model_path,
                               const std::vector<std::string>& names,
                               const std::string& export_data_file_name,
                               const bool inference_shape=true) {
        onnx::ModelProto proto{};
        frcc::loadModel(&proto, import_model_path, true);
        auto result = frcc::Optimize(proto, names, inference_shape);
        frcc::saveModel(&result, export_model_path, true,
                                export_data_file_name);
      });

  frcc_cpp2py_export.def("get_available_passes",
                             &frcc::GetAvailablePasses);
}