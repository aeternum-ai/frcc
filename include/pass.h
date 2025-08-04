#ifndef FRCC_PASS_H
#define FRCC_PASS_H

#include "onnx/common/ir.h"
#include <string>

namespace frcc{


struct PostPassAnalysis {
    virtual ~PostPassAnalysis() = default;
};

enum PassType {
    Fuse = 0,
    Nop = 1,
    Other = 2
};

enum PassAnalysisType {
    Empty = 0,
    CountBased = 1
};

enum PassEfficiency {
    Partial = 0,
    Complete = 1
};

enum PassOptimizationType {
    None = 0,
    Compute = 1,
    Memory = 2,
    ComputeMemory = 3,
    Stability = 4
};

enum NodeDestroyType {
    DestroyFalse = 0,
    DestroyTrue = 1,
};

class Pass {
    PassType pass_type;
    PassEfficiency pass_efficiency;
    PassOptimizationType pass_optimization_type;

 public:
    Pass(PassType pass_type, PassEfficiency pass_efficiency,
        PassOptimizationType pass_optimization_type);
    virtual ~Pass();

    PassType getPassType() const {
    return this->pass_type;
    }
    PassEfficiency getPassEfficiency() const {
    return this->pass_efficiency;
    }
    PassOptimizationType getPassOptimizationType() const {
    return this->pass_optimization_type;
    }
    virtual PassAnalysisType getPassAnalysisType() const = 0;
    virtual std::string getPassName() const = 0;

    virtual bool initializePass(onnx::Graph &) {
    return false;
    }
    virtual bool finalizePass(onnx::Graph &) {
    return false;
    }
    virtual std::shared_ptr<PostPassAnalysis> runPass(onnx::Graph &graph) = 0;
};

class PatternMatchingPass : public Pass {
public:
    explicit PatternMatchingPass(PassType pass_type, 
        PassEfficiency pass_efficiency,
        PassOptimizationType pass_optimization_type) :
        Pass(pass_type, pass_efficiency, pass_optimization_type) {}

    ~PatternMatchingPass() override;

    virtual bool patternMatchPredicate(onnx::Node *node) = 0;
    virtual bool runTransform(onnx::Node *node, onnx::Graph &graph,
                            NodeDestroyType &destroy_current) = 0;

    std::shared_ptr<PostPassAnalysis> runPass(onnx::Graph &graph) override;

    PassAnalysisType getPassAnalysisType() const override;

private:
    unsigned int runPassInternal(onnx::Graph& graph);
};

class FullGraphBasedPass : public Pass {
 public:
  explicit FullGraphBasedPass(PassType pass_type,
                              PassEfficiency pass_efficiency,
                              PassOptimizationType pass_optimization_type)
      : Pass(pass_type, pass_efficiency, pass_optimization_type) {}
  ~FullGraphBasedPass() override;
};


struct CountBasedPassAnalysis : PostPassAnalysis {
    Pass *pass;
    unsigned int num_positive_transforms;
    bool initialization_done;
    bool finalization_done;

public:
    explicit CountBasedPassAnalysis(Pass *pass,
                                    unsigned int num_positive_transforms,
                                    bool initialization_done,
                                    bool finalization_done);

    bool graphChanged() {
    return this->num_positive_transforms > 0;
    }
    bool numSucceededTransforms() {
    return this->num_positive_transforms;
    }
};

inline bool areTwoValuesBothInputOrOutput(const onnx::Value *value1,
                                          const onnx::Value *value2) {
  const auto IsInputOrOutput = [](const onnx::Value *value) {
    const auto graph = value->owningGraph();
    const bool is_output =
        std::find(graph->outputs().rbegin(), graph->outputs().rend(), value) !=
        graph->outputs().rend();
    const bool is_input =
        value->node()->kind() == onnx::kCaptured ||
        std::find(graph->inputs().rbegin(), graph->inputs().rend(), value) !=
            graph->inputs().rend();
    return is_output || is_input;
  };
  return IsInputOrOutput(value1) && IsInputOrOutput(value2);
}

inline bool tryReplacingAllUsesWith(onnx::Value *oldValue, onnx::Value *newValue) {
  if (areTwoValuesBothInputOrOutput(oldValue, newValue)) {
    return false;
  }
  oldValue->replaceAllUsesWith(newValue);
  return true;
}

inline bool tryReplacingAllUsesWith(onnx::Node *oldNode, onnx::Node *newNode) {
  assert(oldNode->outputs().size() == newNode->outputs().size());
  size_t nOutputs = oldNode->outputs().size();
  for (size_t i = 0; i < nOutputs; i++) {
    const auto *oldValue = oldNode->outputs()[i];
    const auto *newValue = newNode->outputs()[i];
    if (areTwoValuesBothInputOrOutput(oldValue, newValue)) {
      return false;
    }
  }
  oldNode->replaceAllUsesWith(newNode);
  return true;
}

}

#endif //FRCC_PASS_H