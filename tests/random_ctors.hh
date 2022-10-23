#pragma once

#include <typed-geometry/feature/matrix.hh>
#include <typed-geometry/feature/random.hh>

namespace test
{
template <int D, class T = float, class Rng>
tg::vec<D, T> random_vector(Rng& rng, T range = 5.f)
{
    tg::vec<D, T> v;
    for (auto x = 0; x < D; ++x)
        v[x] = uniform(rng, -range, range);
    return v;
}
template <int D, class T = float, class Rng>
tg::mat<D, D, T> random_matrix(Rng& rng, T range = 2.f)
{
    tg::mat<D, D, T> m;
    for (auto x = 0; x < D; ++x)
        for (auto y = 0; y < D; ++y)
            m[x][y] = uniform(rng, -range, range);
    return m;
}
template <int D, class T = float, class Rng>
tg::mat<D, D, T> random_invertible_matrix(Rng& rng, T range = 2.f)
{
    while (true)
    {
        auto m = random_matrix<D, T>(rng, range);
        if (tg::abs(determinant(m)) > T(0.1))
            return m;
    }
}
}
