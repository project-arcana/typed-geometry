#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/vector.hh>

FUZZ_TEST("Subvector")(tg::rng& rng)
{
    // 1D
    {
        auto u = uniform_vec(rng, tg::aabb1(-5.f, 5.f));
        auto v = tg::subvector<0, 1>(u);

        CHECK(u == nx::approx(v));
    }

    // 2D
    {
        auto u = uniform_vec(rng, tg::aabb2(-5.f, 5.f));

        auto v = tg::subvector<0, 2>(u);
        CHECK(u == nx::approx(v));

        auto w = tg::subvector<0, 1>(u);
        CHECK(w.x == nx::approx(u.x));

        w = tg::subvector<1, 1>(u);
        CHECK(w.x == nx::approx(u.y));
    }

    // 3D
    {
        auto u = uniform_vec(rng, tg::aabb3(-5.f, 5.f));

        auto v = tg::subvector<0, 3>(u);
        CHECK(u == nx::approx(v));

        auto w = tg::subvector<0, 1>(u);
        CHECK(w.x == nx::approx(u.x));

        w = tg::subvector<1, 1>(u);
        CHECK(w.x == nx::approx(u.y));

        w = tg::subvector<2, 1>(u);
        CHECK(w.x == nx::approx(u.z));
    }

    // 4D
    {
        auto u = uniform_vec(rng, tg::aabb4(-5.f, 5.f));

        auto v = tg::subvector<0, 4>(u);
        CHECK(u == nx::approx(v));

        auto w = tg::subvector<0, 1>(u);
        CHECK(w.x == nx::approx(u.x));

        w = tg::subvector<1, 1>(u);
        CHECK(w.x == nx::approx(u.y));

        w = tg::subvector<2, 1>(u);
        CHECK(w.x == nx::approx(u.z));

        w = tg::subvector<3, 1>(u);
        CHECK(w.x == nx::approx(u.w));
    }
}
