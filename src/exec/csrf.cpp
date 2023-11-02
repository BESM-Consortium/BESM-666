#include <array>
#include <functional>
#include <utility>

#include "besm-666/exec/csrf.hpp"
#include "besm-666/util/bit-magic.hpp"

namespace besm::exec {

CSRF::CSRF()
    : mhartid(*this), mstatus(*this), mepc(*this), mtvec(*this), mcause(*this),
      privillege_(PRIVILLEGE_MACHINE) {}

std::variant<bool, RV64UDWord> CSRF::write(RV64UDWord rawId,
                                           RV64UDWord value) noexcept {
    ICSR::Id id = static_cast<ICSR::Id>(rawId);
    if (!this->fitPrivillege(id) || this->registerReadOnly(id)) {
        return false;
    }

    auto regItr = registers_.find(id);
    if (regItr == registers_.cend()) {
        return false;
    }

    ICSR &reg = regItr->second;

    RV64UDWord oldValue = reg.read();

    if (!reg.write(value)) {
        return false;
    }

    return oldValue;
}

std::variant<bool, RV64UDWord> CSRF::read(RV64UDWord rawId) const noexcept {
    ICSR::Id id = static_cast<ICSR::Id>(rawId);
    if (!this->fitPrivillege(id)) {
        return false;
    }

    auto regItr = registers_.find(id);
    if (regItr == registers_.cend()) {
        return false;
    }

    ICSR &reg = regItr->second;

    return reg.read();
}

std::variant<bool, RV64UDWord> CSRF::setBits(RV64UDWord rawId,
                                             RV64UDWord mask) noexcept {
    ICSR::Id id = static_cast<ICSR::Id>(rawId);
    if (!this->fitPrivillege(id) || this->registerReadOnly(id)) {
        return false;
    }

    auto regItr = registers_.find(id);
    if (regItr == registers_.cend()) {
        return false;
    }

    ICSR &reg = regItr->second;

    RV64UDWord oldValue = reg.read();

    if (!reg.write(oldValue | mask)) {
        return false;
    }

    return oldValue;
}

std::variant<bool, RV64UDWord> CSRF::clearBits(RV64UDWord rawId,
                                               RV64UDWord mask) noexcept {
    ICSR::Id id = static_cast<ICSR::Id>(rawId);
    if (!this->fitPrivillege(id) || this->registerReadOnly(id)) {
        return false;
    }

    auto regItr = registers_.find(id);
    if (regItr == registers_.cend()) {
        return false;
    }

    ICSR &reg = regItr->second;

    RV64UDWord oldValue = reg.read();

    if (!reg.write(oldValue & ~mask)) {
        return false;
    }

    return oldValue;
}

void CSRF::addRegister(ICSR &reg) {
    registers_.insert(
        std::make_pair(reg.getId(), std::reference_wrapper<ICSR>(reg)));
}

RV64UDWord CSRF::registerPrivillege(ICSR::Id id) noexcept {
    RV64UWord rawid = static_cast<RV64UWord>(id);
    RV64UWord rawpriv = util::ExtractBits<RV64UWord, 2, 8>(rawid);
    return rawpriv;
}

bool CSRF::registerReadOnly(ICSR::Id id) noexcept {
    RV64UWord rawid = static_cast<RV64UWord>(id);
    return util::ExtractBits<RV64UWord, 2, 10>(id) == 0b11;
}

bool CSRF::fitPrivillege(ICSR::Id id) const noexcept {
    return CSRF::registerPrivillege(id) == this->getPrivillege();
}

} // namespace besm::exec
