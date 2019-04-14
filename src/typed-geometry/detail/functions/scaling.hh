#pragma once

#include "../../types/mat.hh"
#include "../../types/vec.hh"

namespace tg
{
template <int D, class T>
TG_NODISCARD constexpr mat<D + 1, D + 1, T> scaling(size<D, T> const& v)
{
    return mat<D + 1, D + 1, T>::diag(vec<D + 1, T>(vec<D, T>(v), T(1)));
}
} // namespace tg
