#include <iostream>

#include "besm-666/exec/gprf.hpp"
#include "besm-666/sim/config.hpp"
#include "besm-666/sim/machine.hpp"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Invalid arguments, expected elf file name" << std::endl;
        std::terminate();
    }

    besm::sim::ConfigBuilder configBuilder;
    configBuilder.setExecutablePath(argv[1]);

    besm::sim::Config config = configBuilder.build();

    besm::sim::Machine machine(config);

    machine.run();

    besm::exec::GPRFStateDumper(std::clog).dump(machine.getState());

    return machine.getState().read(besm::exec::GPRF::X10) == 1;
}
