#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>

namespace besm::util {

template <typename PayloadType, typename TagType> struct TagCount {
    TagType operator()(PayloadType payload) {
        return payload * 2; // TODO: will be changed
    }
};

template <typename PayloadType, typename TagType> class CacheEntry {
public:
    CacheEntry();
    ~CacheEntry(){};

    void setPayload(PayloadType const &payload) noexcept(
        std::is_nothrow_copy_constructible_v<PayloadType>);
    void setPayload(PayloadType &&payload) noexcept(
        std::is_nothrow_move_constructible_v<PayloadType>);

    PayloadType const &getPayload() const noexcept;
    PayloadType &getPayload() noexcept;
    TagType const &getTag() const noexcept;
    void setTag(TagType tag) noexcept;

    void validate() noexcept;
    void invalidate() noexcept;
    bool valid() const noexcept;

private:
    PayloadType payload_;
    bool valid_;
    TagType tag_;
};

template <typename PayloadType, typename TagType,
          template <typename> typename HashFunction = std::hash,
          template <typename, typename> typename TagFunction = TagCount>
class Cache {
public:
    Cache(size_t ways, size_t sets);
    ~Cache() { invalidate(); };

    void add(PayloadType const &payload) noexcept(
        std::is_nothrow_copy_constructible_v<PayloadType>);
    void add(PayloadType &&payload) noexcept(
        std::is_nothrow_move_constructible_v<PayloadType>);

    CacheEntry<PayloadType, TagType> &find(TagType tag) noexcept;
    CacheEntry<PayloadType, TagType> const &find(TagType tag) const noexcept;

    void invalidate(TagType tag) noexcept;
    void invalidate() noexcept;
    size_t getSize() noexcept;

    void dump() noexcept;

private:
    size_t ways_;
    size_t sets_;
    size_t size_ = ways_ * sets_;
    std::unique_ptr<CacheEntry<PayloadType, TagType>[]> cachedData_;
    std::unique_ptr<size_t[]> counters_;
};


//-------------Cache------------------//

template <typename PayloadType, typename TagType,
          template <typename> typename HashFunction,
          template <typename, typename> typename TagFunction>
Cache<PayloadType, TagType, HashFunction, TagFunction>::Cache(size_t ways,
                                                              size_t sets)
    : ways_(ways), sets_(sets) {
    cachedData_ =
        std::make_unique<CacheEntry<PayloadType, TagType>[]>(ways * sets);
    counters_ = std::make_unique<size_t[]>(sets);
}

// TODO: will be changed, test implementation
template <typename PayloadType, typename TagType,
          template <typename> typename HashFunction,
          template <typename, typename> typename TagFunction>
void Cache<PayloadType, TagType, HashFunction, TagFunction>::add(
    PayloadType const
        &payload) noexcept(std::is_nothrow_copy_constructible_v<PayloadType>) {
    TagType tag = TagFunction<PayloadType, TagType>{}(payload);
    auto set = HashFunction<TagType>{}(tag) % sets_;

    if (counters_[set] >= ways_)
        counters_[set] = 0;

    cachedData_[set * ways_ + counters_[set]].setPayload(payload);
    cachedData_[set * ways_ + counters_[set]].setTag(tag);
    counters_[set]++;
}

template <typename PayloadType, typename TagType,
          template <typename> typename HashFunction,
          template <typename, typename> typename TagFunction>
void Cache<PayloadType, TagType, HashFunction, TagFunction>::add(
    PayloadType
        &&payload) noexcept(std::is_nothrow_move_constructible_v<PayloadType>) {
    TagType tag = TagFunction<PayloadType, TagType>{}(payload);
    auto set = HashFunction<TagType>{}(tag) % sets_;

    if (counters_[set] >= ways_)
        counters_[set] = 0;

    cachedData_[set * ways_ + counters_[set]].setPayload(payload);
    cachedData_[set * ways_ + counters_[set]].setTag(tag);
    counters_[set]++;
}

template <typename PayloadType, typename TagType,
          template <typename> typename HashFunction,
          template <typename, typename> typename TagFunction>
CacheEntry<PayloadType, TagType> &
Cache<PayloadType, TagType, HashFunction, TagFunction>::find(
    TagType tag) noexcept {
    auto set = HashFunction<TagType>{}(tag) % sets_;
    for (auto i = set * ways_; i < set * ways_ + ways_; i++) {
        if (cachedData_[i].getTag() == tag && cachedData_[i].valid())
            return cachedData_[i];
    }

    return cachedData_[set * ways_ + counters_[set]];
}

template <typename PayloadType, typename TagType,
          template <typename> typename HashFunction,
          template <typename, typename> typename TagFunction>
CacheEntry<PayloadType, TagType> const &
Cache<PayloadType, TagType, HashFunction, TagFunction>::find(
    TagType tag) const noexcept {
    auto set = HashFunction<TagType>{}(tag) % sets_;
    for (auto i = set * ways_; i < set * ways_ + ways_; i++) {
        if (cachedData_[i].getTag() == tag && cachedData_[i].valid())
            return cachedData_[i];
    }

    return cachedData_[set * ways_ + counters_[set]];
}
// end TODO

template <typename PayloadType, typename TagType,
          template <typename> typename HashFunction,
          template <typename, typename> typename TagFunction>
void Cache<PayloadType, TagType, HashFunction, TagFunction>::invalidate(
    TagType tag) noexcept {
    auto set = HashFunction<TagType>{}(tag) % sets_;
    for (auto i = set * ways_; i < set * ways_ + ways_; i++) {
        if (cachedData_[i].getTag() == tag && cachedData_[i].valid())
            cachedData_[i].invalidate();
    }
}

template <typename PayloadType, typename TagType,
          template <typename> typename HashFunction,
          template <typename, typename> typename TagFunction>
void Cache<PayloadType, TagType, HashFunction,
           TagFunction>::invalidate() noexcept {
    for (int i = 0; i < getSize(); i++) {
        cachedData_[i].invalidate();
    }
}

template <typename PayloadType, typename TagType,
          template <typename> typename HashFunction,
          template <typename, typename> typename TagFunction>
size_t
Cache<PayloadType, TagType, HashFunction, TagFunction>::getSize() noexcept {
    return size_;
}

template <typename PayloadType, typename TagType,
          template <typename> typename HashFunction,
          template <typename, typename> typename TagFunction>
void Cache<PayloadType, TagType, HashFunction, TagFunction>::dump() noexcept {
    for (int i = 0; i < sets_; i++) {
        for (int j = 0; j < ways_; j++) {
            std::cout << cachedData_[i * ways_ + j].getPayload() << " ";
        }
        std::cout << std::endl;
    }
}

//-------------CacheEntry------------------//

template <typename PayloadType, typename TagType>
CacheEntry<PayloadType, TagType>::CacheEntry() : valid_(false), tag_(0) {}

template <typename PayloadType, typename TagType>
void CacheEntry<PayloadType, TagType>::setPayload(
    PayloadType const
        &payload) noexcept(std::is_nothrow_copy_constructible_v<PayloadType>) {
    payload_ = payload;
    validate();
}

template <typename PayloadType, typename TagType>
void CacheEntry<PayloadType, TagType>::setPayload(
    PayloadType
        &&payload) noexcept(std::is_nothrow_move_constructible_v<PayloadType>) {
    payload_ = std::move(payload);
    validate();
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
void CacheEntry<PayloadType, TagType>::validate() noexcept {
    valid_ = true;
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

template <typename PayloadType, typename TagType>
void CacheEntry<PayloadType, TagType>::setTag(TagType tag) noexcept {
    tag_ = tag;
}
} // namespace besm::util