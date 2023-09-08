#pragma once

#include <type_traits>

namespace besm {

template <typename IntType> constexpr inline bool Is2Pow(IntType value) {
    static_assert(std::is_integral_v<IntType>, "Only integral types allowed");
    return value == static_cast<IntType>(0) || (value & (value - 1)) == 0;
}

template <typename IntType>
constexpr inline IntType Nearest2PowDivident(IntType value,
                                             IntType divider2pow) {
    static_assert(std::is_integral_v<IntType>, "Only integral types allowed");
    return (value + divider2pow) & ~(divider2pow - 1);
}

} // namespace besm
