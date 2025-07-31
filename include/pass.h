#ifndef FRCC_PASS_H
#define FRCC_PASS_H

#include "onnx/common/ir.h"
#include <string>

namespace frcc{

// Base struct representing result of a pass.
struct PostPassAnalysis {
    virtual ~PostPassAnalysis() = default;
};

enum PassType {
    Fuse = 0,
};

// Enum that represents the return type of the analysis.
enum PassAnalysisType {
    // An empty analysis is returned. Most likely will return PostPassAnalysis.
    Empty = 0,
    // A count based analysis is returned. Most likely of type
    // CountBasedPassAnalysis
    CountBased = 1
};

enum PassEfficiency {
  // A partially efficient optimization pass cannot guarantee that running two
  // consecutive passes
  // will return the same result as running a single pass.
  Partial = 0,
  // A completely efficient optimization guarantees that running two consecutive
  // passes is equivalent
  // to running a single pass.
  Complete = 1
};

enum PassOptimizationType {
    // Is not optimizing anything. Most likely will be used in an immutable pass.
    None = 0,
    // Optimizes for compute.
    Compute = 1,
    // Optimizes for memory.
    Memory = 2,
    // Optimizes for both compute and memory.
    ComputeMemory = 3,
    // Optimizes for stability (e.g. log-sum-exp trick).
    Stability = 4
};

enum NodeDestroyType {
  // Does not destroy node
  DestroyFalse = 0,
  // Equivalent to calling it.destroyCurrent() once.
  DestroyTrue = 1,
};

// Base class for all optimizations within ONNX. A pass must contain the
// annotations described above. Furthermore each pass is given the ability to
// initialize and finalize it's pass. Each pass must have a unique name that
// pass managers/registry will use as identification. Finally the pass
// implements runPass which completes the pass inplace.
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

}

#endif //FRCC_PASS_H