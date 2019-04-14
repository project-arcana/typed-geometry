#pragma once

#include "../../types/pos.hh"
#include "../../types/scalar.hh"
#include "../../types/vec.hh"
#include "../scalar_traits.hh"

namespace tg
{
template <class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
TG_NODISCARD constexpr ScalarT mix(ScalarT v0, ScalarT v1, ScalarT t)
{
    return v0 + t * (v1 - v0);
}

template <int D, class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
TG_NODISCARD constexpr vec<D, ScalarT> mix(vec<D, ScalarT> const& v0, vec<D, ScalarT> const& v1, ScalarT t)
{
    return v0 + t * (v1 - v0);
}
template <int D, class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
TG_NODISCARD constexpr pos<D, ScalarT> mix(pos<D, ScalarT> const& v0, pos<D, ScalarT> const& v1, ScalarT t)
{
    return v0 + t * (v1 - v0);
}

// TODO: more types

} // namespace tg
