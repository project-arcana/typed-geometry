#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/matrix.hh>

FUZZ_TEST("Scaling")(tg::rng& rng)
{
    auto box1 = tg::aabb1(tg::pos1(-1.0f), tg::pos1(1.0f));
    auto box2 = tg::aabb2(tg::pos2(-1.0f), tg::pos2(1.0f));
    auto box3 = tg::aabb3(tg::pos3(-1.0f), tg::pos3(1.0f));

    // multiplying with identities
    {
        auto identity = tg::scaling(tg::size1(1.0f));
        auto u = uniform(rng, box1);

        auto v = identity * u;

        CHECK(u == nx::approx(v));
    }
    {
        auto identity = tg::scaling(tg::size2(1.0f));
        auto u = uniform(rng, box2);

        auto v = identity * u;

        CHECK(u == nx::approx(v));
    }
    {
        auto identity = tg::scaling(tg::size3(1.0f));
        auto u = uniform(rng, box3);

        auto v = identity * u;

        CHECK(u == nx::approx(v));
    }

    // scaling vector and comparing lengths
    auto r = tg::abs(uniform(rng, box1).x) * 10.0f;

    {
        auto mat = tg::scaling(tg::size1(r));

        auto v = tg::vec1(uniform(rng, box1));

        auto l0 = length(v);
        auto l1 = length(mat * v);

        CHECK(l0 * r == nx::approx(l1));
    }
    {
        auto mat = tg::scaling(tg::size2(r));

        auto v = tg::vec2(uniform(rng, box1));

        auto l0 = length(v);
        auto l1 = length(mat * v);

        CHECK(l0 * r == nx::approx(l1));
    }
    {
        auto mat = tg::scaling(tg::size3(r));

        auto v = tg::vec3(uniform(rng, box3));

        auto l0 = length(v);
        auto l1 = length(mat * v);

        CHECK(l0 * r == nx::approx(l1));
    }
}
