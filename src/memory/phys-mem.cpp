#include <cassert>
#include <memory>
#include <stdexcept>

#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/memory/ram.hpp"
#include "besm-666/util/elf-parser.hpp"
#include "besm-666/util/math.hpp"

namespace besm::mem {

RV64UChar PhysMem::loadByte(RV64Ptr address) const {
    auto [range, device] = this->findDevice(address);
    return device->loadByte(address - range.leftBorder());
}
RV64UHWord PhysMem::loadHWord(RV64Ptr address) const {
    auto [range, device] = this->findDevice(address);
    return device->loadHWord(address - range.leftBorder());
}
RV64UWord PhysMem::loadWord(RV64Ptr address) const {
    auto [range, device] = this->findDevice(address);
    return device->loadWord(address - range.leftBorder());
}
RV64UDWord PhysMem::loadDWord(RV64Ptr address) const {
    auto [range, device] = this->findDevice(address);
    return device->loadDWord(address - range.leftBorder());
}

void PhysMem::storeByte(RV64Ptr address, RV64UChar value) {
    auto [range, device] = this->findDevice(address);
    device->storeByte(address, value);
}
void PhysMem::storeHWord(RV64Ptr address, RV64UHWord value) {
    auto [range, device] = this->findDevice(address);
    device->storeHWord(address, value);
}
void PhysMem::storeWord(RV64Ptr address, RV64UWord value) {
    auto [range, device] = this->findDevice(address);
    device->storeWord(address, value);
}
void PhysMem::storeDWord(RV64Ptr address, RV64UDWord value) {
    auto [range, device] = this->findDevice(address);
    device->storeDWord(address, value);
}

void PhysMem::storeContArea(RV64Ptr address, void const *data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        this->storeByte(address + i,
                        *(reinterpret_cast<RV64UChar const *>(data) + i));
    }
}

std::pair<void const *, size_t> PhysMem::getHostAddress(RV64Ptr address) const {
    auto [range, device] = this->findDevice(address);
    return device->getHostAddress(address - range.leftBorder());
}
std::pair<void *, size_t> PhysMem::getHostAddress(RV64Ptr address) {
    auto [range, device] = this->findDevice(address);
    return device->getHostAddress(address - range.leftBorder());
}

std::pair<util::Range<RV64Ptr>, std::shared_ptr<IPhysMemDevice>>
PhysMem::findDevice(RV64Ptr address) const {
    if (lastAccessedRange_.contains(address)) {
        return std::make_pair(lastAccessedRange_, lastAccessedDevice_);
    }

    for (auto [range, device] : devices_) {
        if (range.contains(address)) {
            lastAccessedRange_ = range;
            lastAccessedDevice_ = device;

            return std::make_pair(range, device);
        }
    }

    throw IPhysMemDevice::InvalidAddressError("Device not found");
}

std::vector<PhysMem::DeviceDescriptor> PhysMem::getDevices() const {
    std::vector<DeviceDescriptor> deviceDescriptors;
    for (auto const &device : devices_) {
        DeviceDescriptor descriptor = {
            .range = device.first,
            .device =
                std::static_pointer_cast<IPhysMemDevice const>(device.second)};
        deviceDescriptors.push_back(std::move(descriptor));
    }

    return deviceDescriptors;
}

void PhysMemBuilder::mapRAM(RV64Ptr address, size_t ramSize, size_t ramPageSize,
                            size_t ramChunkSize) {
    std::shared_ptr<IPhysMemDevice> ram =
        std::make_shared<dev::RAM>(ramSize, ramPageSize, ramChunkSize);

    this->mapDevice(ram, address);
}

void PhysMemBuilder::mapDevice(std::shared_ptr<IPhysMemDevice> const &device,
                               RV64Ptr address) {
    util::Range<RV64Ptr> range(address, address + device->getSize());

    for (auto const &device : devices_) {
        if (range.contains(device.first)) {
            throw MappingIntersection("Invalid mapping");
        }
    }

    devices_.insert(std::make_pair(range, device));
}

std::shared_ptr<PhysMem> PhysMemBuilder::build() {
    return std::shared_ptr<PhysMem>(new PhysMem(std::move(devices_)));
}

PhysMemLoader::PhysMemLoader(std::shared_ptr<PhysMem> const &physMem)
    : physMem_(physMem) {}

void PhysMemLoader::loadElf(std::filesystem::path const &elfPath) {
    std::unique_ptr<util::IElfParser> parser = util::createParser(elfPath);
    for (auto const &segment : parser->getLoadableSegments()) {
        physMem_->storeContArea(segment.address, segment.data, segment.size);
    }
}

} // namespace besm::mem
