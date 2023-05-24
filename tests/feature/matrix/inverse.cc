#include <nexus/fuzz_test.hh>
#include <nexus/ext/tg-approx.hh>

#include <typed-geometry/feature/matrix.hh>

#include "random_ctors.hh"

FUZZ_TEST("Inverse")(tg::rng& rng)
{
    // random mat1 inverse test
    {
        auto box1 = tg::aabb1(tg::pos1(-1), tg::pos1(1));

        auto m = tg::mat1();
        auto r = 0.0f;
        while (r == 0)
            r = uniform(rng, box1).x * 10.0f;
        m[0] = r;

        CHECK(tg::inverse(m) * m == nx::approx(tg::mat1::identity));
    }

    // random mat2 inverse test
    {
        auto m = test::random_invertible_matrix<2>(rng);

        CHECK(tg::inverse(m) * m == nx::approx(tg::mat2::identity));
    }

    // random mat3 inverse test
    {
        auto m = test::random_invertible_matrix<3>(rng);

        CHECK(tg::inverse(m) * m == nx::approx(tg::mat3::identity));
    }

    // random mat4 inverse test
    {
        auto m = test::random_invertible_matrix<4>(rng);

        CHECK(tg::inverse(m) * m == nx::approx(tg::mat4::identity));
    }
}
