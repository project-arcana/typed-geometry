#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/angle.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/scalar.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
TG_NODISCARD constexpr ScalarT mix(ScalarT v0, ScalarT v1, ScalarT t)
{
    return v0 + t * (v1 - v0);
}
template <class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
TG_NODISCARD constexpr angle<ScalarT> mix(angle<ScalarT> v0, angle<ScalarT> v1, ScalarT t)
{
    return tg::radians(tg::mix(v0.radians(), v1.radians(), t));
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
