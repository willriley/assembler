//
// Created by Will on 3/3/20.
//

#ifndef ASSEMBLER_ASSEMBLER_HH
#define ASSEMBLER_ASSEMBLER_HH
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

struct btype {
    unsigned int opc : 7;
    unsigned int im2 : 5; // imm[4:1], imm[11]
    unsigned int f3  : 3;
    unsigned int rs1 : 5;
    unsigned int rs2 : 5;
    unsigned int im1 : 7; // imm[12], imm[10:5]
};

// 1111 1110 0000 0000 0000 1011 1110 0011

const int I_OPCODE = 0b0010011;
const int R_OPCODE = 0b0110011;
const int B_OPCODE = 0b1100011;

#endif //ASSEMBLER_ASSEMBLER_HH
