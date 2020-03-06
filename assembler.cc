//
// Created by Will on 3/3/20.
//
#include "assembler.hh"
#include <cstdio>
#include <unitypes.h>
#include <fstream>
#include <sstream>

namespace assembler {

    void trim_space(std::string &in) {
        auto begin = in.find_first_not_of(' ');
        if (begin == std::string::npos) {
            in = "";
            return;
        }

        in = in.substr(begin);
        auto end = in.find_first_of(' ');
        if (end == std::string::npos) {
            return;
        }

        in = in.substr(0, end);
    }

    unsigned get_reg(const std::string &s) {
        if (s[0] == 'x') {
            return (unsigned) std::atoi(&s[1]);
        }
        try {
            return aliases.at(s);
        } catch (const std::exception& ex) {
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
                       .f3 = cmds.at(args[0]).first,
                       .imm = imm};
        printf("0x%08x\n", *(uint32_t *) &instr);
    }

    // add, xor, mul, sll, srl
    // add ra, x0, x0
    void parse_rtype(std::vector<std::string>& args) {
        rtype instr = {.opc = R_OPCODE,
                       .rd = get_reg(args[1]),
                       .rs1 = get_reg(args[2]),
                       .rs2 = get_reg(args[3]),
                       .f3 = cmds.at(args[0]).first,
                       .f7 = static_cast<unsigned int>(args[0] == "mul" ? 1 : 0)};

        printf("0x%08x\n", *(uint32_t *) &instr);
    }

    // beq, bne
    // beq x0, x0, 0
    void parse_btype(std::vector<std::string>& args) {
        int offset = std::atoi(args[3].c_str());
        if (offset > 4095 || offset < -4096) {
            throw std::runtime_error("branch offsets should be in range [-4096, 4096)");
        }

        btype instr = {.opc = B_OPCODE,
                       .rs1 = get_reg(args[1]),
                       .rs2 = get_reg(args[2]),
                       .f3  = cmds.at(args[0]).first};

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
        try {
            parser p = cmds.at(args[0]).second;
            p(args);
        } catch (const std::exception& e) {
            throw std::runtime_error("unsupported instruction " + args[0]);
        }
    }

    void parse_file(const std::string& name) {
        std::ifstream file(name);
        std::string line;

        while (std::getline(file, line)) {
            // skip past label
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                line = line.substr(pos+1);
                pos = line.find_first_not_of(' ');
                line = pos != std::string::npos ? line.substr(pos) : "";
            }

            // skip empty lines, labels (e.g. "label:"), and comments (e.g. "# comment")
            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::vector<std::string> instr;
            std::istringstream ss(line);
            std::string token;

            std::getline(ss, token, ' ');
            instr.push_back(token);

            for (int i=0; i<3; i++) {
                if (!std::getline(ss, token, ',')) {
                    throw std::runtime_error("command doesn't have 3 args");
                }
                trim_space(token);
                instr.push_back(token);
            }

            parse(instr);
        }
    }
}


int main(int argc, char *argv[]) {
    using namespace assembler;
    if (argc <= 1) {
        printf("please specify an input file\n");
        return 0;
    }

    parse_file(std::string(argv[1]));
    return 0;
}
