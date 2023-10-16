#include <CLI/Validators.hpp>
#include <chrono>
#include <cstdlib>
#include <iostream>

#include "CLI/CLI.hpp"

#include "besm-666/exec/gprf.hpp"
#include "besm-666/sim/config.hpp"
#include "besm-666/sim/machine.hpp"

int main(int argc, char *argv[]) {
    besm::sim::ConfigBuilder configBuilder;

    CLI::App app(
        "BESM-666 (Best Ever SiMulator is a toy RISCV functional simulator");

    app.add_option_function<std::string>(
           "-e,--executable",
           [&](std::string const &string) {
               configBuilder.setExecutablePath(string);
           },
           "Setups the file to be executed with the simulator")
        ->required()
        ->check(CLI::ExistingFile);

    app.add_option_function<std::string>(
        "-p,--plugin",
        [&](std::string const &string) { configBuilder.addPlugin(string); },
        "Use plugin with specified path during the simulation");

    CLI11_PARSE(app, argc, argv);

    besm::sim::Config config = configBuilder.build();
    besm::sim::Machine machine(config);
    std::clog << "[BESM-666] INFO: Created RISCV machine." << std::endl;
    std::clog << "[BESM-666] INFO: Starting simulation" << std::endl;

    auto time_start = std::chrono::steady_clock::now();
    machine.run();
    auto time_end = std::chrono::steady_clock::now();

    double ellapsedSecond =
        std::chrono::duration_cast<std::chrono::nanoseconds>(time_end -
                                                             time_start)
            .count() *
        1e-9;

    size_t instrsExecuted = machine.getInstrsExecuted();

    double mips = static_cast<double>(instrsExecuted) * 1e-6 / ellapsedSecond;

    std::clog << "[BESM-666] Simulation finished." << std::endl;
    std::clog << "[BESM-666] Time = " << ellapsedSecond << "s, Insns "
              << instrsExecuted << ", MIPS = " << mips << std::endl;
    besm::exec::GPRFStateDumper(std::clog).dump(machine.getState());

    return 0;
}
