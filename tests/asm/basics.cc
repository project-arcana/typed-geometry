#ifndef _MSC_VER
#include <asm_test.hh>

TEST("TypedGeometryAsm.Basics")
{
    asm_tester asm_test("float foo(float f) { return f * f + f; }", "clang");
    asm_stats as;
    as.adds = 1;
    as.muls = 1;
    asm_test.assert_limits(as);
    // asm_test.dump();
}

TEST("TypedGeometryAsm.Empty")
{
    asm_tester asm_test("#include <typed-geometry/tg.hh>", "clang", "-std=c++17");
    asm_test.assert_limits({}, false, 0);
}

TG_ASM_TEST(TypedGeometryAsm, BasicTG, "float foo(float f) { return f * f; }")
{
    asm_stats s;
    s.muls = 1;
    asm_test.assert_limits(s);
}

TG_ASM_TEST(TypedGeometryAsm, VecConstNoDiv, "tg::vec3 foo(tg::vec3 v) { return v / 3; }")
{
    asm_stats s;
    s.muls = 3;
    asm_test.assert_limits(s);
}
#endif
