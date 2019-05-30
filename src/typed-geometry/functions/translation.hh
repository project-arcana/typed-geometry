#pragma once

#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <int D, class T>
[[nodiscard]] constexpr mat<D + 1, D + 1, T> translation(vec<D, T> const& v)
{
    auto m = mat<D + 1, D + 1, T>::identity;
    m[D] = vec<D + 1, T>(v, T(1));
    return m;
}

// Note: revisit once we have use cases
// template <int D, class T>
// constexpr mat<D + 1, D + 1, T> translation(pos<D, T> const& v)
// {
//     auto m = mat<D + 1, D + 1, T>::identity;
//     m[D] = vec<D + 1, T>(vec<D, T>(v), T(1));
//     return m;
// }
} // namespace tg
