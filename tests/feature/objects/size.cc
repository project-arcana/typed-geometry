#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("Size")(tg::rng& rng)
{
    auto box1 = tg::aabb1(tg::pos1(-1.0f), tg::pos1(0.0f));
    auto r0 = uniform(rng, box1).x;

    box1 = tg::aabb1(tg::pos1(0.1f), tg::pos1(1.0f));
    auto r1 = uniform(rng, box1).x;

    box1 = tg::aabb1(tg::pos1(r0), tg::pos1(r1));
    auto box2 = tg::aabb2(tg::pos2(r0), tg::pos2(r1));
    auto box3 = tg::aabb3(tg::pos3(r0), tg::pos3(r1));

    {
        auto res = size_of(box1);
        CHECK(res.width == nx::approx(r1 - r0));
    }

    {
        auto res = size_of(box2);
        CHECK(res.width == nx::approx(r1 - r0));
    }

    {
        auto res = size_of(box3);
        CHECK(res.width == nx::approx(r1 - r0));
    }
}
