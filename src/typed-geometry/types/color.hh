#pragma once

#include <clean-core/fwd.hh>

#include <typed-geometry/types/scalars/default.hh>
#include "../detail/color_traits.hh"
#include "../detail/comp_traits.hh"
#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "fwd.hh"

namespace tg
{
template <int D, class ScalarT>
struct color;

// see feature/colors.hh for rationale / more doc

/// linear color in RGB using float components
using color3 = color<3, f32>;
/// linear color in RGB with straight alpha a using float components
using color4 = color<4, f32>;

// ======== IMPLEMENTATION ========

template <class ScalarT>
struct color<3, ScalarT>
{
    ScalarT r = static_cast<ScalarT>(0);
    ScalarT g = static_cast<ScalarT>(0);
    ScalarT b = static_cast<ScalarT>(0);

    static const color black;   ///< a solid black (0,0,0) color (NOTE: this is a static member, NOT a property)
    static const color white;   ///< a solid white (1,1,1) color (NOTE: this is a static member, NOT a property)
    static const color red;     ///< a solid red (1,0,0) color (NOTE: this is a static member, NOT a property)
    static const color green;   ///< a solid green (0,1,0) color (NOTE: this is a static member, NOT a property)
    static const color blue;    ///< a solid blue (0,0,1) color (NOTE: this is a static member, NOT a property)
    static const color cyan;    ///< a solid cyan (0,1,1) color (NOTE: this is a static member, NOT a property)
    static const color magenta; ///< a solid magenta (1,0,1) color (NOTE: this is a static member, NOT a property)
    static const color yellow;  ///< a solid yellow (1,1,0) color (NOTE: this is a static member, NOT a property)

    TG_DECLARE_COMP_TYPE_3(color);

    // NOTE: requires feature/color included
    constexpr static color from_hex_string(cc::string_view s);
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

    static const color black;       ///< a solid opaque black (0,0,0,1) color (NOTE: this is a static member, NOT a property)
    static const color white;       ///< a solid opaque white (1,1,1,1) color (NOTE: this is a static member, NOT a property)
    static const color red;         ///< a solid opaque red (1,0,0,1) color (NOTE: this is a static member, NOT a property)
    static const color green;       ///< a solid opaque green (0,1,0,1) color (NOTE: this is a static member, NOT a property)
    static const color blue;        ///< a solid opaque blue (0,0,1,1) color (NOTE: this is a static member, NOT a property)
    static const color cyan;        ///< a solid opaque cyan (0,1,1,1) color (NOTE: this is a static member, NOT a property)
    static const color magenta;     ///< a solid opaque magenta (1,0,1,1) color (NOTE: this is a static member, NOT a property)
    static const color yellow;      ///< a solid opaque yellow (1,1,0,1) color (NOTE: this is a static member, NOT a property)
    static const color transparent; ///< a fully transparent black (0,0,0,0) (NOTE: this is a static member, NOT a property)

    TG_DECLARE_COMP_TYPE_4(color);

    constexpr color(ScalarT r, ScalarT g, ScalarT b) : r(r), g(g), b(b) {}
    constexpr color(color<3, ScalarT> const& rgb, ScalarT a = ScalarT(1)) : r(rgb.r), g(rgb.g), b(rgb.b), a(a) {}

    // NOTE: requires feature/color included
    constexpr static color from_hex_string(cc::string_view s);
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

// traits
template <class T>
struct color_traits<color<3, T>> : detail::base_color_traits<3, T, alpha_type::none>
{
};
template <class T>
struct color_traits<color<4, T>> : detail::base_color_traits<4, T, alpha_type::straight>
{
};

} // namespace tg
