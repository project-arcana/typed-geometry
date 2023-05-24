#include <nexus/fuzz_test.hh>

#include <nexus/ext/tg-approx.hh>

#include <clean-core/any_of.hh>

#include <typed-geometry/feature/matrix.hh>
#include <typed-geometry/feature/quat.hh>
#include <typed-geometry/feature/random.hh>

FUZZ_TEST("matrix decompose")(tg::rng& rng)
{
    auto const translation = uniform_vec(rng, tg::aabb3(-10, 10));
    auto scale = tg::size3(uniform(rng, tg::aabb3(0.1f, 2)));
    auto const rotation = tg::uniform<tg::quat>(rng);

    auto is_uniform = bool(uniform(rng));
    if (is_uniform)
        scale.height = scale.depth = scale.width;

    auto const M = tg::translation(translation) * tg::mat4(rotation) * tg::scaling(scale);

    // decompose
    {
        tg::vec3 t;
        tg::mat3 R;
        tg::size3 s;
        tg::decompose_transformation(M, t, R, s);

        auto const M2 = tg::translation(t) * tg::mat4(R) * tg::scaling(s);
        CHECK(M2 == nx::approx(M));
        CHECK(t == nx::approx(translation));
        CHECK(R == nx::approx(tg::mat3(rotation)));
        CHECK(s == nx::approx(scale));
    }
    if (is_uniform)
    {
        tg::vec3 t;
        tg::mat3 R;
        float s;
        tg::decompose_transformation(M, t, R, s);

        auto const M2 = tg::translation(t) * tg::mat4(R) * tg::scaling<3>(s);
        CHECK(M2 == nx::approx(M));
        CHECK(t == nx::approx(translation));
        CHECK(R == nx::approx(tg::mat3(rotation)));
        CHECK(s == nx::approx(scale.width));
    }

    // direct functions
    CHECK(tg::translation_of(M) == nx::approx(translation));
    CHECK(tg::rotation_mat3_of(M) == nx::approx(tg::mat3(rotation)));
    CHECK(tg::rotation_mat4_of(M) == nx::approx(tg::mat4(rotation)));
    CHECK(tg::rotation_quat_of(M) == cc::any_of(nx::approx(rotation), nx::approx(-rotation)));
    CHECK(tg::scale_of(M) == nx::approx(scale));
    if (is_uniform)
        CHECK(tg::scale_uniform_of(M) == nx::approx(scale.width));
}
