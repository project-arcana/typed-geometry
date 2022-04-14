#include <nexus/fuzz_test.hh>

TG_FUZZ_TEST(Triangle, Circles)
{
    auto const execute_test = [&](auto bb) {
        auto const p0 = uniform(rng, bb);
        auto const p1 = uniform(rng, bb);
        auto const p2 = uniform(rng, bb);

        auto const t = tg::triangle(p0, p1, p2);

        auto const p = tg::perimeter_of(t);

        CHECK(distance(p0, p1) + distance(p1, p2) + distance(p2, p0) == approx(p).epsilon(0.001f));

        //
        // circumcenter
        //
        if (tg::min_height_of(t) > 0.1f) // flat triangles have extreme/unstable circles
        {
            auto const cc = tg::circumcenter_of(t);
            auto const cs = tg::circumcircle_of(t);
            auto const cr = tg::circumradius_of(t);

            CHECK(distance(p0, cc) == approx(distance(p1, cc)).epsilon(0.001f));
            CHECK(distance(p1, cc) == approx(distance(p2, cc)).epsilon(0.001f));
            CHECK(distance(p2, cc) == approx(distance(p0, cc)).epsilon(0.001f));

            CHECK(cs.radius == approx(distance(p0, cc)).epsilon(0.001f));
            CHECK(cs.center == approx(cc, 0.001f));
            CHECK(cr == approx(cs.radius).epsilon(0.001f));
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
                CHECK(distance(e, ic) == approx(ir).epsilon(0.001f));

            CHECK(is.center == approx(ic, 0.001f));
            CHECK(ir == approx(is.radius).epsilon(0.001f));
        }
    };

    execute_test(tg::aabb2(-10, 10));
    execute_test(tg::aabb3(-10, 10));
}
