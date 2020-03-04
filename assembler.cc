//
// Created by Will on 3/3/20.
//
#include "assembler.hh"
#include <cstdio>
#include <unitypes.h>



int main() {
    // addi a0, x0, 5
    itype i = {.imm = 5, .rs1 = 0, .f3 = 0, .rd = 10, .opc = I_OPCODE};
    printf("0x%08x\n", *(uint32_t *) &i);

    // ori a0, x0, 100
    itype j = {.imm = 100, .rs1 = 0, .f3 = 6, .rd = 10, .opc = I_OPCODE};
    printf("0x%08x\n", *(uint32_t *) &j);

    // add x5, x28, x11
    rtype k = {.f7 = 0, .rs2 = 11, .rs1 = 28, .f3 = 0, .rd = 5, .opc = R_OPCODE};
    printf("0x%08x\n", *(uint32_t *) &k);

    // here: beq x0, x0, here
    btype b = {.im1 = 0, .im2 = 0, .f3 = 0, .rs1 = 0, .rs2 = 0, .opc = B_OPCODE};
    printf("0x%08x\n", *(uint32_t *) &b);
    return 0;
}
