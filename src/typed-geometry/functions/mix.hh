#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/angle.hh>
#include <typed-geometry/types/color.hh>
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
TG_NODISCARD constexpr angle_t<ScalarT> mix(angle_t<ScalarT> v0, angle_t<ScalarT> v1, ScalarT t)
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

template <class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
TG_NODISCARD constexpr color<3, ScalarT> mix(color<3, ScalarT> const& c0, color<3, ScalarT> const& c1, ScalarT t)
{
    return {
        mix(c0.r, c1.r, t), //
        mix(c0.g, c1.g, t), //
        mix(c0.b, c1.b, t)  //
    };
}
template <class ScalarT, class = enable_if<is_floating_point<ScalarT>>>
TG_NODISCARD constexpr color<4, ScalarT> mix(color<4, ScalarT> const& c0, color<4, ScalarT> const& c1, ScalarT t)
{
    return {
        mix(c0.r, c1.r, t), //
        mix(c0.g, c1.g, t), //
        mix(c0.b, c1.b, t), //
        mix(c0.a, c1.a, t)  //
    };
}

// TODO: more types

} // namespace tg
