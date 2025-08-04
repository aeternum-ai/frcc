#include "optimizer.h"

namespace frcc {

GlobalPassRegistry Optimizer::AvailablePasses;

Optimizer::Optimizer(const std::vector<std::string>& pass_names) {
    this->pass_manager = std::shared_ptr<GeneralPassManager>(new GeneralPassManager());
    for (const auto& pass_name : pass_names) {
        auto pass = AvailablePasses.find(pass_name);
        this->pass_manager->add(pass);
    }
}

Optimizer::~Optimizer() {}

onnx::ModelProto Optimize(
    const onnx::ModelProto& mp_in,
    const std::vector<std::string>& names,
    const bool &inference_shape) {
  Optimizer current_opt(names);
  return current_opt.optimize(mp_in, inference_shape);
}

const std::vector<std::string> GetAvailablePasses() {
  return Optimizer::AvailablePasses.GetAvailablePasses();
}

}