#pragma once

#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <int D, class T>
[[nodiscard]] constexpr mat<D + 1, D + 1, T> scaling(size<D, T> const& v)
{
    return mat<D + 1, D + 1, T>::diag(vec<D + 1, T>(vec<D, T>(v), T(1)));
}
template <int D, class T>
[[nodiscard]] constexpr mat<D + 1, D + 1, T> scaling(T v)
{
    return mat<D + 1, D + 1, T>::diag(vec<D + 1, T>(vec<D, T>(v), T(1)));
}

template <class T>
[[nodiscard]] constexpr mat<3, 3, T> scaling(T sx, T sy)
{
    return mat<3, 3, T>::diag(vec<3, T>(sx, sy, T(1)));
}
template <class T>
[[nodiscard]] constexpr mat<4, 4, T> scaling(T sx, T sy, T sz)
{
    return mat<4, 4, T>::diag(vec<4, T>(sx, sy, sz, T(1)));
}
} // namespace tg
