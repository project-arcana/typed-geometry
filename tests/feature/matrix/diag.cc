#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/matrix.hh>
#include <typed-geometry/feature/vector.hh>

template <int D, class ScalarT>
static void checkAllEqual(const tg::vec<D, ScalarT>& v, ScalarT c)
{
    /// checks whether all elements of vector equal to c
    for (auto i = 0; i < D; i++)
        CHECK(v[i] == c);
}

template <int M, int N, class ScalarT>
static tg::mat<M, N, ScalarT> randMat(tg::rng& rng)
{
    auto b = 5.0f;
    auto dBox = tg::aabb<N, ScalarT>(-tg::pos<N, ScalarT>(b), tg::pos<N, ScalarT>(b));

    auto mat = tg::mat<M, N, ScalarT>();
    for (auto c = 0; c < M; c++)
        mat[c] = tg::vec<N, ScalarT>(uniform(rng, dBox));
    return mat;
}

TEST("Diag")
{
    // m x n identity and zero matrices
    // square
    {
        auto mat = tg::mat1::identity;
        auto d = diag(mat);
        checkAllEqual(d, 1.0f);

        mat = tg::mat1::zero;
        d = diag(mat);
        checkAllEqual(d, 0.0f);
    }
    {
        auto mat = tg::mat2::identity;
        auto d = diag(mat);
        checkAllEqual(d, 1.0f);

        mat = tg::mat2::zero;
        d = diag(mat);
        checkAllEqual(d, 0.0f);
    }
    {
        auto mat = tg::mat3::identity;
        auto d = diag(mat);
        checkAllEqual(d, 1.0f);

        mat = tg::mat3::zero;
        d = diag(mat);
        checkAllEqual(d, 0.0f);
    }
    {
        auto mat = tg::mat4::identity;
        auto d = diag(mat);
        checkAllEqual(d, 1.0f);

        mat = tg::mat4::zero;
        d = diag(mat);
        checkAllEqual(d, 0.0f);
    }
    // non-square
    {
        auto mat = tg::mat2x3::identity;
        auto d = diag(mat);
        checkAllEqual(d, 1.0f);

        mat = tg::mat2x3::zero;
        d = diag(mat);
        checkAllEqual(d, 0.0f);
    }
    {
        auto mat = tg::mat2x4::identity;
        auto d = diag(mat);
        checkAllEqual(d, 1.0f);

        mat = tg::mat2x4::zero;
        d = diag(mat);
        checkAllEqual(d, 0.0f);
    }
    {
        auto mat = tg::mat3x2::identity;
        auto d = diag(mat);
        checkAllEqual(d, 1.0f);

        mat = tg::mat3x2::zero;
        d = diag(mat);
        checkAllEqual(d, 0.0f);
    }
    {
        auto mat = tg::mat3x4::identity;
        auto d = diag(mat);
        checkAllEqual(d, 1.0f);

        mat = tg::mat3x4::zero;
        d = diag(mat);
        checkAllEqual(d, 0.0f);
    }
    {
        auto mat = tg::mat4x2::identity;
        auto d = diag(mat);
        checkAllEqual(d, 1.0f);

        mat = tg::mat4x2::zero;
        d = diag(mat);
        checkAllEqual(d, 0.0f);
    }
    {
        auto mat = tg::mat4x3::identity;
        auto d = diag(mat);
        checkAllEqual(d, 1.0f);

        mat = tg::mat4x3::zero;
        d = diag(mat);
        checkAllEqual(d, 0.0f);
    }
}

FUZZ_TEST("Diag")(tg::rng& rng)
{
    auto randBox = tg::aabb1(-10.0f, 10.0f);
    // random square matrices
    {
        auto mat = randMat<1, 1, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<2, 2, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);
        CHECK(d.y == mat[1][1]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        mat[1][1] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<3, 3, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);
        CHECK(d.y == mat[1][1]);
        CHECK(d.z == mat[2][2]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        mat[1][1] = r;
        mat[2][2] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<4, 4, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);
        CHECK(d.y == mat[1][1]);
        CHECK(d.z == mat[2][2]);
        CHECK(d.w == mat[3][3]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        mat[1][1] = r;
        mat[2][2] = r;
        mat[3][3] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    // non-square
    {
        auto mat = randMat<3, 4, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);
        CHECK(d.y == mat[1][1]);
        CHECK(d.z == mat[2][2]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        mat[1][1] = r;
        mat[2][2] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<4, 3, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);
        CHECK(d.y == mat[1][1]);
        CHECK(d.z == mat[2][2]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        mat[1][1] = r;
        mat[2][2] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<4, 2, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);
        CHECK(d.y == mat[1][1]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        mat[1][1] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<2, 4, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);
        CHECK(d.y == mat[1][1]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        mat[1][1] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<4, 1, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<1, 4, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<1, 3, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<1, 2, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<3, 2, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);
        CHECK(d.y == mat[1][1]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        mat[1][1] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<2, 3, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);
        CHECK(d.y == mat[1][1]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        mat[1][1] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<3, 1, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
    {
        auto mat = randMat<2, 1, tg::f32>(rng);
        auto d = diag(mat);
        CHECK(d.x == mat[0][0]);

        // set to random value
        auto r = uniform(rng, randBox).x;
        mat[0][0] = r;
        d = diag(mat);
        checkAllEqual(d, r);
    }
}
