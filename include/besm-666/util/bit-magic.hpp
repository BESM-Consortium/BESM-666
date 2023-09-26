#pragma once

#include <cstddef>
#include <type_traits>

namespace besm::util {

template <typename Type, size_t SignBits> Type SignExtend(Type value) {
    static_assert(std::is_unsigned_v<Type>);

    constexpr Type const Mask = (static_cast<Type>(1) << SignBits) - 1;
    constexpr Type const SignbitMask = static_cast<Type>(1) << (SignBits - 1);

    Type cleanUppermostBitsValue = value & Mask;
    Type signExtendedValue =
        (cleanUppermostBitsValue ^ SignbitMask) - SignbitMask;

    return signExtendedValue;
}

template <typename Type, size_t Bits, size_t From = 0>
Type ExtractBits(Type value) {
    static_assert(std::is_unsigned_v<Type>);

    constexpr Type const Mask = ((static_cast<Type>(1) << Bits) - 1) << From;

    return (value & Mask) >> From;
}

template <typename PassT, typename RetT, size_t Bits, size_t From = 0>
RetT ExtractBits(PassT value) {
    return static_cast<RetT>(ExtractBits<PassT, Bits, From>(value));
}

template <typename Type> union SUConverter {
    static_assert(std::is_integral_v<Type>);

    std::make_signed_t<Type> s;
    std::make_unsigned_t<Type> u;
};

template <typename Type> std::make_signed_t<Type> Signify(Type value) {
    static_assert(std::is_unsigned_v<Type>);

    SUConverter<Type> c = {.u = value};
    return c.s;
}

template <typename Type> std::make_unsigned_t<Type> Unsignify(Type value) {
    static_assert(std::is_signed_v<Type>);

    SUConverter<Type> c = {.s = value};
    return c.u;
}

} // namespace besm::util
