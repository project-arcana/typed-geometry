#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/vector.hh>

FUZZ_TEST("Normalize")(tg::rng& rng)
{
    // float
    {
        auto max_r = tg::exp(uniform(rng, -4.0f, 4.0f));

        auto range1 = tg::aabb1(tg::pos1(-max_r), tg::pos1(max_r));
        auto range2 = tg::aabb2(tg::pos2(-max_r), tg::pos2(max_r));
        auto range3 = tg::aabb3(tg::pos3(-max_r), tg::pos3(max_r));

        auto v1 = uniform(rng, range1) - tg::pos1::zero;
        auto v2 = uniform(rng, range2) - tg::pos2::zero;
        auto v3 = uniform(rng, range3) - tg::pos3::zero;

        if (max_r < 0.5f)
        {
            CHECK(!is_normalized(v1));
            CHECK(!is_normalized(v2));
            CHECK(!is_normalized(v3));
        }
    }

    // double
    {
        auto max_r = tg::exp(uniform(rng, -4.0, 4.0));

        auto range1 = tg::daabb1(tg::dpos1(-max_r), tg::dpos1(max_r));
        auto range2 = tg::daabb2(tg::dpos2(-max_r), tg::dpos2(max_r));
        auto range3 = tg::daabb3(tg::dpos3(-max_r), tg::dpos3(max_r));

        auto v1 = uniform(rng, range1) - tg::dpos1::zero;
        auto v2 = uniform(rng, range2) - tg::dpos2::zero;
        auto v3 = uniform(rng, range3) - tg::dpos3::zero;

        if (max_r < 0.5f)
        {
            CHECK(!is_normalized(v1));
            CHECK(!is_normalized(v2));
            CHECK(!is_normalized(v3));
        }
    }
}

FUZZ_TEST("BigNormalize")(tg::rng& rng)
{
    // float
    {
        auto max_r = tg::pow(10.f, uniform(rng, -18.0f, 18.0f));

        auto range1 = tg::aabb1(-max_r, max_r);
        auto range2 = tg::aabb2(-max_r, max_r);
        auto range3 = tg::aabb3(-max_r, max_r);

        auto v0 = uniform_vec(rng, range1);
        auto v1 = uniform_vec(rng, range2);
        auto v2 = uniform_vec(rng, range3);

        if (length(v0) <= 1e-20f)
            return;
        if (length(v1) <= 1e-20f)
            return;
        if (length(v2) <= 1e-20f)
            return;

        auto d0 = normalize(v0);
        auto d1 = normalize(v1);
        auto d2 = normalize(v2);

        CHECK(tg::detail::is_dir_valid(d0));
        CHECK(tg::detail::is_dir_valid(d1));
        CHECK(tg::detail::is_dir_valid(d2));

        CHECK(dot(d0, d0) == nx::approx(1.f).abs(0.01f));
        CHECK(dot(d1, d1) == nx::approx(1.f).abs(0.01f));
        CHECK(dot(d2, d2) == nx::approx(1.f).abs(0.01f));
    }
}
