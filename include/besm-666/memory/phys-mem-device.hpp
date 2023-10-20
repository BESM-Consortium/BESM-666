#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "besm-666/riscv-types.hpp"
#include "besm-666/util/dummy-exception.hpp"

namespace besm::mem {

class IPhysMemDevice {
public:
    enum Type {
        RAM,
        UART,
        TIMER,

        NUM_TYPES
    };

    BESM_UTIL_DUMMY_EXCEPTION(InvalidAddressError);
    BESM_UTIL_DUMMY_EXCEPTION(UnalignedAddressError);

    IPhysMemDevice(Type type);
    virtual ~IPhysMemDevice() = default;

    virtual RV64UChar loadByte(RV64Ptr address) const = 0;
    virtual RV64UHWord loadHWord(RV64Ptr address) const = 0;
    virtual RV64UWord loadWord(RV64Ptr address) const = 0;
    virtual RV64UDWord loadDWord(RV64Ptr address) const = 0;

    virtual void storeByte(RV64Ptr address, RV64UChar value) = 0;
    virtual void storeHWord(RV64Ptr address, RV64UHWord value) = 0;
    virtual void storeWord(RV64Ptr address, RV64UWord value) = 0;
    virtual void storeDWord(RV64Ptr address, RV64UDWord value) = 0;

    virtual std::pair<void const *, size_t>
    getHostAddress(RV64Ptr address) const = 0;
    virtual std::pair<void *, size_t> touchHostAddress(RV64Ptr address) = 0;

    virtual size_t getSize() const = 0;

    Type getType() const noexcept;
    std::string getTypeName() const;

private:
    Type type_;
};

} // namespace besm::mem
