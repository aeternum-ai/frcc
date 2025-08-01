#include "optimizer.h"

namespace frcc {

GlobalPassRegistry Optimizer::AvailablePasses;

Optimizer::Optimizer(std::vector<std::string>& pass_names) {
    this->pass_manager = std::shared_ptr<GeneralPassManager>(new GeneralPassManager());
    for (const auto& pass_name : pass_names) {
        auto pass = AvailablePasses.find(pass_name);
        this->pass_manager->add(pass);
    }
}

Optimizer::~Optimizer() {}

}