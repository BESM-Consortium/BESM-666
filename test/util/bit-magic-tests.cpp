#include <gtest/gtest.h>
#include <limits>
#include <locale>
#include <numeric>

#include "besm-666/util/bit-magic.hpp"

TEST(SUConverter, UtoS) {
    unsigned int a = std::numeric_limits<unsigned int>::max();

    besm::util::SUConverter<unsigned int> c = {.u = a};

    EXPECT_EQ(c.s, -1);
}

TEST(SUConverter, StoU) {
    signed int a = std::numeric_limits<signed int>::max();

    besm::util::SUConverter<signed int> c = {.s = a};

    EXPECT_EQ(c.u, (1u << (sizeof(unsigned int) * 8 - 1)) - 1);
}

TEST(SignExtend, PositiveValue) {
    unsigned int v = 3; // 011

    unsigned int r = besm::util::SignExtend<unsigned int, 3>(v);

    EXPECT_EQ(r, 3u);
}

TEST(SignExtend, NegativeValue) {
    unsigned int v = 7; // 111 (-1)

    unsigned int r = besm::util::SignExtend<unsigned int, 3>(v);

    EXPECT_EQ(r, std::numeric_limits<unsigned int>::max());
}

TEST(Signify, PositiveValue) {
    unsigned int v = 3;

    signed int r = besm::util::Signify(v);

    EXPECT_EQ(r, 3);
}

TEST(Signify, NegativeValue) {
    unsigned int v = std::numeric_limits<unsigned int>::max();

    signed int r = besm::util::Signify(v);

    EXPECT_EQ(r, -1);
}

TEST(Unsignify, PositiveValue) {
    signed int v = 3;

    unsigned int r = besm::util::Unsignify(v);

    EXPECT_EQ(r, 3u);
}

TEST(Unsignify, NegativeValue) {
    signed int v = -1;

    unsigned int r = besm::util::Unsignify(v);

    EXPECT_EQ(r, std::numeric_limits<unsigned int>::max());
}

TEST(ExtractBits, Dummy) {
    unsigned int v = 0b00111101111;

    unsigned int r = besm::util::ExtractBits<unsigned int, 4, 5>(v);

    EXPECT_EQ(r, 0b1111);
}
