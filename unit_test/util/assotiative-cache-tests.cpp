#include "besm-666/util/assotiative-cache.hpp"
#include <gtest/gtest.h>

using namespace besm::util;
template <typename PayloadType, typename HashType>
HashType fhash(PayloadType const &payload) {
    return std::hash<PayloadType>{}(payload);
}

template <typename PayloadType, typename TagType>
TagType ftag(PayloadType const &payload) {
    return 2 * payload;
}

class CacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        cache.add(5);
        cache.add(5);
        cache.add(5);
        cache.add(25);
        cache.add(42);
        cache.add(5);
    }

    Cache<int, int, size_t, ftag, fhash> cache{2, 25};
};

TEST_F(CacheTest, find) {
    using namespace besm::util;
    EXPECT_EQ(cache.find(5 * 2).getPayload(), 5);
    EXPECT_EQ(cache.find(25 * 2).getPayload(), 25);
    EXPECT_EQ(cache.find(42 * 2).getPayload(), 42);
}

TEST_F(CacheTest, valid) {
    using namespace besm::util;
    EXPECT_TRUE(cache.find(5 * 2).valid());
    EXPECT_TRUE(cache.find(25 * 2).valid());
    EXPECT_TRUE(cache.find(42 * 2).valid());
}

TEST_F(CacheTest, invalid) {
    using namespace besm::util;
    EXPECT_FALSE(cache.find(228).valid());
    EXPECT_FALSE(cache.find(1488).valid());
    EXPECT_FALSE(cache.find(404).valid());
}

TEST_F(CacheTest, counters) {
    using namespace besm::util;
    EXPECT_EQ(cache.getCounter(5 * 2 % 25), 0);
    EXPECT_EQ(cache.getCounter(25 * 2 % 25), 1);
    EXPECT_EQ(cache.getCounter(42 * 2 % 25), 1);
}