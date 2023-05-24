#pragma once

#include <clean-core/fwd.hh>

#include <typed-geometry/detail/color_traits.hh>
#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
template <class ScalarT>
struct hsl_t;
template <class ScalarT>
struct hsla_t;

// see feature/colors.hh for rationale / more doc

/// color in hsl, float per comp
using hsl = hsl_t<f32>;
/// color in hsl with straight alpha, float per comp
using hsla = hsla_t<f32>;

// ======== IMPLEMENTATION ========

template <class ScalarT>
struct hsl_t
{
    ScalarT h = static_cast<ScalarT>(0); // [0, 1] NOT [0, 6] or [0, 360]
    ScalarT s = static_cast<ScalarT>(0); // [0, 1]
    ScalarT l = static_cast<ScalarT>(0); // [0, 1]

    hsl_t() = default;
    hsl_t(ScalarT h, ScalarT s, ScalarT l) : h(h), s(s), l(l) {}

    constexpr bool operator==(hsl_t const& v) const { return h == v.h && l == v.l && s == v.s; }
    constexpr bool operator!=(hsl_t const& v) const { return h != v.h || l != v.l || s != v.s; }
};

template <class ScalarT>
struct hsla_t
{
    ScalarT h = static_cast<ScalarT>(0); // [0, 1] NOT [0, 6] or [0, 360]
    ScalarT s = static_cast<ScalarT>(0); // [0, 1]
    ScalarT l = static_cast<ScalarT>(0); // [0, 1]
    ScalarT a = color_scalar_from_float<ScalarT>(1);

    hsla_t() = default;
    hsla_t(ScalarT h, ScalarT s, ScalarT l, ScalarT a = color_scalar_from_float<ScalarT>(1)) : h(h), s(s), l(l), a(a) {}

    constexpr bool operator==(hsla_t const& v) const { return h == v.h && l == v.l && s == v.s && a == v.a; }
    constexpr bool operator!=(hsla_t const& v) const { return h != v.h || l != v.l || s != v.s || a != v.a; }
};

// reflection
template <class I, class ScalarT>
constexpr void introspect(I&& i, hsl_t<ScalarT>& v)
{
    i(v.h, "h");
    i(v.s, "s");
    i(v.l, "l");
}
template <class I, class ScalarT>
constexpr void introspect(I&& i, hsla_t<ScalarT>& v)
{
    i(v.h, "h");
    i(v.s, "s");
    i(v.l, "l");
    i(v.a, "a");
}

// traits
template <class T>
struct color_traits<hsl_t<T>> : detail::base_color_traits<3, T, alpha_type::none>
{
};
template <class T>
struct color_traits<hsla_t<T>> : detail::base_color_traits<4, T, alpha_type::straight>
{
};
}
