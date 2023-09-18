#pragma once
#include <stdexcept>

namespace besm::dec {

class IllegalOpcodeException : public std::runtime_error {
public:
    IllegalOpcodeException(std::string cause) : std::runtime_error(cause){};
    IllegalOpcodeException()
        : IllegalOpcodeException("IllegalOpcodeException"){};
};

} // namespace besm::dec
