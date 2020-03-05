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
            printf("reg %d\n", std::atoi(&s[1]));
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

    // add, xor, mul
    // add ra, x0, x0
    void parse_rtype(std::vector<std::string>& args) {
        rtype instr = {.opc = R_OPCODE,
                       .rd = get_reg(args[1]),
                       .rs1 = get_reg(args[2]),
                       .rs2 = get_reg(args[3]),
                       .f3 = handlers.at(args[0]).first,
                       .f7 = static_cast<unsigned int>(args[0] == "mul" ? 1 : 0)};

        printf("0x%08x\n", *(uint32_t *) &instr);
    }

    // beq, bne
    // beq x0, x0, 0
    void parse_btype(std::vector<std::string>& args) {
        int offset = std::atoi(args[3].c_str());

        printf("offset %d\n", offset);
        btype instr = {.opc = B_OPCODE,
                       .rs1 = get_reg(args[1]),
                       .rs2 = get_reg(args[2]),
                       .f3  = handlers.at(args[0]).first};

        // set im1 based on offset[12] and offset[10:5]
        instr.im1 = (offset & 0x000007e0) >> 5;
        if (offset & 0x00001000) {
            instr.im1 |= 0x40;
        }

        // set im2 based on offset[4:1] and offset[11]
        instr.im2 = offset & 0x0000001e;
        if (offset & 0x00000800) {
            instr.im2++;
        }
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

    // ori a0, x0, 2033
    itype j = {.imm = 2033, .rs1 = 0, .f3 = 6, .rd = 10, .opc = I_OPCODE};
    printf("0x%08x\n", *(uint32_t *) &j);

    args = {"ori", "a0", "x0", "2033"};
    parse(args);

    /**
     * rtype tests
     * mul x5, x25, x17
     * add x7, x4, x29
     * xor x9, x13, x30
     */
    args = {"mul", "x5", "x25", "x17"};
    parse(args);
    args = {"add", "x7", "x4", "x29"};
    parse(args);
    args = {"xor", "x9", "x13", "x30"};
    parse(args);

    /**
     * btype tests
     * beq x0 x0 8
     * beq x0 x0 -4
     */
    args = {"beq", "x0", "x0", "172"};
    parse(args);

    args = {"beq", "x0", "x0", "-4"};
    parse(args);

    // add x5, x28, x11
//    rtype k = {.f7 = 0, .rs2 = 11, .rs1 = 28, .f3 = 0, .rd = 5, .opc = R_OPCODE};
//    printf("0x%08x\n", *(uint32_t *) &k);

    // here: beq x0, x0, here
//    btype b = {.im1 = 0, .im2 = 0, .f3 = 0, .rs1 = 0, .rs2 = 0, .opc = B_OPCODE};
//    printf("0x%08x\n", *(uint32_t *) &b);

    return 0;
}
