#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("Triangle - Circles")(tg::rng& rng)
{
    auto const execute_test = [&](auto bb)
    {
        auto const p0 = uniform(rng, bb);
        auto const p1 = uniform(rng, bb);
        auto const p2 = uniform(rng, bb);

        auto const t = tg::triangle(p0, p1, p2);

        auto const p = tg::perimeter_of(t);

        CHECK(distance(p0, p1) + distance(p1, p2) + distance(p2, p0) == nx::approx(p));

        //
        // circumcenter
        //
        if (tg::min_height_of(t) > 0.1f) // flat triangles have extreme/unstable circles
        {
            auto const cc = tg::circumcenter_of(t);
            auto const cs = tg::circumcircle_of(t);
            auto const cr = tg::circumradius_of(t);

            CHECK(distance(p0, cc) == nx::approx(distance(p1, cc)).abs(0.01f));
            CHECK(distance(p2, cc) == nx::approx(distance(p0, cc)).abs(0.01f));
            CHECK(distance(p1, cc) == nx::approx(distance(p2, cc)).abs(0.01f));

            CHECK(cs.radius == nx::approx(distance(p0, cc)).abs(0.01f));
            CHECK(cs.center == nx::approx(cc).abs(0.01f));
            CHECK(cr == nx::approx(cs.radius).abs(0.01f).rel(0.01f));
        }

        //
        // incenter
        //
        if (tg::min_height_of(t) > 0.1f) // flat triangles have unstable circles
        {
            auto const ic = tg::incenter_of(t);
            auto const is = tg::incircle_of(t);
            auto const ir = tg::inradius_of(t);

            for (auto e : tg::edges_of(t))
                CHECK(distance(e, ic) == nx::approx(ir).abs(0.01f));

            CHECK(is.center == nx::approx(ic).abs(0.01f));
            CHECK(ir == nx::approx(is.radius).abs(0.01f));
        }
    };

    execute_test(tg::aabb2(-10, 10));
    execute_test(tg::aabb3(-10, 10));
}
