#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>
#include <typed-geometry/feature/vector.hh>

FUZZ_TEST("Volume")(tg::rng& rng)
{
    {
        const auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
        const auto r = uniform(rng, 0.0f, 10.0f);
        const auto r2 = uniform(rng, 0.0f, 10.0f);
        const auto r3 = uniform(rng, 0.0f, 10.0f);
        const auto n = tg::uniform<tg::dir3>(rng);
        const auto pos30 = uniform(rng, range3);
        const auto pos31 = uniform(rng, range3);
        const auto d30 = tg::uniform<tg::dir3>(rng);
        const auto d31 = any_normal(d30);
        const auto d32 = normalize(cross(d30, d31));

        const auto ball = tg::sphere3(pos30, r);

        const auto vb = volume_of(ball);
        CHECK(vb == nx::approx(4.0f / 3.0f * tg::pi_scalar<tg::f32> * tg::pow3(r)));

        const auto cylinder = tg::cylinder3(pos30, pos30 + n * r * 2, r);
        const auto vc = volume_of(cylinder);
        CHECK(vc == nx::approx(3.0f / 2.0f * vb));

        // position and orientation is irrelevant
        const auto cone = tg::cone3({pos30, r, n}, r);

        // volume of ball is equal to volume of cylinder - 2 * volume of cone
        CHECK(vb == nx::approx(vc - 2 * volume_of(cone)));

        const auto ellipsoid = tg::ellipse3(pos30, tg::mat3::from_cols(r * d30, r2 * d31, r3 * d32));
        const auto ve = volume_of(ellipsoid);
        // volume of ellipsoid is scaled compared to the sphere
        CHECK(ve == nx::approx(vb * r2 * r3 / (r * r)));

        const auto hemisphere = tg::hemisphere3(pos30, r, n);
        const auto vh = volume_of(hemisphere);
        // volume of ball is two times volume of hemisphere
        CHECK(vb == nx::approx(2.f * vh));

        const auto capsule = tg::capsule3(pos30, pos31, r);
        const auto vCapsule = volume_of(capsule);
        const auto vCyl = volume_of(tg::cylinder3(pos30, pos31, r));
        // volume of capsule is volume of cylinder + two caps
        CHECK(vCapsule == nx::approx(vCyl + 2.f * vh));
    }
}
