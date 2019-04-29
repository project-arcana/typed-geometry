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
    constexpr explicit color(ScalarT v) : r(v), g(v), b(v) {}
    constexpr color(ScalarT r, ScalarT g, ScalarT b) : r(r), g(g), b(b) {}
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
    constexpr explicit color(ScalarT v) : r(v), g(v), b(v), a(v) {}
    constexpr color(ScalarT r, ScalarT g, ScalarT b, ScalarT a = ScalarT(1)) : r(r), g(g), b(b), a(a) {}
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
#ifdef TG_SUPPORT_CXX17
template <class A, class B, class C>
color(A const& r, B const& g, C const& b)->color<3, promoted_scalar<A, promoted_scalar<B, C>>>;
template <class A, class B, class C, class D>
color(A const& r, B const& g, C const& b, D const& a)->color<4, promoted_scalar<promoted_scalar<A, B>, promoted_scalar<C, D>>>;
#endif

} // namespace tg
