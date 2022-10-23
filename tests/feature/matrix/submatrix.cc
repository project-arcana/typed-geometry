#include <nexus/fuzz_test.hh>
#include <nexus/ext/tg-approx.hh>

#include <typed-geometry/feature/matrix.hh>

#include "random_ctors.hh"

FUZZ_TEST("Submatrix")(tg::rng& rng)
{
    {
        auto mat = test::random_matrix<2>(rng);
        auto copy = tg::submatrix<0, 0, 2, 2>(mat);

        CHECK(copy == nx::approx(mat));

        {
            auto sub = tg::submatrix<0, 0, 1, 2>(mat);
            CHECK(sub[0] == nx::approx(mat[0]));
        }

        {
            auto sub = tg::submatrix<1, 0, 1, 2>(mat);
            CHECK(sub[0] == nx::approx(mat[1]));
        }
    }

    {
        auto mat = test::random_matrix<3>(rng);
        auto copy = tg::submatrix<0, 0, 3, 3>(mat);

        CHECK(copy == nx::approx(mat));

        {
            auto sub = tg::submatrix<0, 0, 1, 3>(mat);
            CHECK(sub[0] == nx::approx(mat[0]));
        }

        {
            auto sub = tg::submatrix<1, 0, 1, 3>(mat);
            CHECK(sub[0] == nx::approx(mat[1]));
        }

        {
            auto sub = tg::submatrix<2, 0, 1, 3>(mat);
            CHECK(sub[0] == nx::approx(mat[2]));
        }
    }
}
