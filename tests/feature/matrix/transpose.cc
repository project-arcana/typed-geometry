#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/matrix.hh>

#include "random_ctors.hh"

FUZZ_TEST("Transpose")(tg::rng& rng)
{
    // orthogonal matrix: tranposed is inverse
    {
        auto box1 = tg::aabb1(tg::pos1(-1.0f), tg::pos1(1.0f));
        auto r = uniform(rng, box1).x;

        // 2D
        {
            auto mat = tg::rotation_around(tg::pos2::zero, tg::angle::from_degree(r));
            CHECK(inverse(mat) == nx::approx(transpose(mat)));
        }
        // 3D
        {
            auto mat = tg::rotation_around(tg::dir3::pos_y, tg::angle::from_degree(r));
            CHECK(inverse(mat) == nx::approx(transpose(mat)));
        }
    }

    // transposing the transposed
    {
        // 2D
        {
            auto mat = test::random_matrix<2>(rng);
            CHECK(mat == nx::approx(transpose(transpose(mat))));
        }

        // 3D
        {
            auto mat = test::random_matrix<3>(rng);
            CHECK(mat == nx::approx(transpose(transpose(mat))));
        }

        // 4D
        {
            auto mat = test::random_matrix<4>(rng);
            CHECK(mat == nx::approx(transpose(transpose(mat))));
        }
    }
}
