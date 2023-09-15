#include <gtest/gtest.h>

#include "besm-666/reg/rf.hpp"

TEST(gprf, read_write_test) {
    besm::reg::GPRF gprf;

    for (besm::RV64DWord i = 0; i < besm::reg::GPRF::Size; ++i) {
        gprf.write(i, i);
    }
    for (besm::RV64DWord i = 0; i < besm::reg::GPRF::Size; ++i) {
        EXPECT_EQ(gprf.read(i), i);
    }
}
