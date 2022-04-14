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

            CHECK(distance(p0, cc) == nx::approx(distance(p1, cc)));
            CHECK(distance(p1, cc) == nx::approx(distance(p2, cc)));
            CHECK(distance(p2, cc) == nx::approx(distance(p0, cc)));

            CHECK(cs.radius == nx::approx(distance(p0, cc)));
            CHECK(cs.center == nx::approx(cc));
            CHECK(cr == nx::approx(cs.radius));
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
                CHECK(distance(e, ic) == nx::approx(ir));

            CHECK(is.center == nx::approx(ic).abs(0.001f));
            CHECK(ir == nx::approx(is.radius));
        }
    };

    execute_test(tg::aabb2(-10, 10));
    execute_test(tg::aabb3(-10, 10));
}
