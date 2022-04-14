#include <nexus/test.hh>

TG_FUZZ_TEST(TypedGeometry, Determinant)
{
    auto rd1 = tg::aabb1(0.0f, 1.0f);

    // matrices with obvious determinants
    {
        // 1D
        const auto D = 1;
        auto mat = tg::mat<D, D, tg::f32>::zero;
        CHECK(determinant(mat) == 0.0f);

        mat = tg::mat<D, D, tg::f32>::identity;
        CHECK(determinant(mat) == approx(1.0f));

        auto rf = uniform(rng, rd1).x;
        mat = tg::mat<D, D, tg::f32>::diag(rf);
        auto res = tg::pow(rf, D);
        CHECK(determinant(mat) == approx(res));
    }

    {
        // 2D
        const auto D = 2;
        auto mat = tg::mat<D, D, tg::f32>::zero;
        CHECK(determinant(mat) == 0.0f);

        mat = tg::mat<D, D, tg::f32>::identity;
        CHECK(determinant(mat) == approx(1.0f));

        auto rf = uniform(rng, rd1).x;
        mat = tg::mat<D, D, tg::f32>::diag(rf);
        auto res = tg::pow(rf, D);
        CHECK(determinant(mat) == approx(res));
    }

    {
        // 3D
        const auto D = 3;
        auto mat = tg::mat<D, D, tg::f32>::zero;
        CHECK(determinant(mat) == 0.0f);

        mat = tg::mat<D, D, tg::f32>::identity;
        CHECK(determinant(mat) == approx(1.0f));

        auto rf = uniform(rng, rd1).x;
        mat = tg::mat<D, D, tg::f32>::diag(rf);
        auto res = tg::pow(rf, D);
        CHECK(determinant(mat) == approx(res));
    }

    // random DxD matrices for 3D
    {
        auto matA = random_matrix<3>(rng);
        auto matB = random_matrix<3>(rng);
        // confirm multiplicativity
        CHECK(determinant(matA * matB) == approx(determinant(matA) * determinant(matB)).epsilon(0.01));

        matA = random_invertible_matrix<3>(rng);
        matB = random_invertible_matrix<3>(rng);

        // if invertible det != 0
        CHECK(determinant(matA) != approx(0.0f));
        // CHECK(determinant(matB) != approx(0.0f));

        // confirm multiplicativity
        auto detA = determinant(matA);
        auto detB = determinant(matB);
        CHECK(determinant(matA * matB) == approx(detA * detB));

        auto invA = inverse(matA);
        auto invB = inverse(matB);

        CHECK(determinant(invA) == approx(1.0f / detA).epsilon(0.01));
        CHECK(determinant(invB) == approx(1.0f / detB).epsilon(0.01));
    }

    // random DxD matrices for 2D
    {
        auto matA = random_matrix<2>(rng);
        auto matB = random_matrix<2>(rng);
        // confirm multiplicativity
        CHECK(determinant(matA * matB) == approx(determinant(matA) * determinant(matB)).epsilon(0.01));

        matA = random_invertible_matrix<2>(rng);
        matB = random_invertible_matrix<2>(rng);

        // if invertible det != 0
        CHECK(determinant(matA) != approx(0.0f));
        CHECK(determinant(matB) != approx(0.0f));

        // confirm multiplicativity
        auto detA = determinant(matA);
        auto detB = determinant(matB);
        CHECK(determinant(matA * matB) == approx(detA * detB));

        auto invA = inverse(matA);
        auto invB = inverse(matB);

        CHECK(determinant(invA) == approx(1.0f / detA));
        CHECK(determinant(invB) == approx(1.0f / detB));
    }
}
