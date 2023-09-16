#include "./decoder/decoder.hpp"
#include "besm-666/autogen/encoding.out.h"
#include <gtest/gtest.h>

#define MATCH_AND 0x7033

TEST(Decoder, decodes_simple_instruction) {
    ASSERT_EQ(besm::decoder::Decoder::next((MATCH_AND & 0b1111111) & 0xFFFFFFFF)
                  .opcode,
              MATCH_AND & 0b1111111);
}

#undef MATCH_AND
