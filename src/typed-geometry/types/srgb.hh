#pragma once

#include <clean-core/fwd.hh>

#include <typed-geometry/detail/color_traits.hh>
#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
template <class ScalarT>
struct srgb;
template <class ScalarT>
struct srgba;

// see feature/colors.hh for rationale / more doc

/// color in sRGB (gamme-compressed, 8 bit per channel)
using srgb8 = srgb<u8>;
/// color in sRGB (gamme-compressed, 8 bit per channel) with straight alpha
using srgba8 = srgba<u8>;

// ======== IMPLEMENTATION ========

template <class ScalarT>
struct srgb
{
    ScalarT r = static_cast<ScalarT>(0);
    ScalarT g = static_cast<ScalarT>(0);
    ScalarT b = static_cast<ScalarT>(0);

    static const srgb black;   ///< a solid black (0,0,0) color (NOTE: this is a static member, NOT a property)
    static const srgb white;   ///< a solid white (1,1,1) color (NOTE: this is a static member, NOT a property)
    static const srgb red;     ///< a solid red (1,0,0) color (NOTE: this is a static member, NOT a property)
    static const srgb green;   ///< a solid green (0,1,0) color (NOTE: this is a static member, NOT a property)
    static const srgb blue;    ///< a solid blue (0,0,1) color (NOTE: this is a static member, NOT a property)
    static const srgb cyan;    ///< a solid cyan (0,1,1) color (NOTE: this is a static member, NOT a property)
    static const srgb magenta; ///< a solid magenta (1,0,1) color (NOTE: this is a static member, NOT a property)
    static const srgb yellow;  ///< a solid yellow (1,1,0) color (NOTE: this is a static member, NOT a property)

    srgb() = default;
    srgb(ScalarT r, ScalarT g, ScalarT b) : r(r), g(g), b(b) {}

    constexpr bool operator==(srgb const& v) const { return r == v.r && b == v.b && g == v.g; }
    constexpr bool operator!=(srgb const& v) const { return r != v.r || b != v.b || g != v.g; }

    // NOTE: requires feature/color included
    constexpr static srgb from_hex_string(cc::string_view s);
};

// TODO: 1 vs 255

template <class T>
const srgb<T> srgb<T>::black = {T(0), T(0), T(0)};
template <class T>
const srgb<T> srgb<T>::white = {T(255), T(255), T(255)};
template <class T>
const srgb<T> srgb<T>::red = {T(255), T(0), T(0)};
template <class T>
const srgb<T> srgb<T>::green = {T(0), T(255), T(0)};
template <class T>
const srgb<T> srgb<T>::blue = {T(0), T(0), T(255)};
template <class T>
const srgb<T> srgb<T>::cyan = {T(0), T(255), T(255)};
template <class T>
const srgb<T> srgb<T>::magenta = {T(255), T(0), T(255)};
template <class T>
const srgb<T> srgb<T>::yellow = {T(255), T(255), T(0)};


template <class ScalarT>
struct srgba
{
    ScalarT r = static_cast<ScalarT>(0);
    ScalarT g = static_cast<ScalarT>(0);
    ScalarT b = static_cast<ScalarT>(0);
    ScalarT a = color_scalar_from_float<ScalarT>(1);

    static const srgba black;       ///< a solid opaque black (0,0,0,1) color (NOTE: this is a static member, NOT a property)
    static const srgba white;       ///< a solid opaque white (1,1,1,1) color (NOTE: this is a static member, NOT a property)
    static const srgba red;         ///< a solid opaque red (1,0,0,1) color (NOTE: this is a static member, NOT a property)
    static const srgba green;       ///< a solid opaque green (0,1,0,1) color (NOTE: this is a static member, NOT a property)
    static const srgba blue;        ///< a solid opaque blue (0,0,1,1) color (NOTE: this is a static member, NOT a property)
    static const srgba cyan;        ///< a solid opaque cyan (0,1,1,1) color (NOTE: this is a static member, NOT a property)
    static const srgba magenta;     ///< a solid opaque magenta (1,0,1,1) color (NOTE: this is a static member, NOT a property)
    static const srgba yellow;      ///< a solid opaque yellow (1,1,0,1) color (NOTE: this is a static member, NOT a property)
    static const srgba transparent; ///< a fully transparent black (0,0,0,0) (NOTE: this is a static member, NOT a property)

    srgba() = default;
    srgba(ScalarT r, ScalarT g, ScalarT b, ScalarT a = color_scalar_from_float<ScalarT>(1)) : r(r), g(g), b(b), a(a) {}

    constexpr bool operator==(srgba const& v) const { return r == v.r && b == v.b && g == v.g && a == v.a; }
    constexpr bool operator!=(srgba const& v) const { return r != v.r || b != v.b || g != v.g || a != v.a; }

    // NOTE: requires feature/color included
    constexpr static srgba from_hex_string(cc::string_view s);
};

template <class T>
const srgba<T> srgba<T>::black = {T(0), T(0), T(0), T(255)};
template <class T>
const srgba<T> srgba<T>::white = {T(255), T(255), T(255), T(255)};
template <class T>
const srgba<T> srgba<T>::red = {T(255), T(0), T(0), T(255)};
template <class T>
const srgba<T> srgba<T>::green = {T(0), T(255), T(0), T(255)};
template <class T>
const srgba<T> srgba<T>::blue = {T(0), T(0), T(255), T(255)};
template <class T>
const srgba<T> srgba<T>::cyan = {T(0), T(255), T(255), T(255)};
template <class T>
const srgba<T> srgba<T>::magenta = {T(255), T(0), T(255), T(255)};
template <class T>
const srgba<T> srgba<T>::yellow = {T(255), T(255), T(0), T(255)};
template <class T>
const srgba<T> srgba<T>::transparent = {T(0), T(0), T(0), T(0)};

// reflection
template <class I, class ScalarT>
constexpr void introspect(I&& i, srgb<ScalarT>& v)
{
    i(v.r, "r");
    i(v.g, "g");
    i(v.b, "b");
}
template <class I, class ScalarT>
constexpr void introspect(I&& i, srgba<ScalarT>& v)
{
    i(v.r, "r");
    i(v.g, "g");
    i(v.b, "b");
    i(v.a, "a");
}

// traits
template <class T>
struct color_traits<srgb<T>> : detail::base_color_traits<3, T, alpha_type::none>
{
};
template <class T>
struct color_traits<srgba<T>> : detail::base_color_traits<4, T, alpha_type::straight>
{
};
}
