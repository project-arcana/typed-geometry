#include "asm_tester.hh"

#include <fstream>
#include <iostream>
#include <sstream>

#include <nexus/test.hh>

asm_tester::asm_tester(const std::string& code, const std::string& compiler, const std::string& args)
{
    std::string compile_cmd;
    std::string dump_cmd;

    std::string tmp_file = "/tmp/tg-test-asm";

    auto curr_dir = std::string(__FILE__);
    curr_dir = curr_dir.substr(0, curr_dir.rfind('/'));

    // write file
    {
        std::ofstream file(tmp_file + ".cc");
        file << code << "\n";
    }

    // assemble compile cmd
    {
        std::ostringstream ss;
        ss << compiler;
        ss << " " << args;
        ss << " -msse2";
        ss << " -O2";
        ss << " -I" << curr_dir << "/../src";
        ss << " -I" << curr_dir << "/../../clean-core/src";
        ss << " -c " << tmp_file << ".cc";
        ss << " -o " << tmp_file << ".o";
        compile_cmd = ss.str();
    }

    // assemble dump cmd
    {
        std::ostringstream ss;
        ss << "objdump -d --no-show-raw-insn ";
        ss << tmp_file << ".o";
        ss << " | grep \"^ \" | cut -f2,3 > ";
        ss << tmp_file << ".asm";
        dump_cmd = ss.str();
    }

    // std::cout << compile_cmd << std::endl;
    // std::cout << dump_cmd << std::endl;

    // execute
    auto c_ret = system(compile_cmd.c_str());
    CHECK(c_ret == 0);

    auto d_ret = system(dump_cmd.c_str());
    CHECK(d_ret == 0);

    // parse file
    std::ifstream file(tmp_file + ".asm");
    std::string line;
    while (std::getline(file, line))
    {
        auto p = line.find(' ');
        if (p == std::string::npos)
            continue;

        auto op = line.substr(0, p);
        auto p2 = line.rfind(' ');
        auto args = p == line.size() - 1 ? "" : line.substr(p2 + 1);

        instructions.emplace_back(op, args);
        icnt[op]++;

        auto starts_with = [&](std::string const& s) -> bool {
            if (s.size() > op.size())
                return false;
            return op.substr(0, s.size()) == s;
        };

        if (starts_with("mov"))
            stats.movs++;

        else if (starts_with("mul"))
            stats.muls++;

        else if (starts_with("add"))
            stats.adds++;

        else if (starts_with("call"))
            stats.calls++;

        else if (starts_with("jmp"))
            stats.jumps++;

        else if (starts_with("ret") || starts_with("push") || starts_with("pop"))
            stats.other++;

        else
        {
            dump();
            std::cerr << "unrecognized opcode '" << op << "'" << std::endl;
            CHECK(false);
        }
    }
}

void asm_tester::assert_limits(const asm_stats& limits, bool ignore_mov, int other_eps) const
{
    if (!ignore_mov)
        CHECK(stats.movs <= limits.movs);

    /*
    int movs = 0;
    int adds = 0;
    int muls = 0;
    int divs = 0;
    int calls = 0;
    int jumps = 0;
    int cond_jumps = 0;
    int sqrts = 0;
     */

    CHECK(stats.adds <= limits.adds);
    CHECK(stats.muls <= limits.muls);
    CHECK(stats.divs <= limits.divs);
    CHECK(stats.calls <= limits.calls);
    CHECK(stats.jumps <= limits.jumps);
    CHECK(stats.cond_jumps <= limits.cond_jumps);
    CHECK(stats.sqrts <= limits.sqrts);
    CHECK(stats.other <= limits.other + other_eps);
}

void asm_tester::dump() const
{
    for (auto const& kvp : instructions)
        std::cout << kvp.first << " " << kvp.second << std::endl;
}
