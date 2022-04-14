#include <nexus/fuzz_test.hh>

FUZZ_TEST("Rotation")(tg::rng& rng)
{
    auto a = tg::degree(uniform(rng, 0.0f, 360.0f));

    CHECK(frobenius_norm(rotation_x(a) - rotation_around(tg::dir3::pos_x, a)) == approx(0.0f));
    CHECK(frobenius_norm(rotation_y(a) - rotation_around(tg::dir3::pos_y, a)) == approx(0.0f));
    CHECK(frobenius_norm(rotation_z(a) - rotation_around(tg::dir3::pos_z, a)) == approx(0.0f));

    auto v = tg::uniform<tg::dir3>(rng);
    auto R1 = rotation_around(v, a);
    auto R2 = rotation_around(-v, -a);
    CHECK(frobenius_norm(R1 - R2) == approx(0.0f));

    CHECK(is_orthonormal(R1));
    CHECK(is_orthonormal(R2));
}
