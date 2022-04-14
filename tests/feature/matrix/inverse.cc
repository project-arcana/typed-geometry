#include <nexus/test.hh>

TG_FUZZ_TEST(TypedGeometry, Inverse)
{
    // random mat1 inverse test
    {
        auto box1 = tg::aabb1(tg::pos1(-1), tg::pos1(1));

        auto m = tg::mat1();
        auto r = 0.0f;
        while (r == 0)
            r = uniform(rng, box1).x * 10.0f;
        m[0] = r;

        CHECK(tg::inverse(m) * m == approx(tg::mat1::identity, 0.01f));
    }

    // random mat2 inverse test
    {
        auto m = random_invertible_matrix<2>(rng);

        CHECK(tg::inverse(m) * m == approx(tg::mat2::identity, 0.01f));
    }

    // random mat3 inverse test
    {
        auto m = random_invertible_matrix<3>(rng);

        CHECK(tg::inverse(m) * m == approx(tg::mat3::identity, 0.01f));
    }

    // random mat4 inverse test
    {
        auto m = random_invertible_matrix<4>(rng);

        CHECK(tg::inverse(m) * m == approx(tg::mat4::identity, 0.01f));
    }
}
