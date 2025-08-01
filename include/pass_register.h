#ifndef FRCC_PASS_REGISTER_H
#define FRCC_PASS_REGISTER_H

#include <unordered_set>
#include <vector>
#include <map>
#include <cassert>

#include "onnx/common/ir.h"
#include "onnx/common/ir_pb_converter.h"
#include "onnx/proto_utils.h"

#include "passes/vanilla_remove_identity_pass.h"

namespace frcc {

struct GlobalPassRegistry {
    std::map<std::string, std::shared_ptr<Pass>> passes;
    std::vector<std::string> passes_names;

    GlobalPassRegistry() {
        registerPass<VanillaRemoveIdentityPass>();
    }

    ~GlobalPassRegistry() {
        this->passes.clear();
    }

    std::shared_ptr<Pass> find(std::string pass_name) {
        auto iterator = this->passes.find(pass_name);
        assert(iterator != this->passes.end());
        return iterator->second;
    }

    const std::vector<std::string> GetAvailablePasses() {
        return passes_names;
    }

    template<typename T>
    void registerPass() {
        static_assert(std::is_base_of<Pass, T>::value, "T must inherit from Pass");
        std::shared_ptr<Pass> pass(new T());
        passes[pass->getPassName()] = pass;
        passes_names.emplace_back(pass->getPassName());
    }

};

}

#endif // FRCC_PASS_REGISTER_H