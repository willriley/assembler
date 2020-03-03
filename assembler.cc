//
// Created by Will on 3/3/20.
//
#include <iostream>
#include <cstdio>

// arrange this
struct itype {
    unsigned int opc : 7;
    unsigned int rd  : 5;
    unsigned int f3  : 3;
    unsigned int rs1 : 5;
    unsigned int imm : 12;
};

struct rtype {
    unsigned int opc : 7;
    unsigned int rd  : 5;
    unsigned int f3  : 3;
    unsigned int rs1 : 5;
    unsigned int rs2 : 5;
    unsigned int f7  : 7;
};

const int I_OPCODE = 0b010011;
const int R_OPCODE = 0b110011;


int main() {
    // addi a0, x0, 5
    itype i = {.imm = 5, .rs1 = 0, .f3 = 0, .rd = 10, .opc = I_OPCODE};
    std::cout << std::hex << *(uint32_t *) &i << std::endl;
    printf("0x%08x\n", *(uint32_t *) &i);

    // ori a0, x0, 100
    itype j = {.imm = 100, .rs1 = 0, .f3 = 6, .rd = 10, .opc = I_OPCODE};
    std::cout << std::hex << *(uint32_t *) &j << std::endl;
    printf("0x%08x\n", *(uint32_t *) &j);

    // add x5, x28, x11
    rtype k = {.f7 = 0, .rs2 = 11, .rs1 = 28, .f3 = 0, .rd = 5, .opc = R_OPCODE};
    printf("0x%08x\n", *(uint32_t *) &k);
    return 0;
}
