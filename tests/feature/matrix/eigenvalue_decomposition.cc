// test requires eigen. Enable TG_EIGEN_TESTS to run this
#ifdef TG_EIGEN_TESTS

#include <Eigen/Eigenvalues>
#include <typed-geometry/functions/matrix/eigenvalues.hh>

#include <nexus/fuzz_test.hh>

template <int M, int N, class ScalarT>
static tg::mat<M, N, ScalarT> rand_symmetric_Mat(tg::rng& rng)
{
    auto b = ScalarT(5.0);
    auto mat = tg::mat<M, N, ScalarT>();
    for (auto c = 0; c < M; c++)
        for (auto r = c; r < N; r++)
        {
            mat[c][r] = tg::uniform(rng, -b, b);
            mat[r][c] = mat[c][r];
        }
    return mat;
}

FUZZ_TEST("EigenDecompositionSymmetric")(tg::rng& rng)
{
    auto const generate_matrices = [&]() -> tg::pair<tg::dmat3, Eigen::Matrix3d> {
        tg::dmat3 tgm = rand_symmetric_Mat<3, 3, double>(rng);
        // eigen is row major
        Eigen::Matrix3d em;
        for (auto r : tg::range(3))
            for (auto c : tg::range(3))
                em(r, c) = tgm[c][r];
        return {tgm, em};
    };

    auto [tgm, em] = generate_matrices();

    Eigen::EigenSolver<Eigen::Matrix3d> solver;
    solver.compute(em, true);
    auto const eevals = solver.eigenvalues();
    auto decomp = tg::eigen_decomposition_symmetric(tgm);

    tg::array<double, 3> eevala;
    eevala[0] = eevals[0].real();
    eevala[1] = eevals[1].real();
    eevala[2] = eevals[2].real();

    // sort eigenvalues by size
    if (decomp[0].eigenvalue > decomp[1].eigenvalue)
        std::swap(decomp[0], decomp[1]);
    if (decomp[0].eigenvalue > decomp[2].eigenvalue)
        std::swap(decomp[2], decomp[2]);
    if (decomp[1].eigenvalue > decomp[2].eigenvalue)
        std::swap(decomp[1], decomp[2]);

    if (eevala[0] > eevala[1])
        std::swap(eevala[0], eevala[1]);
    if (eevala[0] > eevala[2])
        std::swap(eevala[0], eevala[2]);
    if (eevala[1] > eevala[2])
        std::swap(eevala[1], eevala[2]);

    for (auto i : tg::range(3))
    {
        // check same eigenvalue
        CHECK(eevala[i] == nx::approx(decomp[i].eigenvalue));
        // check if eigendecomposition
        auto const lhs = tgm * decomp[i].eigenvector;
        auto const rhs = decomp[i].eigenvalue * decomp[i].eigenvector;
        CHECK(lhs[0] == nx::approx(rhs[0]));
        CHECK(lhs[1] == nx::approx(rhs[1]));
        CHECK(lhs[2] == nx::approx(rhs[2]));
    }
}


#endif
