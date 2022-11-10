#pragma once

#include <clean-core/string_view.hh>

#include <typed-geometry/types/color.hh>
#include <typed-geometry/types/srgb.hh>

#include <typed-geometry/functions/color/string-conversions.hh>

namespace tg
{
namespace literals
{
constexpr srgb8 operator""_srgb8(char const* str, size_t s) { return srgb8::from_hex_string({str, s}); }
constexpr srgba8 operator""_srgba8(char const* str, size_t s) { return srgba8::from_hex_string({str, s}); }
constexpr color3 operator""_color3(char const* str, size_t s) { return color3::from_hex_string({str, s}); }
constexpr color4 operator""_color4(char const* str, size_t s) { return color4::from_hex_string({str, s}); }
} // namespace literals
} // namespace tg

#ifdef TG_EXPORT_LITERALS
using namespace tg::literals;
#endif
