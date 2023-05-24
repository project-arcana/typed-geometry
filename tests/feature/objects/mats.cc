#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/matrix.hh>

FUZZ_TEST("MatrixBasics")(tg::rng& rng)
{
    auto range3 = tg::aabb3(tg::pos3(-2.0f), tg::pos3(2.0f));

    {
        tg::mat3 m;
        tg::vec3 v;
        while (v == tg::vec3::zero)
            v = uniform(rng, range3) - tg::pos3::zero;

        CHECK(length(v) >= 0.0f);
        CHECK(m * v == tg::vec3());

        m = tg::identity<tg::mat3>();
        CHECK(m * v == v);

        m = tg::diag<tg::mat3>(2.0f);
        CHECK(m * v == v * 2.0f);
    }
    {
        tg::mat3 m;
        tg::vec3 v;
        while (v == tg::vec3::zero)
            v = uniform(rng, range3) - tg::pos3::zero;

        CHECK(length(v) >= 0.0f);
        CHECK(m * v == tg::vec3());

        m = tg::mat3::identity;
        CHECK(m * v == v);

        m = tg::mat3::diag(2.0f);
        CHECK(m * v == v * 2.0f);
    }
    {
        tg::imat4 m;
        CHECK(m + 1 == tg::imat4::ones);
    }
    {
        tg::mat4 m = tg::mat4::diag(tg::vec4(3, 2, 4, 1));
        CHECK(m * tg::vec3(2, 3, 4) == tg::vec3(6, 6, 16));
    }
}
