#include <CLI/Validators.hpp>
#include <cstdlib>
#include <iostream>

#include "CLI/CLI.hpp"
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

    CLI11_PARSE(app, argc, argv);

    besm::sim::Config config = configBuilder.build();

    besm::sim::Machine machine(config);

    machine.run();

    return 0;
}
