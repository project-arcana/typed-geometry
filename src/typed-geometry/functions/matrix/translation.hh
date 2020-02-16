#pragma once

#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/detail/scalar_traits.hh>

namespace tg
{
template <int D, class T>
[[nodiscard]] constexpr mat<D + 1, D + 1, T> translation(vec<D, T> const& v)
{
    auto m = mat<D + 1, D + 1, T>::identity;
    m[D] = vec<D + 1, T>(v, T(1));
    return m;
}

template <int D, class T>
[[nodiscard]] constexpr mat<D + 1, D + 1, T> translation(pos<D, T> const& v)
{
    auto m = mat<D + 1, D + 1, T>::identity;
    m[D] = vec<D + 1, T>(vec<D, T>(v), T(1));
    return m;
}

template <class T, class = enable_if<is_scalar<T>>>
[[nodiscard]] constexpr mat<3, 3, T> translation(T x, T y)
{
    auto m = mat<3, 3, T>::identity;
    m[2] = vec<3, T>(x, y, T(1));
    return m;
}

template <class T, class = enable_if<is_scalar<T>>>
[[nodiscard]] constexpr mat<4, 4, T> translation(T x, T y, T z)
{
    auto m = mat<4, 4, T>::identity;
    m[3] = vec<4, T>(x, y, z, T(1));
    return m;
}
} // namespace tg
