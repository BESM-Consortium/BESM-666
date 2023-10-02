#include "besm-666/exec/gprf.hpp"
#include "besm-666/util/bit-magic.hpp"

namespace besm::exec {
void GPRFStateDumper::dump(GPRF const &gprf) {
    stream_ << "[GPRF] State dump (skipped are zeroed):" << std::endl;

    for (Register regId = 0; regId < GPRF::Size; ++regId) {
        RV64UDWord v = gprf.read(regId);
        if(v == 0) {
            continue;
        }

        stream_ << "x" << (int)regId << " (" << REG_ALIAS[regId] << "): ";
        stream_ << "dec = " << v << " (" << util::Signify(v) << "), ";
        stream_ << std::hex << "hex = " << v << std::dec << std::endl;
    }
}
} // namespace besm::exec
