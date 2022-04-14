#include <nexus/fuzz_test.hh>

TEST("norms")
{
    CHECK(frobenius_norm(tg::mat4::identity) == 2.0f);
    CHECK(frobenius_norm(tg::mat4::zero) == 0.0f);
    CHECK(frobenius_norm(tg::mat4::ones) == 4.0f);
}

FUZZ_TEST("Norm")(tg::rng& rng)
{
    {
        auto max_r = tg::exp(uniform(rng, -4.0f, 4.0f));

        auto range1 = tg::aabb1(tg::pos1(-max_r), tg::pos1(max_r));
        auto range2 = tg::aabb2(tg::pos2(-max_r), tg::pos2(max_r));
        auto range3 = tg::aabb3(tg::pos3(-max_r), tg::pos3(max_r));
        auto range4 = tg::aabb4(tg::pos4(-max_r), tg::pos4(max_r));

        auto v1 = uniform(rng, range1) - tg::pos1::zero;
        auto v2 = uniform(rng, range2) - tg::pos2::zero;
        auto v3 = uniform(rng, range3) - tg::pos3::zero;
        auto v4 = uniform(rng, range4) - tg::pos4::zero;

        CHECK(length(v1) == approx(norm(v1, 2.f)));
        CHECK(length(v2) == approx(norm(v2, 2.f)));
        CHECK(length(v3) == approx(norm(v3, 2.f)));
        CHECK(length(v4) == approx(norm(v4, 2.f)));
    }

    {
        auto p = uniform(rng, 1.0f, 10.0f);
        CHECK(norm(tg::vec3::unit_x, p) == approx(1.0f));
        CHECK(norm(tg::vec4::unit_z, p) == approx(1.0f));
        CHECK(norm(tg::dvec2::unit_y, double(p)) == approx(1.0f));
    }
}
