#ifndef ELIMINATE_IDENTITY_PASS_H
#define ELIMINATE_IDENTITY_PASS_H

#include "pass.h"

namespace frcc {

struct EliminateIdentityPass final : public PatternMatchingPass {
    explicit EliminateIdentityPass() 
        : PatternMatchingPass(PassType::Nop, 
            PassEfficiency::Complete, PassOptimizationType::Compute) {}

    std::string getPassName() const override {
        return "eliminate_identity";
    }

    bool patternMatchPredicate(onnx::Node *node) override {
        return node->kind() == onnx::kIdentity;
    }

    bool runTransform(onnx::Node *node, onnx::Graph &graph,
                            NodeDestroyType &destroy_current) override {
        const bool replacing_success =
        tryReplacingAllUsesWith(node->output(), node->input());
        if (!replacing_success) 
            return false;
        destroy_current = NodeDestroyType::DestroyTrue;
        return true;
    }

};

}

#endif // VANILLA_REMOVE_IDENTITY_PASS_H