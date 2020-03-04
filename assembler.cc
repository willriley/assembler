//
// Created by Will on 3/3/20.
//
#include "assembler.hh"
#include <cstdio>
#include <unitypes.h>
#include <vector>

namespace assembler {
    unsigned get_reg(const std::string &s) {
        if (s[0] == 'x') {
            return (unsigned) std::atoi(&s[1]);
        }
        try {
            return aliases.at(s);
        } catch (std::exception& ex) {
            throw std::runtime_error("invalid register specified");
        }
    }

    // addi, slli, slri, ori, andi
    void parse_itype(std::vector<std::string>& args) {
        int imm = std::atoi(args[3].c_str());

        if ((args[0] == "slli" || args[0] == "slri") && (imm < 0 || imm > 31)) {
            throw std::runtime_error("shift immediate value should be in range [0,32).");
        } else if (imm > 2047 || imm < -2048) {
            throw std::runtime_error("immediate value should be in range [-2048,2048).");
        }

        itype instr = {.opc = I_OPCODE,
                .rd = get_reg(args[1]),
                .rs1 = get_reg(args[2]),
                .f3 = handlers.at(args[0]).first,
                .imm = imm};
        printf("0x%08x\n", *(uint32_t *) &instr);
    }

    void parse(std::vector<std::string>& args) {
        // call handler for function
        parser p = handlers.at(args[0]).second;
        p(args);
    }
}


int main() {
    // addi a0, x0, 5
    using namespace assembler;
    itype i = {.imm = 5, .rs1 = 0, .f3 = 0, .rd = 10, .opc = I_OPCODE};
    printf("0x%08x\n", *(uint32_t *) &i);

    // addi a0, x0, 5
    std::vector<std::string> args = {"addi", "a0", "x0", "5"};
    parse(args);

    // ori a0, x0, 100
    itype j = {.imm = 100, .rs1 = 0, .f3 = 6, .rd = 10, .opc = I_OPCODE};
    printf("0x%08x\n", *(uint32_t *) &j);

    args = {"ori", "a0", "x0", "100"};
    parse(args);


    // add x5, x28, x11
    rtype k = {.f7 = 0, .rs2 = 11, .rs1 = 28, .f3 = 0, .rd = 5, .opc = R_OPCODE};
    printf("0x%08x\n", *(uint32_t *) &k);

    // here: beq x0, x0, here
    btype b = {.im1 = 0, .im2 = 0, .f3 = 0, .rs1 = 0, .rs2 = 0, .opc = B_OPCODE};
    printf("0x%08x\n", *(uint32_t *) &b);

    return 0;
}
