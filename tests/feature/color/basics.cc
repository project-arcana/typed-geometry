#include <nexus/test.hh>

#include <typed-geometry/feature/colors.hh>

TEST("color basics", disabled)
{
    // standard linear colors
    [[maybe_unused]] tg::color3 c0;
    [[maybe_unused]] tg::color4 c1;

    // compressed srgb colors
    [[maybe_unused]] tg::srgb8 c2;
    [[maybe_unused]] tg::srgba8 c3;
}

TEST("color literals")
{
    CHECK("fff"_color3 == tg::color3::white);
    CHECK("ff0000"_color4 == tg::color4::red);
    CHECK("0f0f"_color4 == tg::color4::green);
    CHECK("000"_srgb8 == tg::srgb8::black);
    CHECK("#00f"_srgba8 == tg::srgba8::blue);
}
