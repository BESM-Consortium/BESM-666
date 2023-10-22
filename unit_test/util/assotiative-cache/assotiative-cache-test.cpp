#include "besm-666/util/assotiative-cache.hpp"
#include <gtest/gtest.h>

using namespace besm;

class CacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        cache.add(5);
        cache.add(5);
        cache.add(5);
        cache.add(25);
        cache.add(42);
    }

    template <typename PayloadType, typename HashType>
    HashType myhash(PayloadType const &payload) {
        return std::hash<PayloadType>{}(payload);
    }

    template <typename PayloadType, typename TagType>
    TagType mytag(PayloadType const &payload) {
        return 2 * payload;
    }

    Cache<int, int, size_t, mytag, myhash> cache(2, 25);
};

TEST_F(CacheTest, kek) {
    using namespace besm::util;
    ASSERT_EQ(2, 1);
}