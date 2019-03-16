#pragma once

#include "../../types/mat.hh"
#include "../../types/vec.hh"

namespace tg
{
template <int D, class T>
constexpr mat<D + 1, D + 1, T> translation(vec<D, T> const& v)
{
    auto m = mat<D + 1, D + 1, T>::identity;
    m[D] = vec<D + 1, T>(v, T(1));
    return m;
}
template <int D, class T>
constexpr mat<D + 1, D + 1, T> translation(pos<D, T> const& v)
{
    auto m = mat<D + 1, D + 1, T>::identity;
    m[D] = vec<D + 1, T>(vec<D, T>(v), T(1));
    return m;
}
} // namespace tg
