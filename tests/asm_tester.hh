#pragma once

#include <map>
#include <string>
#include <vector>

struct asm_stats
{
    int movs = 0;
    int adds = 0;
    int muls = 0;
    int divs = 0;
    int calls = 0;
    int jumps = 0;
    int cond_jumps = 0;
    int sqrts = 0;
    int other = 0;
};

struct asm_tester
{
    // instruction stats
    asm_stats stats;

    // instruction counts
    std::map<std::string, int> icnt;

    // [op, args]
    std::vector<std::pair<std::string, std::string>> instructions;

    asm_tester(std::string const& code, std::string const& compiler, std::string const& args = "");

    void assert_limits(asm_stats const& limits, bool ignore_mov = true, int other_eps = 10) const;

    void dump() const;
};
