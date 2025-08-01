#ifndef FRCC_PASS_MANAGER_H
#define FRCC_PASS_MANAGER_H

#include <vector>
#include "pass.h"

namespace frcc {

struct PassManagerAnalysis {};
struct EmptyPassManagerAnalysis : PassManagerAnalysis {};


class PassManager {
 public:
  PassManager();
  virtual ~PassManager();

  virtual void add(std::shared_ptr<Pass> P) = 0;
  virtual std::shared_ptr<PassManagerAnalysis> run(onnx::Graph& graph) = 0;
};


class GeneralPassManager : public PassManager {
 public:
  GeneralPassManager();
  ~GeneralPassManager() override;

  void add(std::shared_ptr<Pass> pass) override;
  std::shared_ptr<PassManagerAnalysis> run(onnx::Graph& graph) override;

 protected:
  std::vector<std::shared_ptr<Pass>> passes;
};

}

#endif // FRCC_PASS_MANAGER_H