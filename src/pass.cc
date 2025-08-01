#include "pass.h"

namespace frcc {

Pass::Pass(
    PassType pass_type,
    PassEfficiency pass_efficiency,
    PassOptimizationType pass_optimization_type
) {
    this->pass_type = pass_type;
    this->pass_efficiency = pass_efficiency;
    this->pass_optimization_type = pass_optimization_type;
}

Pass::~Pass() {}

PatternMatchingPass::~PatternMatchingPass() {}

PassAnalysisType PatternMatchingPass::getPassAnalysisType() const {
    return PassAnalysisType::CountBased;
}

unsigned int PatternMatchingPass::runPassInternal(onnx::Graph& graph) {
    unsigned int num_changes = 0;
    for (auto it = graph.begin(); it != graph.end(); it++) {
        auto* n = *it;
        if (this->patternMatchPredicate(n)) {
            NodeDestroyType node_destroy = NodeDestroyType::DestroyFalse;
            num_changes += this->runTransform(n, graph, node_destroy);

            if (node_destroy == NodeDestroyType::DestroyTrue) {
                it.destroyCurrent();
            }
        }
    }
    return num_changes;
}

std::shared_ptr<PostPassAnalysis> PatternMatchingPass::runPass(onnx::Graph &graph) {
    bool initialization_done = this->initializePass(graph);
    unsigned int num_positive_transforms = this->runPassInternal(graph);
    bool finalization_done = this->finalizePass(graph);

    return std::shared_ptr<PostPassAnalysis>(new CountBasedPassAnalysis(this, 
        num_positive_transforms, initialization_done, finalization_done));
}


CountBasedPassAnalysis::CountBasedPassAnalysis(
    Pass* pass,
    unsigned int num_positive_transforms,
    bool initialization_done,
    bool finalization_done) {
  this->pass = pass;
  this->num_positive_transforms = num_positive_transforms;
  this->initialization_done = initialization_done;
  this->finalization_done = finalization_done;
}

}