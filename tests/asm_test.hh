#pragma once

#include <nexus/test.hh>

#include <ctracer/trace.hh>

#include "asm_tester.hh"

#define TG_ASM_TEST(test_case_name, test_name, CODE)                             \
    void tg_asm_test_##test_case_name##_##test_name(asm_tester const& asm_test); \
    TEST(#test_case_name "." #test_name)                                         \
    {                                                                            \
        TRACE("Asm Test " #test_case_name #test_name);                           \
        std::string code = "#include <typed-geometry/tg.hh>\n";                  \
        code += CODE;                                                            \
        asm_tester asm_test(code, "clang", "-std=c++17");                        \
        tg_asm_test_##test_case_name##_##test_name(asm_test);                    \
    }                                                                            \
    void tg_asm_test_##test_case_name##_##test_name(asm_tester const& asm_test)
