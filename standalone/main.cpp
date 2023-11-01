#include <chrono>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "besm-666/sim/hooks.hpp"
#include "capstone/capstone.h"

#include "CLI/CLI.hpp"
#include "besm-666/exec/gprf.hpp"
#include "besm-666/sim/config.hpp"
#include "besm-666/sim/machine.hpp"
#include "besm-666/util/range.hpp"

csh CapstoneHandler;

void OnBBParse(besm::sim::Hart const &hart, void const *pBB) {
    besm::BasicBlock const &bb =
        *reinterpret_cast<besm::BasicBlock const *>(pBB);

    besm::RV64Ptr pc = bb.startPC();
    besm::RV64Size size = bb.size();

    std::clog << "[BESM-666] VERBOSE: Fetched BB of size " << size << " at "
              << pc << ":\n";
    for (besm::RV64Ptr instrAddr = pc; instrAddr < pc + size * 4;
         instrAddr += 4) {
        besm::RV64UWord bytecode = hart.getMMU().loadWord(instrAddr);

        cs_insn *instruction;
        size_t count = cs_disasm(CapstoneHandler,
                                 reinterpret_cast<uint8_t const *>(&bytecode),
                                 4, instrAddr, 0, &instruction);

        if (count == 1) {
            std::clog << "\t" << instruction->address << ": ";
            std::clog << instruction->mnemonic << " " << instruction->op_str
                      << std::endl;
        }
    }
}

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
                      << bytecode << std::dec << " at pc = " << std::hex << pc
                      << std::dec << std::endl;

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

    hookManager->registerHook(besm::sim::HookManager::BASIC_BLOCK_PARSE,
                              OnBBParse);

    hookManager->registerHook(
        besm::sim::HookManager::BASIC_BLOCK_EXECUTE,
        [](besm::sim::Hart const &hart, void const *) {
            std::clog << "[BESM-666] VERBOSE: Force dumping machine state."
                      << std::endl;
            besm::exec::GPRFStateDumper(std::clog).dump(hart.getState());
        });
}

besm::util::Range<besm::RV64Ptr> ParseRange(std::string const &rangeString) {
    auto pos = std::find(rangeString.begin(), rangeString.end(), ',');
    if (pos == rangeString.end()) {
        throw std::invalid_argument("Invalid range");
    } else {
        std::string leftBorderString(rangeString.begin(), pos);
        std::string rightBorderString(pos + 1, rangeString.end());

        besm::RV64Ptr leftBorder = std::stoull(leftBorderString);
        besm::RV64Ptr rightBorder = leftBorder + std::stoull(rightBorderString);

        return besm::util::Range<besm::RV64Ptr>(leftBorder, rightBorder);
    }
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
        ->check(CLI::ExistingFile)
        ->group("Input");

    app.add_option_function<std::string>(
           "--ram",
           [&](std::string const &string) {
               try {
                   configBuilder.addRamRange(ParseRange(string));
               } catch (...) {
                   std::cerr << "Invalid range in --ram: " << string
                             << std::endl;
               }
           },
           "Setup physical memory range as RAM in format \'address,size\' (in "
           "bytes)")
        ->default_val("0,1073741824")
        ->run_callback_for_default()
        ->force_callback()
        ->group("Memory");

    app.add_option_function<size_t>(
           "--ram-page-size",
           [&](size_t value) { configBuilder.setRamPageSize(value); },
           "Setup RAM page size in bytes")
        ->default_val(4096)
        ->run_callback_for_default()
        ->force_callback()
        ->group("Memory");

    app.add_option_function<size_t>(
           "--ram-chunk-size",
           [&](size_t value) { configBuilder.setRamChunkSize(value); })
        ->default_val(2 * 1024 * 1024)
        ->run_callback_for_default()
        ->force_callback()
        ->group("Memory");

    bool verboseLogging = false;
    app.add_flag("-v,--verbose", verboseLogging,
                 "Enables per-instruction machine state logging")
        ->default_val(false)
        ->group("Memory");

    bool a0Validation = false;
    app.add_flag("--a0-validation", a0Validation, "Enable a0 validator mode")
        ->default_val(false)
        ->group("Validation");

    CLI11_PARSE(app, argc, argv);

    std::clog << "[BESM-666] INFO: Creating RISCV machine." << std::endl;
    besm::sim::Config config = configBuilder.build();
    besm::sim::Machine machine(config);

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

    if (a0Validation) {
        if (machine.getState().read(besm::exec::GPRF::X10) == 1) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}
