#pragma once

#include <filesystem>
#include <map>
#include <memory>

#include "besm-666/memory/phys-mem-device.hpp"
#include "besm-666/riscv-types.hpp"
#include "besm-666/util/dummy-exception.hpp"
#include "besm-666/util/elf-parser.hpp"
#include "besm-666/util/non-copyable.hpp"
#include "besm-666/util/range.hpp"

namespace besm::mem {

using PhysMemDeviceMap =
    std::map<util::Range<RV64Ptr>, std::shared_ptr<IPhysMemDevice>>;

class PhysMem : public INonCopyable {
public:
    ~PhysMem() = default;

    RV64UChar loadByte(RV64Ptr address) const;
    RV64UHWord loadHWord(RV64Ptr address) const;
    RV64UWord loadWord(RV64Ptr address) const;
    RV64UDWord loadDWord(RV64Ptr address) const;

    void storeByte(RV64Ptr address, RV64UChar value);
    void storeHWord(RV64Ptr address, RV64UHWord value);
    void storeWord(RV64Ptr address, RV64UWord value);
    void storeDWord(RV64Ptr address, RV64UDWord value);

    void storeContArea(RV64Ptr address, void const *data, size_t size);

    std::pair<void const *, size_t> getHostAddress(RV64Ptr address) const;
    std::pair<void *, size_t> touchHostAddress(RV64Ptr address);

    struct DeviceDescriptor {
        util::Range<RV64Ptr> const range;
        std::shared_ptr<IPhysMemDevice const> const device;
    };

    std::vector<DeviceDescriptor> getDevices() const;

private:
    friend class PhysMemBuilder;

    PhysMem(PhysMemDeviceMap &&devices)
        : devices_(std::move(devices)), lastAccessedRange_(0, 0) {}

    std::pair<util::Range<RV64Ptr>, std::shared_ptr<IPhysMemDevice>>
    findDevice(RV64Ptr address) const;

    PhysMemDeviceMap devices_;
    mutable util::Range<RV64Ptr> lastAccessedRange_;
    mutable std::shared_ptr<IPhysMemDevice> lastAccessedDevice_;
};

class PhysMemBuilder {
public:
    BESM_UTIL_DUMMY_EXCEPTION(MappingIntersection);

    PhysMemBuilder() = default;
    ~PhysMemBuilder() = default;

    PhysMemBuilder &mapRAM(RV64Ptr address, size_t ramSize, size_t ramPageSize,
                           size_t ramChunkSize);
    PhysMemBuilder &mapUART();
    PhysMemBuilder &mapTimer();

    std::shared_ptr<PhysMem> build();

private:
    void mapDevice(std::shared_ptr<IPhysMemDevice> const &device,
                   RV64Ptr address);

    PhysMemDeviceMap devices_;
};

class PhysMemLoader {
public:
    PhysMemLoader(std::shared_ptr<PhysMem> const &physMem);

    void loadElf(std::filesystem::path const &elfPath);
    void loadIso(std::filesystem::path const &isoPath);
    void loadBin(RV64Ptr address, std::filesystem::path const &isoPath);

private:
    std::shared_ptr<PhysMem> physMem_;
};

} // namespace besm::mem
