#pragma once

#include <typed-geometry/tg-lean.hh>

// Feature/Color:
// - color spaces (rgb, srgb, hsv, lab, ...)
// - static handling of srgb (via types)
// - support for alpha and premultiplied alpha
// - color space conversions
// - different representation (8bit, float)
// - color manipulation (darken, lighten, invert, rotate, ...)
// - parsing ("#rgb" etc)
// - literals ("#FF00FF"_srgb)
// - colormaps and distributions

/// common color types
///
/// tg::color3 - linear RGB (in BT.709)
/// tg::color4 - linear RGB (in BT.709) with straight alpha
/// tg::srgb8  - sRGB stored in gamma-corrected 8bit
/// tg::srgba8 - sRGB stored in gamma-corrected 8bit with straight alpha
///
/// Notes:
///  - there is not rgb8 type as that is usually NOT what is desired
///  - only color<D, T> are comp-like types (subscript, explicit-init-by-other-comp-types, ...)
///    (because all other colors types are a lot more "storage only")
///    (TODO: this might get relaxed in the future)
///

namespace tg
{
//
// =========================== conversions ===========================
//

/// Converts the given color into linear space
/// (If it has alpha, it becomes straight alpha)
template <class T>
auto to_linear_color(color<3, T> const& c) -> color<3, T>
{
    return c;
}
template <class T>
auto to_linear_color(color<4, T> const& c) -> color<4, T>
{
    return c;
}

}
