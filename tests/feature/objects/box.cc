#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

TEST("BoxBasics")
{
    auto b = tg::aabb2({-1, 0}, {3, 4});
    CHECK(contains(b, tg::pos2{-1, 0}));
    CHECK(contains(b, tg::pos2{3, 4}));
    CHECK(contains(b, tg::pos2{2, 2}));
    CHECK(centroid_of(b) == tg::pos2(1, 2));
}

FUZZ_TEST("BoxFuzz")(tg::rng& rng)
{
    auto ab = tg::aabb3(tg::pos3(-10) + uniform_vec(rng, tg::sphere3::unit) * 5.f, //
                        tg::pos3(10) + uniform_vec(rng, tg::sphere3::unit) * 5.f);

    auto ob = tg::box3(ab);

    CHECK(volume_of(ob) == nx::approx(volume_of(ab)));

    auto p = uniform(rng, ob);
    CHECK(contains(ob, p));

    auto p2 = uniform(rng, tg::sphere3({0, 0, 0}, 2));
    CHECK(contains(ob, p2) == contains(ab, p2));
}
