#include "pass_manager.h"

namespace frcc {

PassManager::PassManager() {}
PassManager::~PassManager() {}

GeneralPassManager::GeneralPassManager() {}
GeneralPassManager::~GeneralPassManager() {
    this->passes.clear();
}

void GeneralPassManager::add(std::shared_ptr<Pass> pass) {
    this->passes.push_back(std::move(pass));
}

std::shared_ptr<PassManagerAnalysis> GeneralPassManager::run(onnx::Graph& graph) {
    for (const std::shared_ptr<Pass>& pass : this->passes) {
        auto PassAnalysis = pass->runPass(graph);
    }
    return std::shared_ptr<PassManagerAnalysis> (new EmptyPassManagerAnalysis());
}

}