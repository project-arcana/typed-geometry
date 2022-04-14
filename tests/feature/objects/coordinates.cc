#include <nexus/test.hh>

#include <typed-geometry/feature/random.hh>
#include <typed-geometry/feature/objects.hh>

TG_FUZZ_TEST(TypedGeometry, Coordinates)
{
    auto range1 = tg::aabb1(tg::pos1(0), tg::pos1(1));
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));

    {
        auto t = tg::triangle2(uniform(rng, range2), uniform(rng, range2), uniform(rng, range2));
        auto center = centroid_of(t);

        auto coordinates = tg::coordinates(t, center);

        // barycentric coordinates should all be the same for centroid
        CHECK(coordinates[0] == approx(coordinates[1]).epsilon(1e-3));
        CHECK(coordinates[1] == approx(coordinates[2]).epsilon(1e-3));
    }

    {
        auto start = uniform(rng, range3);
        auto end = uniform(rng, range3);
        while(end == start)
            end = uniform(rng, range3);
        auto l = tg::segment3(start, end);

        // check for points in between if coordinates correct
        auto s = uniform(rng, range1).x;
        auto between = start + (end - start) * s;
        CHECK(coordinates(l, between) == approx(s));

        // start and end explicitly checked
        CHECK(coordinates(l, end) == approx(1));
        CHECK(coordinates(l, start) == approx(0));
    }
}
