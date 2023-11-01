#pragma once

#include <cstddef>
#include <iostream>
#include <type_traits>

namespace besm::util {

template <typename Type, size_t SignBits>
constexpr Type SignExtend(Type value) {
    static_assert(std::is_unsigned_v<Type>);

    constexpr Type const Mask = (static_cast<Type>(1) << SignBits) - 1;
    constexpr Type const SignbitMask = static_cast<Type>(1) << (SignBits - 1);

    Type cleanUppermostBitsValue = value & Mask;
    Type signExtendedValue =
        (cleanUppermostBitsValue ^ SignbitMask) - SignbitMask;

    return signExtendedValue;
}

template <typename Type, size_t Bits, size_t Where = 0>
constexpr const Type BitMask = ((static_cast<Type>(1) << Bits) - 1) << Where;

template <typename Type, size_t Bits, size_t From = 0>
constexpr Type ExtractBits(Type value) {
    static_assert(std::is_unsigned_v<Type>);

    constexpr Type const Mask = BitMask<Type, Bits, From>;
    return (value & Mask) >> From;
}

template <typename PassT, typename RetT, size_t Bits, size_t From = 0>
constexpr RetT ExtractBits(PassT value) {
    return static_cast<RetT>(ExtractBits<PassT, Bits, From>(value));
}

template <typename Type, size_t Bits, size_t From = 0>
constexpr Type ClearBits(Type value) {
    static_assert(std::is_unsigned_v<Type>);

    return value & ~BitMask<Type, Bits, From>;
}

template <typename Type, size_t Bits, size_t From = 0>
constexpr Type ChangeBits(Type value, Type newBits) {
    static_assert(std::is_unsigned_v<Type>);

    return ClearBits<Type, Bits, From>(value) |
           (ExtractBits<Type, Bits>(newBits) << From);
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

template <typename Type, Type Mask, size_t Pos, size_t Series, size_t Filled,
          size_t Count>
struct MaskedExtractor {
    constexpr static Type Do(Type value) {
        if constexpr (ExtractBits<Type, 1, Pos>(Mask)) {
            return MaskedExtractor<Type, Mask, Pos + 1, Series + 1, Filled,
                                   Count>::Do(value);
        } else {
            constexpr size_t SeriesMaskOffset = Pos - Series;

            return (ExtractBits<Type, Series, SeriesMaskOffset>(value)
                    << Filled) |
                   MaskedExtractor<Type, Mask, Pos + 1, 0, Filled + Series,
                                   Count>::Do(value);
        }
    }
};

template <typename Type, Type Mask, size_t Series, size_t Filled, size_t Count>
struct MaskedExtractor<Type, Mask, Count, Series, Filled, Count> {
    constexpr static Type Do(Type value) {
        constexpr size_t SeriesMaskOffset = Count - Series;

        if constexpr (SeriesMaskOffset == Count) {
            return static_cast<Type>(0);
        } else {
            return (ExtractBits<Type, Series, SeriesMaskOffset>(value)
                    << Filled);
        }
    };
};

template <typename Type, Type Mask> constexpr Type ExtractMasked(Type value) {
    static_assert(std::is_unsigned_v<Type>);
    if constexpr (Mask == ~static_cast<Type>(0)) {
        return value;
    } else {
        return MaskedExtractor<Type, Mask, 0, 0, 0, sizeof(Type) * 8>::Do(
            value);
    }
}

template <typename Type, Type Mask, size_t Pos, size_t Series, size_t Set,
          size_t Count>
struct MaskedInserter {
    constexpr static Type Do(Type value, Type setPlace) {
        if constexpr (ExtractBits<Type, 1, Pos>(Mask)) {
            return MaskedInserter<Type, Mask, Pos + 1, Series + 1, Set,
                                  Count>::Do(value, setPlace);
        } else {
            constexpr size_t SeriesMaskOffset = Pos - Series;

            Type newSetPlace = ChangeBits<Type, Series, SeriesMaskOffset>(
                setPlace, ExtractBits<Type, Series, Set>(value));

            return MaskedInserter<Type, Mask, Pos + 1, 0, Set + Series,
                                  Count>::Do(value, newSetPlace);
        }
    }
};

template <typename Type, Type Mask, size_t Series, size_t Set, size_t Count>
struct MaskedInserter<Type, Mask, Count, Series, Set, Count> {
    constexpr static Type Do(Type value, Type setPlace) {
        constexpr size_t SeriesMaskOffset = Count - Series;

        if constexpr (SeriesMaskOffset == Count) {
            return static_cast<Type>(setPlace);
        } else {
            return ChangeBits<Type, Series, SeriesMaskOffset>(
                setPlace, ExtractBits<Type, Series, Set>(value));
        }
    }
};

template <typename Type, Type Mask>
constexpr Type InsertMasked(Type value, Type setPlace) {
    static_assert(std::is_unsigned_v<Type>);
    if constexpr (Mask == ~static_cast<Type>(0)) {
        return value;
    } else {
        return MaskedInserter<Type, Mask, 0, 0, 0, sizeof(Type) * 8>::Do(
            value, setPlace);
    }
}

} // namespace besm::util
