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
[[nodiscard]] constexpr ScalarT mix(ScalarT v0, ScalarT v1, tg::dont_deduce<ScalarT> t)
{
    return v0 + t * (v1 - v0);
}
template <class ScalarT>
[[nodiscard]] constexpr angle_t<ScalarT> mix(angle_t<ScalarT> v0, angle_t<ScalarT> v1, tg::dont_deduce<ScalarT> t)
{
    return tg::radians(tg::mix(v0.radians(), v1.radians(), t));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr comp<D, ScalarT> mix(comp<D, ScalarT> const& v0, comp<D, ScalarT> const& v1, tg::dont_deduce<ScalarT> t)
{
    return v0 + t * (v1 - v0);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr size<D, ScalarT> mix(size<D, ScalarT> const& v0, size<D, ScalarT> const& v1, tg::dont_deduce<ScalarT> t)
{
    return v0 + t * (v1 - v0);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr vec<D, ScalarT> mix(vec<D, ScalarT> const& v0, vec<D, ScalarT> const& v1, tg::dont_deduce<ScalarT> t)
{
    return v0 + t * (v1 - v0);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> mix(pos<D, ScalarT> const& v0, pos<D, ScalarT> const& v1, tg::dont_deduce<ScalarT> t)
{
    return v0 + t * (v1 - v0);
}

template <class ScalarT>
[[nodiscard]] constexpr color<3, ScalarT> mix(color<3, ScalarT> const& c0, color<3, ScalarT> const& c1, tg::dont_deduce<ScalarT> t)
{
    return {
        mix(c0.r, c1.r, t), //
        mix(c0.g, c1.g, t), //
        mix(c0.b, c1.b, t)  //
    };
}
template <class ScalarT>
[[nodiscard]] constexpr color<4, ScalarT> mix(color<4, ScalarT> const& c0, color<4, ScalarT> const& c1, tg::dont_deduce<ScalarT> t)
{
    return {
        mix(c0.r, c1.r, t), //
        mix(c0.g, c1.g, t), //
        mix(c0.b, c1.b, t), //
        mix(c0.a, c1.a, t)  //
    };
}

template <int D, class ScalarT>
[[nodiscard]] constexpr comp<D, ScalarT> mix(comp<D, ScalarT> const& v0, comp<D, ScalarT> const& v1, tg::comp<D, ScalarT> t)
{
    return v0 + t * (v1 - v0);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr size<D, ScalarT> mix(size<D, ScalarT> const& v0, size<D, ScalarT> const& v1, tg::comp<D, ScalarT> t)
{
    return v0 + t * (v1 - v0);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr vec<D, ScalarT> mix(vec<D, ScalarT> const& v0, vec<D, ScalarT> const& v1, tg::comp<D, ScalarT> t)
{
    return v0 + t * (v1 - v0);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> mix(pos<D, ScalarT> const& v0, pos<D, ScalarT> const& v1, tg::comp<D, ScalarT> t)
{
    return v0 + t * (v1 - v0);
}

template <class ScalarT>
[[nodiscard]] constexpr color<3, ScalarT> mix(color<3, ScalarT> const& c0, color<3, ScalarT> const& c1, tg::comp<3, ScalarT> t)
{
    return {
        mix(c0.r, c1.r, t.comp0), //
        mix(c0.g, c1.g, t.comp1), //
        mix(c0.b, c1.b, t.comp2)  //
    };
}
template <class ScalarT>
[[nodiscard]] constexpr color<4, ScalarT> mix(color<4, ScalarT> const& c0, color<4, ScalarT> const& c1, tg::comp<4, ScalarT> t)
{
    return {
        mix(c0.r, c1.r, t.comp0), //
        mix(c0.g, c1.g, t.comp1), //
        mix(c0.b, c1.b, t.comp2), //
        mix(c0.a, c1.a, t.comp3)  //
    };
}

// default implementation of lerp is same as mix
template <class A, class B>
[[nodiscard]] constexpr auto lerp(A const& v0, A const& v1, B const& b) -> decltype(mix(v0, v1, b))
{
    return mix(v0, v1, b);
}

// TODO: more types

} // namespace tg
