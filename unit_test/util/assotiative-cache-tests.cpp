#include "besm-666/riscv-types.hpp"
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

constexpr size_t TLBSets = 64;
constexpr size_t TLBWays = 4;

struct TLBEntry {
    // page id = page address / page size
    besm::RV64Size virtualPageId;
    besm::RV64Size physicalPageId;
};

besm::RV64Ptr TLBTag(TLBEntry const& entry) {
    return entry.virtualPageId;
}

besm::RV64Ptr TLBHash(TLBEntry const& entry) {
    return entry.virtualPageId & (TLBSets - 1);
}

TEST(AssotiativeCache, ComplexPayload) {
    using TLBCache = besm::util::Cache<TLBEntry, besm::RV64Ptr, besm::RV64Ptr, 
          TLBTag, TLBHash> ;

    TLBCache cache(TLBWays, TLBSets);

    // Everyone goes to the same way. 1:1 mapping
    cache.add(TLBEntry { 64 * 0, 64 * 0 });
    cache.add(TLBEntry { 64 * 1, 64 * 1 });
    cache.add(TLBEntry { 64 * 2, 64 * 2 });
    cache.add(TLBEntry { 64 * 3, 64 * 3 });
    cache.add(TLBEntry { 64 * 4, 64 * 4 });

    auto entry = cache.find(64 * 4);
    EXPECT_TRUE(entry.valid());
    EXPECT_EQ(entry.getTag(), 64 * 64);
    EXPECT_EQ(entry.getPayload().virtualPageId, 64 * 4);
    EXPECT_EQ(entry.getPayload().physicalPageId, 64 * 4);

    auto lostEntry = cache.find(64 * 0);
    EXPECT_FALSE(lostEntry.valid());
}
