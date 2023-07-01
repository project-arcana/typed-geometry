#pragma once

#include <clean-core/array.hh>
#include <clean-core/macros.hh>

#include <typed-geometry/tg-lean.hh>

#include <typed-geometry/detail/color-literals.hh>

#include <typed-geometry/functions/basic/minmax.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/functions/color/string-conversions.hh>

// Feature/Color:
// - color spaces (rgb, srgb, hsv, lab, yiq, yuv, ...)
// - static handling of srgb (via types)
// - support for alpha and premultiplied alpha
// - color space conversions
// - different representation (8bit, float)
// - color manipulation (darken, lighten, invert, rotate, ...)
// - parsing ("#rgb" etc)
// - literals ("#FF00FF"_srgb)
// - colormaps and distributions
//
// inspired by https://github.com/bgrins/TinyColor

/// common color types
///
/// tg::color3 - linear RGB (in BT.709)
/// tg::color4 - linear RGB (in BT.709) with straight alpha
/// tg::srgb8  - sRGB stored in gamma-corrected 8bit
/// tg::srgba8 - sRGB stored in gamma-corrected 8bit with straight alpha
///
/// Notes:
///  - there is no rgb8 type as that is usually NOT what is desired
///  - only color<D, T> are comp-like types (subscript, explicit-init-by-other-comp-types, ...)
///    (because all other colors types are a lot more "storage only")
///    (TODO: this might get relaxed in the future)
///  - most types are in-memory first, aka usually used with float
///    compact storage of _colors_ is usually done in srgb8
///    TODO: provide hdr types with 16 bit per channel
///

