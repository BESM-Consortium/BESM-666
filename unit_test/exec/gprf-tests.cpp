#include <gtest/gtest.h>

#include "besm-666/exec/gprf.hpp"

TEST(gprf, read_write_test) {
    besm::exec::GPRF gprf;

    for (besm::RV64DWord i = 0; i < besm::exec::GPRF::Size; ++i) {
        gprf.write(i, i);
    }
    for (besm::RV64DWord i = 0; i < besm::exec::GPRF::Size; ++i) {
        EXPECT_EQ(gprf.read(i), i);
    }
}
