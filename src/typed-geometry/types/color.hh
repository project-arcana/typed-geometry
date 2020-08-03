#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../detail/comp_traits.hh"
#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "fwd.hh"

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

    TG_DECLARE_COMP_TYPE_3(color);
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
    ScalarT r = static_cast<ScalarT>(0);
    ScalarT g = static_cast<ScalarT>(0);
    ScalarT b = static_cast<ScalarT>(0);
    ScalarT a = static_cast<ScalarT>(1);

    static const color black;
    static const color white;
    static const color red;
    static const color green;
    static const color blue;
    static const color cyan;
    static const color magenta;
    static const color yellow;
    static const color transparent;

    TG_DECLARE_COMP_TYPE_4(color);

    constexpr color(ScalarT r, ScalarT g, ScalarT b) : r(r), g(g), b(b) {}
    constexpr color(color<3, ScalarT> const& rgb, ScalarT a = ScalarT(1)) : r(rgb.r), g(rgb.g), b(rgb.b), a(a) {}
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
const color<4, T> color<4, T>::transparent = {T(0), T(0), T(0), T(0)};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(color, color, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(color, color, bool, operator!=, ||, !=);

// deduction guides
TG_IMPL_COMP_DEDUCTION_GUIDES(color);

// reflection
TG_IMPL_COMP_INTROSPECT(color);

} // namespace tg
