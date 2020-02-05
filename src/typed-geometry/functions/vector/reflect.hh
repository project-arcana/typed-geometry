#pragma once

#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/vec.hh>

#include "dot.hh"

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr vec<D, ScalarT> reflect(vec<D, ScalarT> const& d, dir<D, ScalarT> const& normal)
{
    return d - (ScalarT(2) * dot(d, normal)) * normal;
}
template <int D, class ScalarT>
[[nodiscard]] constexpr dir<D, ScalarT> reflect(dir<D, ScalarT> const& d, dir<D, ScalarT> const& normal)
{
    return dir<D, ScalarT>(reflect(vec<D, ScalarT>(d), normal));
}

// TODO: blanket implementation for all object/object pairs if second is plane or smth?
}
