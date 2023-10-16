#include <gtest/gtest.h>

#include "besm-666/util/range.hpp"

TEST(Range, EmptyRange) {
    using namespace besm::util;

    Range<int> r(0, 0);

    EXPECT_FALSE(r.contains(0));
}
