#include <nexus/test.hh>

#include <typed-geometry/feature/fixed_int.hh>
#include <typed-geometry/tg.hh>

TEST("tg pretty printer")
{
    auto p3 = tg::pos3(1, 2, 3);
    auto v3 = tg::vec3(2, 3.5, 4);
    auto v4 = tg::vec4(2, 3, 4.5, 5);
    auto d3 = normalize(v3);
    auto d4 = normalize(v4);

    auto ip3 = tg::ipos3(1, 2, 3);
    auto iv3 = tg::ivec3(2, 3, 4);
    auto iv4 = tg::ivec4(2, 3, 4, 5);

    auto s3 = tg::size3(2, 3.5, 4);
    auto is3 = tg::size3(3, 4, 5);

    auto c3 = tg::color3(1, 0, 1);
    auto c4 = tg::color4(1, 0, 1, 1);

    auto bb3 = tg::aabb3({1, 2, 3}, {4, 6, 7});

    auto sp = tg::span<float>(&bb3.min.x, 6);
    auto a = tg::array<float, 4>{2, 3, 4, 5};

    auto angle = 45_deg;
    auto angle2 = 10 * angle;

    auto i128 = tg::i128(1) << 100;
    auto i192 = tg::i192(1) << 180;
    auto i256 = tg::i256(1) << 230;
    auto u128 = tg::u128(1) << 100;
    auto u192 = tg::u192(1) << 180;
    auto u256 = tg::u256(1) << 230;
    auto in128 = -i128;
    auto in192 = -i192;
    auto in256 = -i256;
    auto im128 = tg::i128(-1);
    auto im192 = tg::i192(-1);
    auto im256 = tg::i256(-1);

    CHECK(true); // not a real test
}
