#include "besm-666/memory/prefetcher.hpp"
#include <cassert>

namespace besm::mem {

Prefetcher::Prefetcher(mem::MMU::SPtr mmu)
    : mmu_(mmu), saved_(nullptr), start_(-1), len_(0) {}

RV64UWord Prefetcher::loadWord(RV64Ptr vaddress) {
    if (vaddress > start_ && vaddress < start_ + len_) {
        // this address was already load
        assert((vaddress - start_) % sizeof(RV64UWord) == 0);
        return *(saved_ + (vaddress - start_) / sizeof(RV64UWord));
    } else {
        // load address
        auto pair = mmu_->getHostAddress(vaddress);
        if (pair.second > 0) {
            start_ = vaddress;
            len_ = pair.second;
            saved_ = static_cast<const RV64UWord *>(pair.first);
            return *saved_;
        }
        return mmu_->loadWord(vaddress);
    }
}

} // namespace besm::mem
