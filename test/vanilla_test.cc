#include <gtest/gtest.h>
#include "pass.h"

TEST(compile, main) {
    std::cout << "Hello World from builded project structure" << std::endl;
    frcc::PassType obj = frcc::PassType::Fuse;
    std::cout << "Fuse PassType Obj created" << std::endl;
}