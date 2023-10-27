#include "besm-666/decoder/prefetcher.hpp"

namespace besm::dec {

Prefetcher::Prefetcher(mem::MMU::SPtr mmu)
    : mmu_(std::move(mmu)), saved_(nullptr), start_(-1), len_(0) {}

RV64UWord Prefetcher::loadWord(RV64Ptr vaddress) {
    if (vaddress > start_ && vaddress < start_ + len_) {
        // this address was already load
        return *(RV64UWord *)((uint8_t *)saved_ + vaddress - start_);
    } else {
        // load address
        auto pair = mmu_->getHostAddress(vaddress);
        if (pair.second > 0) {
            start_ = vaddress;
            len_ = pair.second;
            saved_ = pair.first;
            return *(RV64UWord *)saved_;
        }
        return mmu_->loadWord(vaddress);
    }
}

} // namespace besm::dec