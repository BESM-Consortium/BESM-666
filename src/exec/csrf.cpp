#include "besm-666/exec/csrf.hpp"
#include <functional>

namespace besm::exec {

CSRF::CSRF() : mstatus(*this) { this->addRegister(mstatus); }

void CSRF::addRegister(ICSR &reg) {
    registers_.insert(
        std::make_pair(reg.getId(), std::reference_wrapper<ICSR>(reg)));
}

} // namespace besm::exec
