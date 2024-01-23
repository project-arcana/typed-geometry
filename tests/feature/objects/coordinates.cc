#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>
#include <typed-geometry/feature/random.hh>

FUZZ_TEST("Coordinates")(tg::rng& rng)
{
    auto range1 = tg::aabb1(tg::pos1(0), tg::pos1(1));
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));

    {
        auto t = tg::triangle2(uniform(rng, range2), uniform(rng, range2), uniform(rng, range2));
        if (area_of(t) < 0.001f)
            return; // unreliable

        auto center = centroid_of(t);

        auto coordinates = tg::coordinates(t, center);

        // barycentric coordinates should all be the same for centroid
        CHECK(coordinates[0] == nx::approx(coordinates[1]).abs(0.1f));
        CHECK(coordinates[1] == nx::approx(coordinates[2]).abs(0.1f));
    }

    {
        auto start = uniform(rng, range3);
        auto end = uniform(rng, range3);
        if (distance(start, end) < 0.001f)
            return; // unreliable

        auto l = tg::segment3(start, end);

        // check for points in between if coordinates correct
        auto s = uniform(rng, range1).x;
        auto between = start + (end - start) * s;
        CHECK(coordinates(l, between) == nx::approx(s).abs(0.01f));

        // start and end explicitly checked
        CHECK(coordinates(l, end) == nx::approx(1.f).abs(0.01f));
        CHECK(coordinates(l, start) == nx::approx(0.f).abs(0.001f));
    }
}
