#include "onnx/onnx_pb.h"
#include <fstream>
#include <iostream>

void saveOnnxModel(const onnx::ModelProto& model_proto, const std::string& save_path) {
    std::fstream output(save_path, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output.is_open()) {
        // Handle error: file could not be opened
        return;
    }
    if (!model_proto.SerializeToOstream(&output)) {
        // Handle error: failed to serialize model
    }
    output.close();
}