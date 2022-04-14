#include <nexus/test.hh>

TG_FUZZ_TEST(TypedGeometry, Transpose)
{
    // orthogonal matrix: tranposed is inverse
    {
        auto box1 = tg::aabb1(tg::pos1(-1.0f), tg::pos1(1.0f));
        auto r = uniform(rng, box1).x;

        // 2D
        {
            auto mat = tg::rotation_around(tg::pos2::zero, tg::angle::from_degree(r));
            CHECK(inverse(mat) == approx(transpose(mat), 1e-5f));
        }
        // 3D
        {
            auto mat = tg::rotation_around(tg::dir3::pos_y, tg::angle::from_degree(r));
            CHECK(inverse(mat) == approx(transpose(mat), 1e-5f));
        }
    }

    // transposing the transposed
    {
        // 2D
        {
            auto mat = random_matrix<2>(rng);
            CHECK(mat == approx(transpose(transpose(mat))));
        }

        // 3D
        {
            auto mat = random_matrix<3>(rng);
            CHECK(mat == approx(transpose(transpose(mat))));
        }

        // 4D
        {
            auto mat = random_matrix<4>(rng);
            CHECK(mat == approx(transpose(transpose(mat))));
        }
    }
}
