#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <memory>

namespace besm::util {

template<typename PayloadType>
class CacheEntry {
public:
    CacheEntry();
    ~CacheEntry();

    void setPayload(PayloadType const& payload) 
        noexcept(std::is_nothrow_copy_assignable_v<PayloadType>);
    void setPayload(PayloadType&& payload)
        noexcept(std::is_nothrow_move_assignable_v<PayloadType>);

    PayloadType const& getPayload() const noexcept;
    PayloadType& getPayload() noexcept;

    void validate() noexcept;
    void invalidate() noexcept;
    bool valid() const noexcept;

private:
    PayloadType payload_;
    bool valid_;
};

template<typename PayloadType, typename TagType>
class Cache {
public:
    Cache(size_t ways, size_t sets);
    ~Cache();

    void add(PayloadType const& payload)
        noexcept(std::is_nothrow_copy_assignable_v<PayloadType>);
    void add(PayloadType&& payload)
        noexcept(std::is_nothrow_move_assignable_v<PayloadType>);

    CacheEntry<PayloadType>& find(TagType tag) noexcept;
    CacheEntry<PayloadType> const& find(TagType) const noexcept;

    void invalidate(TagType tag) noexcept;
    void invalidate() noexcept;

private:
    size_t ways_;
    size_t sets_;
    std::unique_ptr<CacheEntry<PayloadType>[]> cachedData_;
    std::unique_ptr<size_t[]> counters_;
};

}