namespace tg
{
//
// =========================== conversions ===========================
//

/// converts a single component from linear to sRGB gamma-corrected
template <class T>
[[nodiscard]] constexpr T convert_comp_linear_to_srgb(T v)
{
    static_assert(!std::is_integral_v<T>, "only works with floating types currently");
    if (v <= T(0.0031308))
        return T(12.92) * v;
    else
        return T(1.055) * pow(v, T(1.0 / 2.4)) - T(0.055);
}
/// converts a single component from sRGB gamma-corrected to linear
template <class T>
[[nodiscard]] constexpr T convert_comp_srgb_to_linear(T v)
{
    static_assert(!std::is_integral_v<T>, "only works with floating types currently");
    if (v <= T(0.04045))
        return v * T(1 / 12.92);
    else
        return pow(v + T(0.055), T(2.4)) * T(1 / 1.055);
}

namespace detail
{
template <class Color, class = void>
struct color_converter;
}

/// Converts the given color into linear space
/// (If it has alpha, it becomes straight alpha)
template <class Color>
[[nodiscard]] constexpr linear_color_t_of<Color> to_linear_color(Color const& c)
{
    return detail::color_converter<Color>::to_linear(c);
}

/// Converts the given linear color into a target color
template <class Color>
[[nodiscard]] constexpr Color from_linear_color(linear_color_t_of<Color> const& c)
{
    return detail::color_converter<Color>::from_linear(c);
}

/// converts between two color types
/// NOTE: currently requires "shape compatibility", i.e. both alpha or none alpha
template <class ColorTo, class ColorFrom>
[[nodiscard]] constexpr ColorTo convert_color_to(ColorFrom const& c)
{
    if constexpr (std::is_same_v<ColorTo, ColorFrom>)
        return c;
    else
        return tg::from_linear_color<ColorTo>(tg::to_linear_color(c));
}

namespace detail
{

template <class T>
constexpr T hue2rgb(T p, T q, T t)
{
    if (t < 0)
        t += 1;
    if (t > 1)
        t -= 1;
    if (t < T(1.0 / 6))
        return p + (q - p) * T(6) * t;
    if (t < T(1.0 / 2))
        return q;
    if (t < T(2.0 / 3))
        return p + (q - p) * (T(2. / 3) - t) * T(6);

    return p;
}

template <class Color, class LinearColor>
constexpr LinearColor srgb_to_linear(Color const& c)
{
    LinearColor cc;
    cc.r = tg::convert_comp_srgb_to_linear(tg::color_scalar_to_float(c.r));
    cc.g = tg::convert_comp_srgb_to_linear(tg::color_scalar_to_float(c.g));
    cc.b = tg::convert_comp_srgb_to_linear(tg::color_scalar_to_float(c.b));
    if constexpr (tg::has_alpha<Color>)
        cc.a = tg::color_scalar_to_float(c.a);
    return cc;
}

template <class Color, class LinearColor>
constexpr Color srgb_from_linear(LinearColor const& c)
{
    Color cc;
    using T = decltype(cc.r);
    cc.r = tg::color_scalar_from_float<T>(tg::convert_comp_linear_to_srgb(c.r));
    cc.g = tg::color_scalar_from_float<T>(tg::convert_comp_linear_to_srgb(c.g));
    cc.b = tg::color_scalar_from_float<T>(tg::convert_comp_linear_to_srgb(c.b));
    if constexpr (tg::has_alpha<Color>)
        cc.a = tg::color_scalar_from_float<T>(c.a);
    return cc;
}

template <class Color, class LinearColor>
constexpr LinearColor hsl_to_linear(Color const& c)
{
    LinearColor cc;
    using T = decltype(c.l);
    if (c.l == T(0))
    {
        cc.r = T(0);
        cc.g = T(0);
        cc.b = T(0);
    }
    else
    {
        auto const q = c.l < T(0.5) ? c.l * (T(1) + c.s) : c.l + c.s - c.l * c.s;
        auto const p = T(2) * c.l - q;

        cc.r = detail::hue2rgb(p, q, c.h + T(1.0 / 3));
        cc.g = detail::hue2rgb(p, q, c.h);
        cc.b = detail::hue2rgb(p, q, c.h - T(1.0 / 3));
    }
    if constexpr (tg::has_alpha<Color>)
        cc.a = tg::color_scalar_to_float(c.a);
    return cc;
}

template <class Color, class LinearColor>
constexpr Color hsl_from_linear(LinearColor const& c)
{
    Color cc;
    using T = decltype(cc.l);
    auto const M = tg::max(c.r, c.g, c.b);
    auto const m = tg::min(c.r, c.g, c.b);
    cc.l = (M + m) * T(0.5);
    if (M == m) // achromatic?
    {
        cc.h = T(0);
        cc.s = T(0);
    }
    else
    {
        auto const C = M - m;
        cc.s = cc.l > T(0.5) ? C / (T(2) - M - m) : C / (M + m);

        if (M == c.r)
            cc.h = (c.g - c.b) / C * T(60 / 360.0) + (c.g < c.b ? T(1) : T(0));

        else if (M == c.g)
            cc.h = (c.b - c.r) / C * T(60 / 360.0) + T(120 / 360.0);

        else
            cc.h = (c.r - c.g) / C * T(60 / 360.0) + T(240 / 360.0);
    }
    if constexpr (tg::has_alpha<Color>)
        cc.a = tg::color_scalar_from_float<T>(c.a);
    return cc;
}

template <int D, class T>
struct color_converter<color<D, T>>
{
    CC_FORCE_INLINE static constexpr color<D, T> to_linear(color<D, T> const& c) { return c; }
    CC_FORCE_INLINE static constexpr color<D, T> from_linear(color<D, T> const& c) { return c; }
};

template <class T>
struct color_converter<srgb<T>>
{
    using color_t = srgb<T>;
    using linear_color_t = tg::linear_color_t_of<color_t>;

    static constexpr linear_color_t to_linear(color_t const& c) { return detail::srgb_to_linear<color_t, linear_color_t>(c); }
    static constexpr color_t from_linear(linear_color_t const& c) { return detail::srgb_from_linear<color_t, linear_color_t>(c); }
};

template <class T>
struct color_converter<srgba<T>>
{
    using color_t = srgba<T>;
    using linear_color_t = tg::linear_color_t_of<color_t>;

