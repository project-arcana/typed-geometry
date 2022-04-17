#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/vector.hh>

FUZZ_TEST("Tests - Vec")(tg::rng& rng)
{
    CHECK(is_zero_vector(tg::vec3::zero));

    {
        auto a = uniform_vec(rng, tg::sphere_boundary<3, float>::unit);
        auto b = uniform_vec(rng, tg::sphere_boundary<3, float>::unit);
        CHECK(is_normalized(a));
        CHECK(is_normalized(b));

        auto c = cross(a, b);
        CHECK(are_orthogonal(a, c));
        CHECK(are_orthogonal(b, c));

        auto d = tg::vec3(normalize(c));
        CHECK(are_orthonormal(a, d));
        CHECK(are_orthonormal(b, d));

        CHECK(is_zero_vector(cross(a, a)));
        CHECK(is_zero_vector(cross(b, b)));
        CHECK(is_zero_vector(cross(c, c)));
    }

    {
        auto bb = tg::aabb3(tg::pos3(-5.0f), tg::pos3(5.0f));

        auto a = uniform_vec(rng, bb);
        auto b = uniform_vec(rng, bb);
        auto c = cross(a, b);

        CHECK(are_orthogonal(a, c));
        CHECK(are_orthogonal(b, c));

        auto d = tg::vec3(normalize(c));
        auto e = d * uniform(rng, -2.0f, 2.0f);

        CHECK(are_collinear(c, d));
        CHECK(are_collinear(c, e));
        CHECK(are_collinear(d, e));
    }
}
