#pragma once

#include "../detail/comp_traits.hh"
#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "fwd.hh"
#include "scalar.hh"

namespace tg
{
template <int D, class ScalarT>
struct color;

// Common color types
// TODO: premultiplied alpha vs postmultiplied alpha?
// TODO: float vs u8 colors?
// TODO: sRGB handling

using color3 = color<3, f32>;
using color4 = color<4, f32>;

// ======== IMPLEMENTATION ========

template <class ScalarT>
struct color<3, ScalarT>
{
    ScalarT r = static_cast<ScalarT>(0);
    ScalarT g = static_cast<ScalarT>(0);
    ScalarT b = static_cast<ScalarT>(0);

    static const color black;
    static const color white;
    static const color red;
    static const color green;
    static const color blue;
    static const color cyan;
    static const color magenta;
    static const color yellow;

    constexpr ScalarT& operator[](int i) { return (&r)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&r)[i]; }

    constexpr color() = default;
    constexpr color(color const&) = default;
    constexpr color(color&&) = default;
    constexpr explicit color(ScalarT grey) : r(grey), g(grey), b(grey) {}
    constexpr color(ScalarT r, ScalarT g, ScalarT b) : r(r), g(g), b(b) {}

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr color(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        r = detail::comp_get(v, 0, s, fill);
        g = detail::comp_get(v, 1, s, fill);
        b = detail::comp_get(v, 2, s, fill);
    }

    constexpr color& operator=(color const&) & = default;
    constexpr color& operator=(color const&) && = delete;
    constexpr color& operator=(color&&) & = default;
    constexpr color& operator=(color&&) && = delete;
};

template <class T>
const color<3, T> color<3, T>::black = {T(0), T(0), T(0)};
template <class T>
const color<3, T> color<3, T>::white = {T(1), T(1), T(1)};
template <class T>
const color<3, T> color<3, T>::red = {T(1), T(0), T(0)};
template <class T>
const color<3, T> color<3, T>::green = {T(0), T(1), T(0)};
template <class T>
const color<3, T> color<3, T>::blue = {T(0), T(0), T(1)};
template <class T>
const color<3, T> color<3, T>::cyan = {T(0), T(1), T(1)};
template <class T>
const color<3, T> color<3, T>::magenta = {T(1), T(0), T(1)};
template <class T>
const color<3, T> color<3, T>::yellow = {T(1), T(1), T(0)};

template <class ScalarT>
struct color<4, ScalarT>
{
    static const color black;
    static const color white;
    static const color red;
    static const color green;
    static const color blue;
    static const color cyan;
    static const color magenta;
    static const color yellow;
    static const color transparent;

    ScalarT r = static_cast<ScalarT>(0);
    ScalarT g = static_cast<ScalarT>(0);
    ScalarT b = static_cast<ScalarT>(0);
    ScalarT a = static_cast<ScalarT>(1);

    constexpr ScalarT& operator[](int i) { return (&r)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&r)[i]; }

    constexpr color() = default;
    constexpr color(color const&) = default;
    constexpr color(color&&) = default;
    constexpr explicit color(ScalarT grey) : r(grey), g(grey), b(grey), a(ScalarT(1)) {}
    constexpr color(ScalarT r, ScalarT g, ScalarT b, ScalarT a = ScalarT(1)) : r(r), g(g), b(b), a(a) {}
    constexpr color(color<3, ScalarT> c, ScalarT a = ScalarT(1)) : r(c.r), g(c.g), b(c.b), a(a) {}

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr color(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        r = detail::comp_get(v, 0, s, fill);
        g = detail::comp_get(v, 1, s, fill);
        b = detail::comp_get(v, 2, s, fill);
        a = detail::comp_get(v, 3, s, fill);
    }

    constexpr color& operator=(color const&) & = default;
    constexpr color& operator=(color const&) && = delete;
    constexpr color& operator=(color&&) & = default;
    constexpr color& operator=(color&&) && = delete;
};

template <class T>
const color<4, T> color<4, T>::black = {T(0), T(0), T(0)};
template <class T>
const color<4, T> color<4, T>::white = {T(1), T(1), T(1)};
template <class T>
const color<4, T> color<4, T>::red = {T(1), T(0), T(0)};
template <class T>
const color<4, T> color<4, T>::green = {T(0), T(1), T(0)};
template <class T>
const color<4, T> color<4, T>::blue = {T(0), T(0), T(1)};
template <class T>
const color<4, T> color<4, T>::cyan = {T(0), T(1), T(1)};
template <class T>
const color<4, T> color<4, T>::magenta = {T(1), T(0), T(1)};
template <class T>
const color<4, T> color<4, T>::yellow = {T(1), T(1), T(0)};
template <class T>
const color<4, T> color<4, T>::transparent = {T(0), T(0), T(0), T(1)};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(color, color, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(color, color, bool, operator!=, ||, !=);

// deduction guides
TG_IMPL_COMP_DEDUCTION_GUIDES(color);

} // namespace tg