    static constexpr linear_color_t to_linear(color_t const& c) { return detail::srgb_to_linear<color_t, linear_color_t>(c); }
    static constexpr color_t from_linear(linear_color_t const& c) { return detail::srgb_from_linear<color_t, linear_color_t>(c); }
};

template <class T>
struct color_converter<hsl_t<T>>
{
    using color_t = hsl_t<T>;
    using linear_color_t = tg::linear_color_t_of<color_t>;

    static constexpr linear_color_t to_linear(color_t const& c) { return detail::hsl_to_linear<color_t, linear_color_t>(c); }
    static constexpr color_t from_linear(linear_color_t const& c) { return detail::hsl_from_linear<color_t, linear_color_t>(c); }
};

template <class T>
struct color_converter<hsla_t<T>>
{
    using color_t = hsla_t<T>;
    using linear_color_t = tg::linear_color_t_of<color_t>;

    static constexpr linear_color_t to_linear(color_t const& c) { return detail::hsl_to_linear<color_t, linear_color_t>(c); }
    static constexpr color_t from_linear(linear_color_t const& c) { return detail::hsl_from_linear<color_t, linear_color_t>(c); }
};
}

template <class T>
[[nodiscard]] constexpr auto to_linear_color(hsla_t<T> const& c) -> linear_color_t_of<hsl_t<T>>
{
    linear_color_t_of<hsla_t<T>> cc;
    if (c.l == T(0))
    {
        cc.r = T(0);
        cc.g = T(0);
        cc.b = T(0);
    }
    else
    {
        auto const q = c.l < T(0.5) ? c.l * (T(1) + c.s) : c.l + c.s - c.l * c.s;
        auto const p = T(2) * c.l - q;

        cc.r = detail::hue2rgb(p, q, c.h + T(1.0 / 3));
        cc.g = detail::hue2rgb(p, q, c.h);
        cc.b = detail::hue2rgb(p, q, c.h - T(1.0 / 3));
    }
    cc.a = c.a;
    return cc;
}

//
// =========================== properties ===========================
// TODO: many of these properties can be performance optimized by special overloads
//

/// returns the "attribute of a visual sensation according to which an area appears to emit more or less light"
/// NOTE: this is the 'V' of HSV
/// NOTE: do not confuse with luminance or luma
/// result is in [0,1] and floating-point
template <class Color>
[[nodiscard]] constexpr auto brightness_of(Color const& color)
{
    auto const c = tg::to_linear_color(color);
    return max(c.r, c.g, c.b);
}

/// returns the intensity of a color (the I in HSI)
/// result is in [0,1] and floating-point
template <class Color>
[[nodiscard]] constexpr auto intensity_of(Color const& color)
{
    auto const c = tg::to_linear_color(color);
    return (c.r + c.g + c.b) / 3;
}

/// returns the "brightness relative to the brightness of a similarly illuminated white" (the L in HSL)
/// result is in [0,1] and floating-point
template <class Color>
[[nodiscard]] constexpr auto lightness_of(Color const& color)
{
    auto const c = tg::to_linear_color(color);
    auto const M = max(c.r, c.g, c.b);
    auto const m = min(c.r, c.g, c.b);
    return (M + m) / 2;
}

/// returns the "colorfulness relative to the brightness of a similarly illuminated white"
/// result is in [0,1] and floating-point
template <class Color>
[[nodiscard]] constexpr auto chroma_of(Color const& color)
{
    auto const c = tg::to_linear_color(color);
    auto const M = max(c.r, c.g, c.b);
    auto const m = min(c.r, c.g, c.b);
    return M - m;
}

/// returns the "colorfulness of a stimulus relative to its own brightness" (the S in HSV/HSL)
/// result is in [0,1] and floating-point
template <class Color>
[[nodiscard]] constexpr auto saturation_of(Color const& color)
{
    auto const c = tg::to_linear_color(color);
    using T = decltype(c.r);
    auto const M = max(c.r, c.g, c.b);
    auto const m = min(c.r, c.g, c.b);
    auto const C = M - m;
    if (M <= T(0))
        return T(0);
    else
        return C / M;
}

/// returns the "attribute of a visual sensation according to which an area appears to be similar to one of the perceived colors: red, yellow, green,
/// and blue, or to a combination of two of them" result is in [0, 1] and floating-point. hue of black is 0
/// NOTE: the result is NOT [0, 6] or [0, 360]
template <class Color>
[[nodiscard]] constexpr auto hue_of(Color const& color)
{
    auto const c = tg::to_linear_color(color);
    using T = decltype(c.r);
    auto const M = max(c.r, c.g, c.b);
    auto const m = min(c.r, c.g, c.b);
    auto const C = M - m;

    if (C <= T(0))
        return T(0);

    else if (M == c.r)
        return (c.g - c.b) / C * T(60 / 360.0) + (c.g < c.b ? T(1) : T(0));

    else if (M == c.g)
        return (c.b - c.r) / C * T(60 / 360.0) + T(120 / 360.0);

    else
        return (c.r - c.g) / C * T(60 / 360.0) + T(240 / 360.0);
}

/// returns the relative luminance (Rec. 709 "brightness")
/// result is in [0,1] and floating-point
template <class Color>
[[nodiscard]] constexpr auto luminance_of(Color const& color)
{
    auto const c = tg::to_linear_color(color);
    using T = decltype(c.r);
    return T(0.2126) * c.r + T(0.7152) * c.g + T(0.0722) * c.b;
}

/// returns the luma (Rec. 709 "brightness" but gamma-corrected)
/// result is in [0,1] and floating-point
/// TODO: is this really sRGB or some other correction?
template <class Color>
[[nodiscard]] constexpr auto luma_of(Color const& color)
{
    auto const c = tg::to_linear_color(color);
    using T = decltype(c.r);
    auto const r = tg::convert_comp_linear_to_srgb(c.r);
    auto const g = tg::convert_comp_linear_to_srgb(c.g);
    auto const b = tg::convert_comp_linear_to_srgb(c.b);
    return T(0.2126) * r + T(0.7152) * g + T(0.0722) * b;
}

/// returns true iff the colors perceived brightness is light
template <class Color>
[[nodiscard]] constexpr bool is_light(Color const& color)
{
    return brightness_of(color) >= 0.5f;
}

/// returns true iff the colors perceived brightness is dark
template <class Color>
[[nodiscard]] constexpr bool is_dark(Color const& color)
{
    return brightness_of(color) < 0.5f;
}

/// lightens a color by a relative value (changes L in HSL)
/// 0.0 means unchanged
/// 1.0 means white
template <class Color>
[[nodiscard]] constexpr Color lightened(Color const& color, float amount = 0.1f)
{
    auto cc = tg::convert_color_to<hsl>(color);
    cc.l = tg::saturate(cc.l + amount);
    return tg::convert_color_to<Color>(cc);
}

/// darkens a color by a relative value (changes L in HSL)
/// 0.0 means unchanged
/// 1.0 means black
template <class Color>
[[nodiscard]] constexpr Color darkened(Color const& color, float amount = 0.1f)
{
    auto cc = tg::convert_color_to<hsl>(color);
    cc.l = tg::saturate(cc.l - amount);
    return tg::convert_color_to<Color>(cc);
}

/// brightens a color by a relative value (changes V in HSV by increasing all components at the same time)
/// 0.0 means unchanged
/// 1.0 means white
template <class Color>
[[nodiscard]] constexpr Color brightened(Color const& color, float amount = 0.1f)
{
    auto cc = tg::to_linear_color(color);
    cc.r = tg::saturate(cc.r + amount);
    cc.g = tg::saturate(cc.g + amount);
    cc.b = tg::saturate(cc.b + amount);
    return tg::from_linear_color<Color>(cc);
}

/// saturates a color by a relative value (changes S in HSL)
/// 0.0 means unchanged
/// 1.0 means as-vibrant-as-possible
/// CAUTION: do not confuse with tg::saturate
template <class Color>
[[nodiscard]] constexpr Color saturated(Color const& color, float amount = 0.1f)
{
    static_assert(tg::is_color<Color>, "did you mean tg::saturate?");
    auto cc = tg::convert_color_to<hsl>(color);
    cc.s = tg::saturate(cc.s + amount);
    return tg::convert_color_to<Color>(cc);
}

/// desaturates a color by a relative value (changes S in HSL)
/// 0.0 means unchanged
/// 1.0 means grayscale
template <class Color>
[[nodiscard]] constexpr Color desaturated(Color const& color, float amount = 0.1f)
{
    auto cc = tg::convert_color_to<hsl>(color);
    cc.s = tg::saturate(cc.s - amount);
    return tg::convert_color_to<Color>(cc);
}

/// converts a color to grayscale (but staying in the same color space/type)
template <class Color>
[[nodiscard]] constexpr Color grayscale_of(Color const& color)
{
    auto cc = tg::convert_color_to<hsl>(color);
    cc.s = 0;
    return tg::convert_color_to<Color>(cc);
}

/// returns the complementary color (i.e. hue + 120°)
template <class Color>
[[nodiscard]] constexpr Color complement_of(Color const& color)
{
    auto cc = tg::convert_color_to<hsl>(color);
    cc.h = tg::fract(cc.h + 0.5f);
    return tg::convert_color_to<Color>(cc);
}

/// returns the color triad (i.e. color, color + 120°, color + 240°)
template <class Color>
[[nodiscard]] constexpr cc::array<Color, 3> triad_of(Color const& color)
{
    auto c0 = tg::convert_color_to<hsl>(color);
    auto c1 = c0;
    auto c2 = c0;
    c1.h = tg::fract(c0.h + 1.f / 3);
    c2.h = tg::fract(c0.h + 2.f / 3);
    return {color, tg::convert_color_to<Color>(c1), tg::convert_color_to<Color>(c2)};
}

/// returns the color tetrad
template <class Color>
[[nodiscard]] constexpr cc::array<Color, 4> tetrad_of(Color const& color)
{
    auto c0 = tg::convert_color_to<hsl>(color);
    auto c1 = c0;
    auto c2 = c0;
    auto c3 = c0;
    c1.h = tg::fract(c0.h + 1.f / 4);
    c2.h = tg::fract(c0.h + 2.f / 4);
    c3.h = tg::fract(c0.h + 3.f / 4);
    return {color, tg::convert_color_to<Color>(c1), tg::convert_color_to<Color>(c2), tg::convert_color_to<Color>(c3)};
}

/// returns the color triad (i.e. color, color + 120°, color + 240°)
template <class Color>
[[nodiscard]] constexpr cc::array<Color, 3> split_complement_of(Color const& color)
{
    auto c0 = tg::convert_color_to<hsl>(color);
    auto c1 = c0;
    auto c2 = c0;
    c1.h = tg::fract(c0.h + 72.f / 360);
    c2.h = tg::fract(c0.h + 216.f / 360);
    return {color, tg::convert_color_to<Color>(c1), tg::convert_color_to<Color>(c2)};
}

/// rotates the hue of a color
/// 0.00 means unchanged
/// 0.33 means "one major shift" (red -> green -> blue)
/// 1.00 means unchanged as well (360°)
template <class Color>
[[nodiscard]] constexpr Color rotate_color(Color const& color, float amount)
{
    auto cc = tg::convert_color_to<hsl>(color);
    cc.h = tg::fract(cc.h + amount);
    return tg::convert_color_to<Color>(cc);
}

}
