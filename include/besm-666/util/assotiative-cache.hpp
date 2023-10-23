#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>

namespace besm::util {

// @todo #71:90m Implement TagFunction and HashFunction and get rid of keeping
// tag in CacheEntry
template <typename PayloadType, typename TagType> class CacheEntry {
public:
    CacheEntry();
    ~CacheEntry() = default;

    void setPayload(PayloadType const &payload, TagType tag) noexcept(
        std::is_nothrow_copy_constructible_v<PayloadType>);
    void setPayload(PayloadType &&payload, TagType tag) noexcept(
        std::is_nothrow_move_constructible_v<PayloadType>);

    PayloadType const &getPayload() const noexcept;
    PayloadType &getPayload() noexcept;
    TagType const &getTag() const noexcept;

    void invalidate() noexcept;
    bool valid() const noexcept;

private:
    PayloadType payload_;
    bool valid_;
    TagType tag_;
};

template <typename PayloadType, typename TagType>
using TagFunction = TagType (*)(PayloadType const &);

template <typename PayloadType, typename HashType>
using HashFunction = HashType (*)(PayloadType const &);

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
class Cache {
public:
    Cache(size_t ways, size_t sets);

    Cache(Cache &&other) noexcept;
    Cache &operator=(Cache &&other) noexcept;

    void add(PayloadType const &payload) noexcept(
        std::is_nothrow_copy_constructible_v<PayloadType>);
    void add(PayloadType &&payload) noexcept(
        std::is_nothrow_move_constructible_v<PayloadType>);

    CacheEntry<PayloadType, TagType> &find(TagType tag) noexcept;
    CacheEntry<PayloadType, TagType> const &find(TagType tag) const noexcept;

    void invalidate(TagType tag) noexcept;
    void invalidate() noexcept;
    size_t getSize() const noexcept;
    size_t getWays() const noexcept;
    size_t getSets() const noexcept;
    size_t getCounter(size_t set) noexcept;

