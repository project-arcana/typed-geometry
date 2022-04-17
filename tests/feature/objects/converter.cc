#include <nexus/test.hh>

#include <typed-geometry/feature/objects.hh>

TEST("object convert")
{
    {
        auto v = tg::aabb2(-10, 10);
        auto vv = tg::daabb2(v);
        CHECK(vv.min == tg::dpos2(-10));
    }
    {
        auto v = tg::box3({1, 2, 3}, tg::mat3::identity);
        auto vv = tg::dbox3(v);
        CHECK(volume_of(vv) == 8);
    }
    {
        auto v = tg::capsule3({1, 2, 3}, {2, 3, 4}, 4);
        auto vv = tg::dcapsule3(v);
        CHECK(vv.axis.pos1 == tg::dpos3(2, 3, 4));
    }
    {
        auto v = tg::cone3(tg::sphere2in3({1, 2, 3}, 1, {0, 1, 0}), 3);
        auto vv = tg::dcone3(v);
        CHECK(vv.height == 3);
    }
    {
        auto v = tg::cylinder3({1, 2, 3}, {2, 3, 4}, 4);
        auto vv = tg::dcylinder3(v);
        CHECK(vv.axis.pos1 == tg::dpos3(2, 3, 4));
    }
    {
        auto v = tg::halfspace3({0, 1, 0}, 7);
        auto vv = tg::dhalfspace3(v);
        CHECK(vv.dis == 7);
    }
    {
        auto v = tg::plane3({0, 1, 0}, 7);
        auto vv = tg::dplane3(v);
        CHECK(vv.dis == 7);
    }
    {
        auto v = tg::hemisphere3({1, 2, 3}, 4, {0, 1, 0});
        auto vv = tg::dhemisphere3(v);
        CHECK(vv.radius == 4);
    }
    {
        auto v = tg::sphere3({1, 2, 3}, 4);
        auto vv = tg::dsphere3(v);
        CHECK(vv.radius == 4);
    }
    {
        auto v = tg::inf_cone3({1, 2, 3}, {0, 1, 0}, 10_deg);
        auto vv = tg::dinf_cone3(v);
        CHECK(vv.apex == tg::dpos3(1, 2, 3));
    }
    {
        auto v = tg::inf_cylinder3({{1, 2, 3}, {1, 0, 0}}, 4);
        auto vv = tg::dinf_cylinder3(v);
        CHECK(vv.radius == 4);
    }
    {
        auto v = tg::line3({1, 2, 3}, {1, 0, 0});
        auto vv = tg::dline3(v);
        CHECK(vv.pos == tg::dpos3(1, 2, 3));
    }
    {
        auto v = tg::ray3({1, 2, 3}, {1, 0, 0});
        auto vv = tg::dray3(v);
        CHECK(vv.origin == tg::dpos3(1, 2, 3));
    }
    {
        auto v = tg::segment3({1, 2, 3}, {4, 5, 6});
        auto vv = tg::dsegment3(v);
        CHECK(vv.pos0 == tg::dpos3(1, 2, 3));
    }
    {
        auto v = tg::triangle3({1, 2, 3}, {4, 5, 6}, {7, 8, 9});
        auto vv = tg::dtriangle3(v);
        CHECK(vv.pos0 == tg::dpos3(1, 2, 3));
    }
    {
        auto v = tg::quad3({1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12});
        auto vv = tg::dquad3(v);
        CHECK(vv.pos00 == tg::dpos3(1, 2, 3));
    }
}
