#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "besm-666/instruction.hpp"
#include "besm-666/riscv-types.hpp"
#include "besm-666/rv-instruction-op.hpp"
#include "besm-666/util/bit-magic.hpp"
#include "capstone/capstone.h"

#include "CLI/CLI.hpp"

#include "besm-666/exec/gprf.hpp"
#include "besm-666/sim/config.hpp"
#include "besm-666/sim/hooks.hpp"
#include "besm-666/sim/machine.hpp"
#include "besm-666/util/range.hpp"

bool optionDumpInstructions = false;
bool optionDumpRegisters = false;
bool optionTracingEnabled = false;

std::ofstream traceFile;

csh CapstoneHandler;
std::shared_ptr<besm::sim::Machine> Machine;

besm::RV64Ptr CurrentPC;

std::string str_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::toupper(c); } // correct
    );
    return s;
}

// TD: Dev CSR dumper
// TD: Move pc to instr callback and remove bb fetch callback
void OnBBFetch(besm::exec::BasicBlock const &bb) {
    if (optionDumpInstructions) {
        std::clog << "[BESM-666] VERBOSE: Fetched basic block at PC = "
                  << bb.getPC() << std::endl;
    }

    CurrentPC = bb.getPC();
}

void DumpReg(besm::Register regId, besm::exec::GPRF const &gprf) {
    besm::exec::GPRFStateDumper gprfDumper(std::clog);
    if (regId < besm::exec::GPRF::Size) {
        besm::RV64UDWord value = gprf.read(regId);

        std::clog << '\t' << gprfDumper.getRegName(regId) << " = ";
        std::clog << "0x" << std::hex << value << std::dec << " / " << value
                  << std::endl;
    }
}

void OnInstrExecuted(besm::Instruction const &instr) {
    besm::sim::Hart const &hart = Machine->getHart();

    besm::RV64UWord bytecode = hart.getMMU().loadWord(CurrentPC);

    cs_insn *disassembly;
    size_t count =
        cs_disasm(CapstoneHandler, reinterpret_cast<uint8_t const *>(&bytecode),
                  4, CurrentPC, 1, &disassembly);

    if (count != 1) {
        std::clog << "\tunimp" << std::endl;
    } else {
        if (optionDumpInstructions) {
            std::clog << std::hex << disassembly->address << std::dec << ": ";
            std::clog << disassembly->mnemonic << ' ' << disassembly->op_str
                      << std::endl;

            if (optionDumpRegisters) {
                DumpReg(instr.rd, hart.getGPRF());
                DumpReg(instr.rs1, hart.getGPRF());
                DumpReg(instr.rs2, hart.getGPRF());
                std::clog << "imm = "
                          << "0x" << std::hex << instr.immidiate << std::dec
                          << " / " << instr.immidiate << std::endl;
            }
        }

        if (optionTracingEnabled) {
            std::string mnem = str_toupper(disassembly->mnemonic);
            if(mnem == "MV") {
                mnem = "ADDI";
            }
            if(mnem == "BNEZ") {
                mnem = "BNE";
            }

            traceFile << std::hex << CurrentPC << std::dec << ": "
                      << mnem << '\n';

            if (instr.rd != besm::exec::GPRF::X0) {
                traceFile << "\tReg [" << std::setw(2) << std::setfill('0')
                          << std::hex << (int)instr.rd << std::setw(1)
                          << std::setfill(' ') << std::dec
                          << "] <= " << std::hex
                          << Machine->getHart().getGPRF().read(instr.rd)
                          << std::endl;
            }
            if (instr.isJump()) {
                traceFile << "\tPc <= " << std::hex
                          << Machine->getHart().getGPRF().read(
                                 besm::exec::GPRF::PC)
                          << std::dec << std::endl;
            }

            if (instr.isStore()) {
                besm::RV64UDWord addr =
                    Machine->getHart().getGPRF().read(instr.rs1) +
                    besm::util::SignExtend<besm::RV64UDWord, 12>(
                        instr.immidiate);

                besm::RV64UDWord val;

                switch (instr.operation) {
                case besm::InstructionOp::SB:
                    val = Machine->getHart().getMMU().loadByte(addr);
                    break;
                case besm::InstructionOp::SH:
                    val = Machine->getHart().getMMU().loadHWord(addr);
                    break;
                case besm::InstructionOp::SW:
                    val = Machine->getHart().getMMU().loadWord(addr);
                    break;
                case besm::InstructionOp::SD:
                    val = Machine->getHart().getMMU().loadDWord(addr);
                    break;
                }

                traceFile << "\tMem [" << std::hex << addr << "] <= " << val
                          << std::dec << std::endl;
            }
        }

        cs_free(disassembly, 1);
    }

    CurrentPC += 4;
}

void InitCapstone() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

    cs_open(CS_ARCH_RISCV, CS_MODE_RISCV64, &CapstoneHandler);
    std::atexit([]() { cs_close(&CapstoneHandler); });

    initialized = true;
}

void InitVerboseLogging() {
    besm::sim::HookManager const &hookManager = Machine->getHookManager();

    InitCapstone();

    std::clog << "[BESM-666] VERBOSE: Verbose logging enabled" << std::endl;

    Machine->getHookManager().registerBBFetchHook(OnBBFetch);
    Machine->getHookManager().registerInstrExecHook(OnInstrExecuted);
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

    app.add_flag("-v,--verbose", optionDumpInstructions,
                 "Enables per-instruction machine state logging")
        ->default_val(false)
        ->group("Debug");

    app.add_flag("-V,--very-verbose", optionDumpRegisters,
                 "Enables per-instruction machine state logging")
        ->default_val(false)
        ->group("Debug");

    std::string traceFilename;
    app.add_option("-t,--trace-dump", traceFilename)->group("Debug");

    bool a0Validation = false;
    app.add_flag("--a0-validation", a0Validation, "Enable a0 validator mode")
        ->default_val(false)
        ->group("Validation");

    CLI11_PARSE(app, argc, argv);

    std::clog << "[BESM-666] INFO: Creating RISCV Machine->" << std::endl;
    besm::sim::Config config = configBuilder.build();
    Machine = std::make_unique<besm::sim::Machine>(config);

    if (!traceFilename.empty()) {
        optionTracingEnabled = true;
        traceFile.open(traceFilename);
    }

    if (optionDumpInstructions || optionTracingEnabled) {
        InitVerboseLogging();
    }

    std::clog << "[BESM-666] INFO: Starting simulation" << std::endl;

    auto time_start = std::chrono::steady_clock::now();
    Machine->run();
    auto time_end = std::chrono::steady_clock::now();

    double ellapsedSecond =
        std::chrono::duration_cast<std::chrono::nanoseconds>(time_end -
                                                             time_start)
            .count() *
        1e-9;

    size_t instrsExecuted = Machine->getInstrsExecuted();
    double mips = static_cast<double>(instrsExecuted) * 1e-6 / ellapsedSecond;

    std::clog << "[BESM-666] Simulation finished." << std::endl;
    std::clog << "[BESM-666] Time = " << ellapsedSecond << "s, Insns "
              << instrsExecuted << ", MIPS = " << mips << std::endl;
    besm::exec::GPRFStateDumper(std::clog).dump(Machine->getHart().getGPRF());

    if (a0Validation) {
        if (Machine->getHart().getGPRF().read(besm::exec::GPRF::X10) == 1) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}
