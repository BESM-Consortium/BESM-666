#pragma once

#include <memory>
#include <string>

#include "besm-666/riscv-types.hpp"
#include "besm-666/util/bit-magic.hpp"

namespace besm::exec {

class CSRF;

class ICSR {
public:
    enum Id : RV64UDWord {
        MVENDORID = 0xF11,
        MARCHID = 0xF12,
        MIMPID = 0xF13,
        MHARTID = 0xF14,
        MCONFIGPTR = 0xF15,

        MSTATUS = 0x300,
        MEPC = 0x341,

        NUM_IDS
    };

    ICSR(CSRF &csrf, Id id) noexcept;
    virtual ~ICSR() = default;

    Id getId() const noexcept { return id_; }

    virtual RV64UDWord read() const noexcept = 0;
    virtual bool write(RV64UDWord value) noexcept = 0;

protected:
    virtual void onUpdate() noexcept = 0;

    CSRF &csrf_;
    Id id_;
};

inline bool operator==(ICSR const &lhs, ICSR const &rhs) noexcept {
    return lhs.getId() == rhs.getId();
}

} // namespace besm::exec
