#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/matrix.hh>

#include "random_ctors.hh"

FUZZ_TEST("Determinant")(tg::rng& rng)
{
    auto rd1 = tg::aabb1(0.0f, 1.0f);

    // matrices with obvious determinants
    {
        // 1D
        const auto D = 1;
        auto mat = tg::mat<D, D, tg::f32>::zero;
        CHECK(determinant(mat) == 0.0f);

        mat = tg::mat<D, D, tg::f32>::identity;
        CHECK(determinant(mat) == nx::approx(1.0f));

        auto rf = uniform(rng, rd1).x;
        mat = tg::mat<D, D, tg::f32>::diag(rf);
        auto res = tg::pow(rf, D);
        CHECK(determinant(mat) == nx::approx(res));
    }

    {
        // 2D
        const auto D = 2;
        auto mat = tg::mat<D, D, tg::f32>::zero;
        CHECK(determinant(mat) == 0.0f);

        mat = tg::mat<D, D, tg::f32>::identity;
        CHECK(determinant(mat) == nx::approx(1.0f));

        auto rf = uniform(rng, rd1).x;
        mat = tg::mat<D, D, tg::f32>::diag(rf);
        auto res = tg::pow(rf, D);
        CHECK(determinant(mat) == nx::approx(res));
    }

    {
        // 3D
        const auto D = 3;
        auto mat = tg::mat<D, D, tg::f32>::zero;
        CHECK(determinant(mat) == 0.0f);

        mat = tg::mat<D, D, tg::f32>::identity;
        CHECK(determinant(mat) == nx::approx(1.0f));

        auto rf = uniform(rng, rd1).x;
        mat = tg::mat<D, D, tg::f32>::diag(rf);
        auto res = tg::pow(rf, D);
        CHECK(determinant(mat) == nx::approx(res));
    }

    // random DxD matrices for 3D
    {
        auto matA = test::random_matrix<3>(rng);
        auto matB = test::random_matrix<3>(rng);
        // confirm multiplicativity
        CHECK(determinant(matA * matB) == nx::approx(determinant(matA) * determinant(matB)));

        matA = test::random_invertible_matrix<3>(rng);
        matB = test::random_invertible_matrix<3>(rng);

        // if invertible det != 0
        CHECK(determinant(matA) != nx::approx(0.0f).abs(0.001f));
        // CHECK(determinant(matB) != approx(0.0f));

        // confirm multiplicativity
        auto detA = determinant(matA);
        auto detB = determinant(matB);
        CHECK(determinant(matA * matB) == nx::approx(detA * detB).abs(0.01f));

        auto invA = inverse(matA);
        auto invB = inverse(matB);

        CHECK(determinant(invA) == nx::approx(1.0f / detA).abs(0.01f));
        CHECK(determinant(invB) == nx::approx(1.0f / detB).abs(0.01f));
    }

    // random DxD matrices for 2D
    {
        auto matA = test::random_matrix<2>(rng);
        auto matB = test::random_matrix<2>(rng);
        // confirm multiplicativity
        CHECK(determinant(matA * matB) == nx::approx(determinant(matA) * determinant(matB)));

        matA = test::random_invertible_matrix<2>(rng);
        matB = test::random_invertible_matrix<2>(rng);

        // if invertible det != 0
        CHECK(determinant(matA) != nx::approx(0.0f).abs(0.001f));
        CHECK(determinant(matB) != nx::approx(0.0f).abs(0.001f));

        // confirm multiplicativity
        auto detA = determinant(matA);
        auto detB = determinant(matB);
        CHECK(determinant(matA * matB) == nx::approx(detA * detB));

        auto invA = inverse(matA);
        auto invB = inverse(matB);

        CHECK(determinant(invA) == nx::approx(1.0f / detA));
        CHECK(determinant(invB) == nx::approx(1.0f / detB));
    }
}
