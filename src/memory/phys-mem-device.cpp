#include "besm-666/memory/phys-mem-device.hpp"

namespace besm::mem {

IPhysMemDevice::IPhysMemDevice(Type type) : type_(type) {}

IPhysMemDevice::Type IPhysMemDevice::getType() const noexcept { return type_; }

std::string IPhysMemDevice::getTypeName() const {
    char const *TABLE[NUM_TYPES] = {"RAM", "UART", "TIMER"};

    char const *result = "Undefined";
    if (type_ < NUM_TYPES) {
        result = TABLE[type_];
    }

    return std::string(result);
}

} // namespace besm::mem
