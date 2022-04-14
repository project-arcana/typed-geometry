#include <nexus/test.hh>

TG_FUZZ_TEST(Plane, Intersect)
{
    auto bounds = tg::aabb3(-10, 10);

    // plane - plane
    {
        auto const p0 = tg::plane(tg::uniform<tg::dir3>(rng), uniform(rng, bounds));
        auto const p1 = tg::plane(tg::uniform<tg::dir3>(rng), uniform(rng, bounds));

        auto const l = intersection(p0, p1);

        auto const p = l[uniform(rng, -10.0f, 10.0f)];

        CHECK(distance(p, p0) == approx(0));
        CHECK(distance(p, p1) == approx(0));
    }
}