    template <typename Cache> friend class CacheStateDumper;

private:
    size_t ways_;
    size_t sets_;
    size_t size_ = ways_ * sets_;
    std::unique_ptr<size_t[]> counters_;

protected:
    std::unique_ptr<CacheEntry<PayloadType, TagType>[]> cachedData_;
};

template <typename Cache> class CacheStateDumper {
public:
    CacheStateDumper(std::ostream &stream) : stream_(stream) {}
    void dump(Cache &cache);

private:
    std::ostream &stream_;
};

//-------------CacheStateDumper------------------//
template <typename Cache> void CacheStateDumper<Cache>::dump(Cache &cache) {

    stream_ << "[Cache] State dump" << std::endl;

    for (int i = 0; i < cache.getSets(); i++) {
        for (int j = 0; j < cache.getWays(); j++) {
            if (cache.cachedData_[i * cache.getWays() + j].valid())
                stream_
                    << cache.cachedData_[i * cache.getWays() + j].getPayload()
                    << " ";
            else
                stream_ << "inv ";
        }
        stream_ << std::endl;
    }

    stream_ << "Size: " << cache.getSize() << std::endl;
}

//-------------Cache------------------//

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::Cache(
    Cache<PayloadType, TagType, HashType, TagFunc, HashFunc> &&other) noexcept
    : ways_(other.ways_), sets_(other.sets_), size_(other.size_),
      counters_(std::move(other.counters_)),
      cachedData_(std::move(other.cachedData_)) {}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
Cache<PayloadType, TagType, HashType, TagFunc, HashFunc> &
Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::operator=(
    Cache<PayloadType, TagType, HashType, TagFunc, HashFunc> &&other) noexcept {
    std::swap(ways_, other.ways_);
    std::swap(sets_, other.sets_);
    std::swap(size_, other.size_);
    std::swap(counters_, other.counters_);
    std::swap(cachedData_, other.cachedData_);
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::Cache(size_t ways,
                                                                size_t sets)
    : ways_(ways), sets_(sets) {
    cachedData_ =
        std::make_unique<CacheEntry<PayloadType, TagType>[]>(ways * sets);
    counters_ = std::make_unique<size_t[]>(sets);
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
void Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::add(
    PayloadType const
        &payload) noexcept(std::is_nothrow_copy_constructible_v<PayloadType>) {
    TagType tag = TagFunc(payload);
    auto set = HashFunc(tag) % sets_;

    cachedData_[set * ways_ + counters_[set]].setPayload(payload, tag);
    counters_[set] = (counters_[set] + 1) % ways_;
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
void Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::add(
    PayloadType
        &&payload) noexcept(std::is_nothrow_move_constructible_v<PayloadType>) {
    TagType tag = TagFunc(payload);
    auto set = HashFunc(tag) % sets_;

    cachedData_[set * ways_ + counters_[set]].setPayload(std::move(payload),
                                                         tag);
    counters_[set] = (counters_[set] + 1) % ways_;
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
CacheEntry<PayloadType, TagType> &
Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::find(
    TagType tag) noexcept {
    auto set = HashFunc(tag) % sets_;
    for (auto i = set * ways_; i < set * ways_ + ways_; i++) {
        if (cachedData_[i].getTag() == tag && cachedData_[i].valid())
            return cachedData_[i];
    }

    cachedData_[set * ways_ + counters_[set]].invalidate();
    return cachedData_[set * ways_ + counters_[set]];
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
CacheEntry<PayloadType, TagType> const &
Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::find(
    TagType tag) const noexcept {
    return const_cast<CacheEntry<PayloadType, TagType> const &>(
        (const_cast<Cache<PayloadType, TagType, HashType, TagFunc, HashFunc> *>(
             this))
            ->find(tag));
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
void Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::invalidate(
    TagType tag) noexcept {
    auto set = HashFunc(tag) % sets_;
    for (auto i = set * ways_; i < set * ways_ + ways_; i++) {
        if (cachedData_[i].getTag() == tag && cachedData_[i].valid())
            cachedData_[i].invalidate();
    }
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
void Cache<PayloadType, TagType, HashType, TagFunc,
           HashFunc>::invalidate() noexcept {
    for (int i = 0; i < getSize(); i++) {
        cachedData_[i].invalidate();
    }
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
size_t
Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::getSize() const noexcept {
    return size_;
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
size_t
Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::getWays() const noexcept {
    return ways_;
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
size_t
Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::getSets() const noexcept {
    return sets_;
}

template <typename PayloadType, typename TagType, typename HashType,
          TagFunction<PayloadType, TagType> TagFunc,
          HashFunction<TagType, HashType> HashFunc>
size_t Cache<PayloadType, TagType, HashType, TagFunc, HashFunc>::getCounter(
    size_t set) noexcept {
    return counters_[set];
}

//-------------CacheEntry------------------//
template <typename PayloadType, typename TagType>
CacheEntry<PayloadType, TagType>::CacheEntry() : valid_(false), tag_(0) {}

template <typename PayloadType, typename TagType>
void CacheEntry<PayloadType, TagType>::setPayload(
    PayloadType const &payload,
    TagType tag) noexcept(std::is_nothrow_copy_constructible_v<PayloadType>) {
    payload_ = payload;
    tag_ = tag;
    valid_ = true;
}

template <typename PayloadType, typename TagType>
void CacheEntry<PayloadType, TagType>::setPayload(
    PayloadType &&payload,
    TagType tag) noexcept(std::is_nothrow_move_constructible_v<PayloadType>) {
    payload_ = std::move(payload);
    tag_ = tag;
    valid_ = true;
}

template <typename PayloadType, typename TagType>
PayloadType const &
CacheEntry<PayloadType, TagType>::getPayload() const noexcept {
    return payload_;
}

template <typename PayloadType, typename TagType>
PayloadType &CacheEntry<PayloadType, TagType>::getPayload() noexcept {
    return payload_;
}

template <typename PayloadType, typename TagType>
void CacheEntry<PayloadType, TagType>::invalidate() noexcept {
    valid_ = false;
}

template <typename PayloadType, typename TagType>
bool CacheEntry<PayloadType, TagType>::valid() const noexcept {
    return valid_;
}

template <typename PayloadType, typename TagType>
TagType const &CacheEntry<PayloadType, TagType>::getTag() const noexcept {
    return tag_;
}
} // namespace besm::util
