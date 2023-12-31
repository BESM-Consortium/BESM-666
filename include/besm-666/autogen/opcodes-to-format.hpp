#pragma once

#include "operations-matrix.hpp"
#include <cstdint>

namespace besm::dec {

// @todo #10:90m Generate this file with matrix instead of copypaste from the
// current raw generator.

constexpr static uint8_t OPCODE2FORMAT[128] = {
    ILLEGAL, ILLEGAL, ILLEGAL, I,       ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL,
    ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, I,
    ILLEGAL, ILLEGAL, ILLEGAL, I,       ILLEGAL, ILLEGAL, ILLEGAL, U,
    ILLEGAL, ILLEGAL, ILLEGAL, I,       ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL,
    ILLEGAL, ILLEGAL, ILLEGAL, S,       ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL,
    ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL,
    ILLEGAL, ILLEGAL, ILLEGAL, R,       ILLEGAL, ILLEGAL, ILLEGAL, U,
    ILLEGAL, ILLEGAL, ILLEGAL, R,       ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL,
    ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL,
    ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL,
    ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL,
    ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL,
    ILLEGAL, ILLEGAL, ILLEGAL, B,       ILLEGAL, ILLEGAL, ILLEGAL, I,
    ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, J,
    ILLEGAL, ILLEGAL, ILLEGAL, I,       ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL,
    ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL, ILLEGAL};

} // namespace besm::dec
