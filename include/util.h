#ifndef FRCC_UTIL_H
#define FRCC_UTIL_H

#include "onnx/onnx_pb.h"

namespace frcc {

void loadModel(onnx::ModelProto* m, const std::string& model_path,
               const bool load_external_data = false);

void saveModel(onnx::ModelProto* m, const std::string& model_path,
               const bool save_external_data = false,
               const std::string& data_file_name = {});

}

#endif // FRCC_UTIL_H