#pragma once

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/triangle.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/functions/basic/mix.hh>

/*
 * Interpolation between various objects or inside an object
 *
 * Is usually only well defined if weights add up to 1. Undefined behavior otherwise.
 * Some functions are given with one parameter less and infer the last one
 *
 * Basically a more potent version of tg::mix
 */

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr vec<D, ScalarT> interpolate(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b, ScalarT t)
{
    return mix(a, b, t);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr vec<D, ScalarT> interpolate(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b, ScalarT wa, ScalarT wb)
{
    return a * wa + b * wb;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> interpolate(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b, ScalarT t)
{
    return mix(a, b, t);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> interpolate(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b, ScalarT wa, ScalarT wb)
{
    auto z = pos<D, ScalarT>::zero;
    return z + (a - z) * wa + (b - z) * wb;
}

template <class ScalarT, class = enable_if<is_scalar<ScalarT>>>
[[nodiscard]] constexpr ScalarT interpolate(ScalarT a, ScalarT b, ScalarT t)
{
    return mix(a, b, t);
}

template <class ScalarT, class = enable_if<is_scalar<ScalarT>>>
[[nodiscard]] constexpr ScalarT interpolate(ScalarT a, ScalarT b, ScalarT wa, ScalarT wb)
{
    return a * wa + b * wb;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> interpolate(triangle<D, ScalarT> const& t, ScalarT wa, ScalarT wb)
{
    auto z = pos<D, ScalarT>::zero;
    return z + (t.pos0 - z) * wa + (t.pos1 - z) * wb + (t.pos2 - z) * (ScalarT(1) - wb - wa);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> interpolate(triangle<D, ScalarT> const& t, ScalarT wa, ScalarT wb, ScalarT wc)
{
    auto z = pos<D, ScalarT>::zero;
    return z + (t.pos0 - z) * wa + (t.pos1 - z) * wb + (t.pos2 - z) * wc;
}
} // namespace tg
