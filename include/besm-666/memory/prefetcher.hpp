#pragma once

#include "besm-666/memory/mmu.hpp"

namespace besm::mem {

/**
 * This class provides fetching of raw instructions, keeping
 *  the host memory address for the case if several of them
 *  are sequentially arranged in host memory.
 */
class Prefetcher {
public:
    explicit Prefetcher(mem::MMU::SPtr mmu);

    /**
     * Loads word using mmu.
     * @param vaddress Virtual address.
     * @return word.
     */
    RV64UWord loadWord(RV64Ptr vaddress);

private:
    mem::MMU::SPtr mmu_;

    /**
     * Hosted address of continuous bytes.
     */
    const RV64UWord *saved_;

    /**
     * Virtual address that corresponds to {@link Prefetcher::saved_}.
     */
    RV64Ptr start_;

    /**
     * Number of continuous bytes starting from {@link Prefetcher::start_}
     */
    RV64Size len_;
};

} // namespace besm::mem
