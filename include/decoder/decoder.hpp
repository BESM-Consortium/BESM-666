#ifndef BESM_666_DECODER_HPP
#define BESM_666_DECODER_HPP

#include "besm-666/instruction.hpp"
#include <cassert>
#include <cstdlib>
#include <stdexcept>

#define R 1
#define I 2
#define S 3
#define B 4
#define U 5
#define J 6

namespace besm::decoder {

class Decoder {

#define R 1
#define I 2
#define S 3
#define B 4
#define U 5
#define J 6

public:
    constexpr static uint8_t OPCODES[128] = {
        0, 0, 0, I, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, I, 0, 0, 0, I, 0, 0,
        0, U, 0, 0, 0, I, 0, 0, 0, 0, 0, 0, 0, S, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, R, 0, 0, 0, U, 0, 0, 0, R, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, B, 0, 0, 0, I, 0, 0, 0, 0, 0, 0,
        0, J, 0, 0, 0, I, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    /**
     * Give the {@link Instruction} by the word.
     * @param word word.
     * @return {@link Instruction} struct.
     */
    static besm::Instruction next(const RV64Word word) {
        const Opcode opcode = word & 0b01111111;
        assert(opcode < 128);
        switch (OPCODES[opcode]) {
        case R:
            return parse_R(word, opcode);
        case I:
            return parse_I(word, opcode);
        case S:
            return parse_S(word, opcode);
        case B:
            return parse_B(word, opcode);
        case U:
            return parse_U(word, opcode);
        case J:
            return parse_J(word, opcode);
        default:
            throw std::invalid_argument(
                std::string("Opcode cannot be %i", opcode));
        }
    }

private:
    /*
     * @todo #10:90m Implement the functions of R format. Now it is just
     *  a stub. We need to implement every of them and test.
     */
    static inline Instruction parse_R(const RV64Word word,
                                      const Opcode opcode) {
        return Instruction{.opcode = opcode};
    }

    /*
     * @todo #10:90m Implement the functions of I format. Now it is just
     *  a stub. We need to implement every of them and test.
     */
    static inline Instruction parse_I(const RV64Word word,
                                      const Opcode opcode) {
        return Instruction{.opcode = opcode};
    }

    /*
     * @todo #10:90m Implement the functions of S format. Now it is just
     *  a stub. We need to implement every of them and test.
     */
    static inline Instruction parse_S(const RV64Word word,
                                      const Opcode opcode) {
        return Instruction{.opcode = opcode};
    }

    /*
     * @todo #10:90m Implement the functions of B format. Now it is just
     *  a stub. We need to implement every of them and test.
     */
    static inline Instruction parse_B(const RV64Word word,
                                      const Opcode opcode) {
        return Instruction{.opcode = opcode};
    }

    /*
     * @todo #10:90m Implement the functions of U format. Now it is just
     *  a stub. We need to implement every of them and test.
     */
    static inline Instruction parse_U(const RV64Word word,
                                      const Opcode opcode) {
        return Instruction{.opcode = opcode};
    }

    /*
     * @todo #10:90m Implement the functions of J format. Now it is just
     *  a stub. We need to implement every of them and test.
     */
    static inline Instruction parse_J(const RV64Word word,
                                      const Opcode opcode) {
        return Instruction{.opcode = opcode};
    }

#undef R
#undef I
#undef S
#undef B
#undef U
#undef J
};

} // namespace besm::decoder
#endif // BESM_666_DECODER_HPP
