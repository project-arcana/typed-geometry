#pragma once

#include "../../types/objects/box.hh"
#include "../../types/objects/triangle.hh"
#include "../../types/pos.hh"
#include "../../types/vec.hh"

#include "mix.hh"

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
TG_NODISCARD constexpr vec<D, ScalarT> interpolate(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b, ScalarT t)
{
    return mix(a, b, t);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> interpolate(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b, ScalarT t)
{
    return mix(a, b, t);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> interpolate(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b, ScalarT wa, ScalarT wb)
{
    auto z = pos<D, ScalarT>::zero;
    return z + (a - z) * wa + (b - z) * wb;
}
template <class ScalarT, enable_if<is_scalar<ScalarT>>>
TG_NODISCARD constexpr ScalarT interpolate(ScalarT a, ScalarT b, ScalarT t)
{
    return mix(a, b, t);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> interpolate(triangle<D, ScalarT> const& t, ScalarT wa, ScalarT wb)
{
    auto z = pos<D, ScalarT>::zero;
    return z + (t.v0 - z) * wa + (t.v1 - z) * wb + (t.v2 - z) * (ScalarT(1) - wb - wa);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> interpolate(triangle<D, ScalarT> const& t, ScalarT wa, ScalarT wb, ScalarT wc)
{
    auto z = pos<D, ScalarT>::zero;
    return z + (t.v0 - z) * wa + (t.v1 - z) * wb + (t.v2 - z) * wc;
}
} // namespace tg
