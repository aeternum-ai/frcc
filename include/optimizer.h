#ifndef FRCC_OPTIMIZER_H
#define FRCC_OPTIMIZER_H

#include "onnx/common/ir.h"
#include "onnx/common/ir_pb_converter.h"
#include "onnx/proto_utils.h"

#include "pass_manager.h"
#include "pass_register.h"

namespace frcc {

class Optimizer {
public:
    Optimizer(std::vector<std::string>& pass_names);
    ~Optimizer();

    onnx::ModelProto optimize(const onnx::ModelProto& _mp_in) {
        onnx::ModelProto mp_in = _mp_in;
        if (mp_in.ir_version() <= 3) {
            mp_in.set_ir_version(4);
            std::cerr << "Warning: IR version off model too old " 
                      << "automaticly change IR to version 4, "
                      << "unstable operation is possible" << std::endl;
        }

        std::shared_ptr<onnx::Graph> g = onnx::ImportModelProto(mp_in);

        if (g.get() == nullptr) {
            std::cerr << "Warning: frcc is unable to parse input model. "
                << "(The IR version of the ONNX model may be too old.)"
                << std::endl;
            return mp_in;
        }

        onnx::ModelProto mp_out = onnx::PrepareOutput(mp_in);
        this->pass_manager->run(*g);
        onnx::ExportModelProto(&mp_out, g);

        return mp_out;
    }

private:
    static GlobalPassRegistry AvailablePasses;
    std::shared_ptr<PassManager> pass_manager;
};

}

#endif // FRCC_OPTIMIZER_H