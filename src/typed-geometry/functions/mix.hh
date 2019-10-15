#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/angle.hh>
#include <typed-geometry/types/color.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/scalars/scalars.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr ScalarT mix(ScalarT v0, ScalarT v1, ScalarT t)
{
    return v0 + t * (v1 - v0);
}
template <class ScalarT>
TG_NODISCARD constexpr angle_t<ScalarT> mix(angle_t<ScalarT> v0, angle_t<ScalarT> v1, tg::dont_deduce<ScalarT> t)
{
    return tg::radians(tg::mix(v0.radians(), v1.radians(), t));
}

template <int D, class ScalarT>
TG_NODISCARD constexpr comp<D, ScalarT> mix(comp<D, ScalarT> const& v0, comp<D, ScalarT> const& v1, tg::dont_deduce<ScalarT> t)
{
    return v0 + t * (v1 - v0);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr size<D, ScalarT> mix(size<D, ScalarT> const& v0, size<D, ScalarT> const& v1, tg::dont_deduce<ScalarT> t)
{
    return v0 + t * (v1 - v0);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> mix(vec<D, ScalarT> const& v0, vec<D, ScalarT> const& v1, tg::dont_deduce<ScalarT> t)
{
    return v0 + t * (v1 - v0);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> mix(pos<D, ScalarT> const& v0, pos<D, ScalarT> const& v1, tg::dont_deduce<ScalarT> t)
{
    return v0 + t * (v1 - v0);
}

template <class ScalarT>
TG_NODISCARD constexpr color<3, ScalarT> mix(color<3, ScalarT> const& c0, color<3, ScalarT> const& c1, tg::dont_deduce<ScalarT> t)
{
    return {
        mix(c0.r, c1.r, t), //
        mix(c0.g, c1.g, t), //
        mix(c0.b, c1.b, t)  //
    };
}
template <class ScalarT>
TG_NODISCARD constexpr color<4, ScalarT> mix(color<4, ScalarT> const& c0, color<4, ScalarT> const& c1, tg::dont_deduce<ScalarT> t)
{
    return {
        mix(c0.r, c1.r, t), //
        mix(c0.g, c1.g, t), //
        mix(c0.b, c1.b, t), //
        mix(c0.a, c1.a, t)  //
    };
}

// default implementation of lerp is same as mix
template <template <int, class> class comp_t, int D, class ScalarT>
TG_NODISCARD constexpr auto lerp(comp_t<D, ScalarT> const& v0, comp_t<D, ScalarT> const& v1, tg::dont_deduce<ScalarT> t) -> decltype(mix(v0, v1, t))
{
    return mix(v0, v1, t);
}

template <class ScalarT>
TG_NODISCARD constexpr auto lerp(ScalarT v0, ScalarT v1, dont_deduce<ScalarT> t) -> decltype(mix(v0, v1, t))
{
    return mix(v0, v1, t);
}

// TODO: more types

} // namespace tg
