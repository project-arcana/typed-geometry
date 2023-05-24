#include <nexus/ext/tg-approx.hh>
#include <nexus/test.hh>

#include <typed-geometry/feature/bezier.hh>
#include <typed-geometry/feature/objects.hh>
#include <typed-geometry/feature/random.hh>

TEST("bezier basics")
{
    auto p0 = tg::pos3(1, 0, 0);
    auto p1 = tg::pos3(0, 1, 0);
    auto p2 = tg::pos3(0, 0, 1);
    auto p3 = tg::pos3(1, 1, 1);

    auto b = tg::make_bezier(p0, p1, p2, p3);

    auto bb = tg::aabb_of(p0, p1, p2, p3);

    CHECK(b[0.0f] == nx::approx(p0));
    CHECK(b[1.0f] == nx::approx(p3));

    tg::rng rng;
    for (auto i = 0; i < 100; ++i)
    {
        auto t = uniform(rng, 0.0f, 1.0f);
        CHECK(contains(bb, b[t]));
    }
}
