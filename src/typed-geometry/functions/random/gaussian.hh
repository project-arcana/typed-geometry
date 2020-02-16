#pragma once

#include <typed-geometry/detail/tg_traits.hh>

#include "uniform.hh"

/*
 * normal<T>(rng)  - generates a normally distributed value
 *
 */

namespace tg
{
template <class T, class Rng, class = enable_if<is_rng<Rng>>>
[[nodiscard]] constexpr T normal(Rng& rng)
{
    if constexpr (is_scalar<T>) // Marsaglia polar method
    {
        T x, y, s;
        do
        {
            x = uniform(rng, T(-1), T(1));
            y = uniform(rng, T(-1), T(1));
            s = x * x + y * y;
        } while (s > 1 || s == 0);

        return x * sqrt((T(-2) * tg::log(s)) / s);
    }
    else
    {
        // TODO: component version
        static_assert(always_false<T>, "component version not implemented");
    }
}

template <class T, class Rng>
[[nodiscard]] constexpr T gaussian(Rng& rng, T mean, T sigma)
{
    // TODO: scalar, vector, mixed versions

    return tg::normal<T>(rng) * sigma + mean;
}

}
