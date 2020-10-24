#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <class T>
[[nodiscard]] constexpr T norm(vec<1, T> const& v, T p)
{
    static_assert(is_floating_point<T>, "norm only works on f32/f64");
    return tg::pow(abs(tg::pow(v.x, p)), T(1) / p);
}
template <class T>
[[nodiscard]] constexpr T norm(vec<2, T> const& v, T p)
{
    static_assert(is_floating_point<T>, "norm only works on f32/f64");
    return tg::pow(abs(tg::pow(v.x, p)) + abs(tg::pow(v.y, p)), T(1) / p);
}
template <class T>
[[nodiscard]] constexpr T norm(vec<3, T> const& v, T p)
{
    static_assert(is_floating_point<T>, "norm only works on f32/f64");
    return tg::pow(abs(tg::pow(v.x, p)) + abs(tg::pow(v.y, p)) + abs(tg::pow(v.z, p)), T(1) / p);
}
template <class T>
[[nodiscard]] constexpr T norm(vec<4, T> const& v, T p)
{
    static_assert(is_floating_point<T>, "norm only works on f32/f64");
    return tg::pow(abs(tg::pow(v.x, p)) + abs(tg::pow(v.y, p)) + abs(tg::pow(v.z, p)) + abs(tg::pow(v.w, p)), T(1) / p);
}

template <int C, int R, class T>
[[nodiscard]] constexpr T frobenius_norm_sqr(mat<C, R, T> const& v)
{
    static_assert(is_floating_point<T>, "frobenius_norm_sqr only works on f32/f64");
    auto s = T(0);
    for (auto x = 0; x < C; ++x)
        for (auto y = 0; y < R; ++y)
            s += pow2(v[x][y]);
    return s;
}

template <int C, int R, class T>
[[nodiscard]] constexpr T frobenius_norm(mat<C, R, T> const& v)
{
    static_assert(is_floating_point<T>, "frobenius_norm only works on f32/f64");
    return sqrt(frobenius_norm_sqr(v));
}

}
