//
// Created by Will on 3/3/20.
//

#ifndef ASSEMBLER_ASSEMBLER_HH
#define ASSEMBLER_ASSEMBLER_HH

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <utility>

namespace assembler {

    const int I_OPCODE = 0b0010011;
    const int R_OPCODE = 0b0110011;
    const int B_OPCODE = 0b1100011;

    struct itype {
        unsigned int opc : 7;
        unsigned int rd  : 5;
        unsigned int f3  : 3;
        unsigned int rs1 : 5;
        int imm : 12; // signed immediate
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

    // convert reg name to value (e.g. x28 to 28, ra to 2)
    unsigned get_reg(const std::string &s);

    // parser function type alias
    // takes in a vector of strings and returns nothing (void)
    using parser = std::function<void(std::vector<std::string> &)>;

    void parse_itype(std::vector<std::string> &args);
    void parse_rtype(std::vector<std::string> &args);
    void parse_btype(std::vector<std::string> &args);

    // map instruction to pair of <f3 code, handler function>
    const std::unordered_map<std::string, std::pair<unsigned, parser>> cmds = {
            {"addi", std::make_pair<unsigned, parser>(0, parse_itype)},
            {"slli", std::make_pair<unsigned, parser>(1, parse_itype)},
            {"srli", std::make_pair<unsigned, parser>(5, parse_itype)},
            {"ori", std::make_pair<unsigned, parser>(6, parse_itype)},
            {"andi", std::make_pair<unsigned, parser>(7, parse_itype)},
            {"add", std::make_pair<unsigned, parser>(0, parse_rtype)},
            {"xor", std::make_pair<unsigned, parser>(4, parse_rtype)},
            {"mul", std::make_pair<unsigned, parser>(0, parse_rtype)},
            {"beq", std::make_pair<unsigned, parser>(0, parse_btype)},
            {"bne", std::make_pair<unsigned, parser>(1, parse_btype)},};

    // map of all register aliases
    const std::unordered_map<std::string, unsigned> aliases = {{"zero", 0},
                                                               {"ra",   1},
                                                               {"sp",   2},
                                                               {"gp",   3},
                                                               {"tp",   4},
                                                               {"t0",   5},
                                                               {"t1",   6},
                                                               {"t2",   7},
                                                               {"t3",   28},
                                                               {"t4",   29},
                                                               {"t5",   30},
                                                               {"t6",   31},
                                                               {"fp",   8},
                                                               {"s0",   8},
                                                               {"s1",   9},
                                                               {"s2",   18},
                                                               {"s3",   19},
                                                               {"s4",   20},
                                                               {"s5",   21},
                                                               {"s6",   22},
                                                               {"s7",   23},
                                                               {"s8",   24},
                                                               {"s9",   25},
                                                               {"s10",  26},
                                                               {"s11",  27},
                                                               {"a0",   10},
                                                               {"a1",   11},
                                                               {"a2",   12},
                                                               {"a3",   13},
                                                               {"a4",   14},
                                                               {"a5",   15},
                                                               {"a6",   16},
                                                               {"a7",   17}};
}
#endif //ASSEMBLER_ASSEMBLER_HH
