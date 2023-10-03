#include <chrono>
#include <cstdlib>
#include <iostream>

#include "besm-666/sim/hooks.hpp"
#include "capstone/capstone.h"

#include "CLI/CLI.hpp"
#include "besm-666/exec/gprf.hpp"
#include "besm-666/sim/config.hpp"
#include "besm-666/sim/machine.hpp"

csh CapstoneHandler;

void InitVerboseLogging(besm::sim::Machine &machine) {
    besm::sim::HookManager::SPtr hookManager = machine.getHookManager();

    cs_open(CS_ARCH_RISCV, CS_MODE_RISCV64, &CapstoneHandler);
    std::atexit([]() {
        std::clog << "[BESM-666] VERBOSE: Verbose logger finished" << std::endl;
        cs_close(&CapstoneHandler);
    });

    std::clog << "[BESM-666] VERBOSE: Verbose logging enabled" << std::endl;

    hookManager->registerHook(
        besm::sim::HookManager::INSTRUCTION_FETCH,
        [](besm::sim::Hart const &hart, void const *pBytecode) {
            besm::RV64UDWord pc = hart.getState().read(besm::exec::GPRF::PC);
            besm::RV64UWord bytecode =
                *reinterpret_cast<besm::RV64UWord const *>(pBytecode);
            std::clog << "[BESM-666] VERBOSE: Fetched bytecode " << std::hex
                      << bytecode << std::dec << " at pc = " 
                      << std::hex << pc << std::dec << std::endl;

            cs_insn *instruction;
            size_t count = cs_disasm(
                CapstoneHandler, reinterpret_cast<uint8_t const *>(pBytecode),
                4, hart.getState().read(besm::exec::GPRF::PC), 0, &instruction);

            if (count > 0) {
                std::clog << "[BESM-666] VERBOSE: Disassembly\n\t"
                          << instruction->mnemonic << " " << instruction->op_str
                          << std::endl;
            } else {
                std::clog << "[BESM-666] VERBOSE: Failed to disasm bytecode"
                          << std::endl;
            }

            cs_free(instruction, count);
        });

    hookManager->registerHook(
        besm::sim::HookManager::INSTRUCTION_EXECUTE,
        [](besm::sim::Hart const &hart, void const *) {
            std::clog << "[BESM-666] VERBOSE: Force dumping machine state."
                      << std::endl;
            besm::exec::GPRFStateDumper(std::clog).dump(hart.getState());
        });
}

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

    bool verboseLogging = false;
    app.add_flag("-v,--verbose", verboseLogging,
                 "Enables per-instruction machine state logging")
        ->default_val(false);

    CLI11_PARSE(app, argc, argv);

    besm::sim::Config config = configBuilder.build();
    besm::sim::Machine machine(config);
    std::clog << "[BESM-666] INFO: Created RISCV machine." << std::endl;

    if (verboseLogging) {
        InitVerboseLogging(machine);
    }

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
