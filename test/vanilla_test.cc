#include "onnx/onnx_pb.h"
#include <fstream>
#include <iostream>

int main() {
    std::string filename = "yolos2.onnx"; // Replace with your ONNX model file path

    onnx::ModelProto model;
    std::ifstream in(filename, std::ios_base::binary);

    if (!in.is_open()) {
        std::cerr << "Error: Could not open ONNX model file: " << filename << std::endl;
        return 1;
    }

    if (!model.ParseFromIstream(&in)) {
        std::cerr << "Error: Failed to parse ONNX model from file: " << filename << std::endl;
        return 1;
    }

    in.close();

    std::cout << "ONNX model loaded successfully!" << std::endl;
    std::cout << "Model opset version: " << model.opset_import(0).version() << std::endl;
    std::cout << "Graph name: " << model.graph().name() << std::endl;
    // You can now access other properties of the model, such as inputs, outputs, nodes, etc.

    return 0;
}