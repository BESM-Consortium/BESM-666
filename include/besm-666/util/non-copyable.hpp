#pragma once

namespace besm {

class INonCopyable {
public:
    INonCopyable(INonCopyable const &) = delete;
    INonCopyable const &operator=(INonCopyable const &) = delete;

protected:
    INonCopyable() = default;
};

} // namespace besm
