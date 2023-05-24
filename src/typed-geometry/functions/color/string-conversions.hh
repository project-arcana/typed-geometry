#pragma once

#include <clean-core/string_view.hh>

#include <typed-geometry/types/color.hh>
#include <typed-geometry/types/srgb.hh>

namespace tg
{
namespace detail
{
// -1 if invalid
constexpr int hex_from_char(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';

    if ('a' <= c && c <= 'f')
        return 10 + (c - 'a');

    if ('A' <= c && c <= 'F')
        return 10 + (c - 'A');

    return -1;
}

// -1 if invalid
constexpr int hex_from_str12(cc::string_view s)
{
    if (s.size() == 1)
    {
        auto i = hex_from_char(s[0]);
        if (i == -1)
            return -1;

        return i * 16 + i;
    }

    if (s.size() == 2)
    {
        auto i0 = hex_from_char(s[0]);
        auto i1 = hex_from_char(s[1]);
        if (i0 == -1 || i1 == -1)
            return -1;

        return i0 * 16 + i1;
    }

    return -1;
}

template <class T>
constexpr bool parse_color_comp(T& c, cc::string_view s)
{
    auto i = hex_from_str12(s);
    if (i < 0)
        return false;

    if constexpr (std::is_same_v<T, u8>)
        c = u8(i);
    else
        c = tg::color_scalar_from_float<T>(i / 255.f);

    return true;
}
}

template <class ColorT>
constexpr bool parse_color_from_hex_string(ColorT& c, cc::string_view s)
{
    using T = std::decay_t<decltype(c.r)>;

    s = s.trim().trim('#');
    if (s.starts_with("0x"))
        s = s.subview(2);

    switch (s.size())
    {
    case 3: // rgb
        if (!detail::parse_color_comp(c.r, s.subview(0, 1)))
            return false;
        if (!detail::parse_color_comp(c.g, s.subview(1, 1)))
            return false;
        if (!detail::parse_color_comp(c.b, s.subview(2, 1)))
            return false;
        if constexpr (tg::has_alpha<ColorT>)
            c.a = detail::color_scalar<T>::one();
        return true;

    case 6: // rrggbb
        if (!detail::parse_color_comp(c.r, s.subview(0, 2)))
            return false;
        if (!detail::parse_color_comp(c.g, s.subview(2, 2)))
            return false;
        if (!detail::parse_color_comp(c.b, s.subview(4, 2)))
            return false;
        if constexpr (tg::has_alpha<ColorT>)
            c.a = detail::color_scalar<T>::one();
        return true;
    }

    if constexpr (tg::has_alpha<ColorT>)
    {
        switch (s.size())
        {
        case 4: // rgba
            if (!detail::parse_color_comp(c.r, s.subview(0, 1)))
                return false;
            if (!detail::parse_color_comp(c.g, s.subview(1, 1)))
                return false;
            if (!detail::parse_color_comp(c.b, s.subview(2, 1)))
                return false;
            if (!detail::parse_color_comp(c.a, s.subview(3, 1)))
                return false;
            return true;

        case 8: // rrggbbaa
            if (!detail::parse_color_comp(c.r, s.subview(0, 2)))
                return false;
            if (!detail::parse_color_comp(c.g, s.subview(2, 2)))
                return false;
            if (!detail::parse_color_comp(c.b, s.subview(4, 2)))
                return false;
            if (!detail::parse_color_comp(c.a, s.subview(6, 2)))
                return false;
            return true;
        }
    }

    return false;
}

template <class ScalarT>
constexpr color<3, ScalarT> color<3, ScalarT>::from_hex_string(cc::string_view s)
{
    color<3, ScalarT> c;
    auto ok = tg::parse_color_from_hex_string(c, s);
    CC_ASSERT(ok && "could not parse color");
    return c;
}

template <class ScalarT>
constexpr color<4, ScalarT> color<4, ScalarT>::from_hex_string(cc::string_view s)
{
    color<4, ScalarT> c;
    auto ok = tg::parse_color_from_hex_string(c, s);
    CC_ASSERT(ok && "could not parse color");
    return c;
}

template <class ScalarT>
constexpr srgb<ScalarT> srgb<ScalarT>::from_hex_string(cc::string_view s)
{
    srgb<ScalarT> c;
    auto ok = tg::parse_color_from_hex_string(c, s);
    CC_ASSERT(ok && "could not parse color");
    return c;
}

template <class ScalarT>
constexpr srgba<ScalarT> srgba<ScalarT>::from_hex_string(cc::string_view s)
{
    srgba<ScalarT> c;
    auto ok = tg::parse_color_from_hex_string(c, s);
    CC_ASSERT(ok && "could not parse color");
    return c;
}
}
