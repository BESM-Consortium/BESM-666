#include "besm-666/exec/icsr.hpp"
#include "besm-666/exec/csrf.hpp"

namespace besm::exec {

ICSR::ICSR(CSRF &csrf, Id id) noexcept : csrf_(csrf), id_(id) {
    csrf_.addRegister(*this);
}

} // namespace besm::exec
