#pragma once

#include <functional>

namespace besm::util {

/// Implements range [leftBorder, rightBorder)
template <typename Type> class Range {
public:
    Range(Type leftBorder, Type rightBorder) noexcept;

    bool contains(Type position) const noexcept;
    bool contains(Range<Type> range) const noexcept;
    bool intersects(Range<Type> range) const noexcept;

    Type leftBorder() const noexcept { return leftBorder_; }
    Type rightBorder() const noexcept { return rightBorder_; }

private:
    Type leftBorder_;
    Type rightBorder_;
};

template <typename Type>
Range<Type>::Range(Type leftBorder, Type rightBorder) noexcept
    : leftBorder_(leftBorder), rightBorder_(rightBorder) {}

template <typename Type>
bool Range<Type>::contains(Type position) const noexcept {
    return leftBorder_ <= position && position < rightBorder_;
}

template <typename Type>
bool Range<Type>::contains(Range<Type> range) const noexcept {
    return leftBorder_ <= range.leftBorder_ &&
           range.rightBorder_ < rightBorder_;
}

template <typename Type>
bool Range<Type>::intersects(Range<Type> range) const noexcept {
    return this->contains(range.leftBorder_) ||
           this->contains(range.rightBorder_) || range.contains(leftBorder_) ||
           range.contains(rightBorder_);
}

template <typename Type> bool operator<(Range<Type> lhv, Range<Type> rhv) {
    return lhv.leftBorder() < rhv.leftBorder();
}

} // namespace besm::util
