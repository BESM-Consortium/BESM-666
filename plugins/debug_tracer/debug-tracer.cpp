#include <iostream>
#include "CLI/CLI.hpp"
#include "besm-666/exec/gprf.hpp"
#include "besm-666/instruction.hpp"
#include "besm-666/riscv-types.hpp"
#include "capstone/capstone.h"

#include "besm-666/sim/hooks.hpp"
#include "besm-666/sim/hart.hpp"

namespace {

std::ofstream logFile;
std::ostream* pLogStream = nullptr;


csh CapstoneHandler;

void InstructionFetchHandler(besm::sim::Hart const& hart, void const* pBytecode) {
    besm::RV64UWord bytecode = *reinterpret_cast<besm::RV64UWord const*>(pBytecode);
    besm::RV64UDWord pc = hart.getState().read(besm::exec::GPRF::PC);

    *pLogStream << "[DebugTracer] Fetched bytecode " << std::hex << bytecode <<
        std::dec << " at pc = " << pc << ", disassembly:\n\t";

    cs_insn* instruction;
    size_t count = cs_disasm(CapstoneHandler, reinterpret_cast<uint8_t const*>(pBytecode),
        4, pc, 0, &instruction);

    if(count > 0) {
        *pLogStream << instruction->mnemonic << " " << instruction->op_str << std::endl;
    } else {
        *pLogStream << "[unable to disasm]" << std::endl;
    }

    cs_free(instruction, count);
}

void InstructionDecodeHandler(besm::sim::Hart const& hart, void const* pInstr) {
    // Too early to implement it
}

void InstructionExecHandler(besm::sim::Hart const& hart, void const*) {
    *pLogStream << "[DebugTracer] Instruction executed. Force dumping machine state..." 
        << std::endl;
    besm::exec::GPRFStateDumper(*pLogStream).dump(hart.getState());
}

}

extern "C" {

void init(besm::sim::HookManager::SPtr hookManager, std::string const& commandLine,
    std::ostream& defaultLogStream) {
    pLogStream = &defaultLogStream;

    *pLogStream << "[DebugTracer] DebugTracer plugin enabled, good luck in debugging!" 
        << std::endl;

    std::string logFilePath;

    CLI::App app;
    app.add_option("-l,--log-file", logFilePath, 
        "Dump logs to file (dumped to clog by default)");

    app.parse(commandLine, true);

    if(!logFilePath.empty()) {
        logFile.open(logFilePath);
        if(!logFile.is_open()) {
            *pLogStream << "[DebugTracer] Failed to open log file \'" <<
                logFilePath << "\', using clog stream instead" << std::endl;
        } else {
            *pLogStream << "[DebugTracer] Log file was set up" << std::endl;
            std::ref(*pLogStream) = logFile;
        }
    }

    cs_err csErr = cs_open(CS_ARCH_RISCV, CS_MODE_RISCV64, &CapstoneHandler);
    if(csErr != CS_ERR_OK) {
        *pLogStream << "[DebugTracer] Unable to initialize capstone engine, " 
            "disassembly will be unavailable" << std::endl;
        *pLogStream << "[DebugTracer] The error occured in Capstone Engine is \'" <<
            cs_strerror(csErr) << "\'" << std::endl;
    }

    hookManager->registerHook(besm::sim::HookManager::INSTRUCTION_FETCH,
        InstructionFetchHandler);
    hookManager->registerHook(besm::sim::HookManager::INSTRUCTION_DECODE,
        InstructionDecodeHandler);
    hookManager->registerHook(besm::sim::HookManager::INSTRUCTION_EXECUTE,
        InstructionExecHandler);
}

}

