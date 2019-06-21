#pragma once

#include <sstream>
#include <string>

#include <typed-geometry/types/types.hh>

#include "../detail/tg_traits.hh"

namespace tg
{
template <class T>
std::string to_string(T const& v)
{
    std::ostringstream ss;
    ss << v;
    return ss.str();
}

namespace detail
{
template <class CharT, class Traits>
std::basic_ostringstream<CharT, Traits> temp_sstream(std::basic_ostream<CharT, Traits> const& out)
{
    std::basic_ostringstream<CharT, Traits> ss;
    ss.flags(out.flags());
    ss.imbue(out.getloc());
    ss.precision(out.precision());
    return ss;
}
}

// guideline:
// - the output should be valid C++ (and reconstruct the type)

// how to add a new type:
// - extend traits.hh:
//     template <int D, class ScalarT>
//     struct type_name_t<aabb<D, ScalarT>>
//     {
//         static constexpr char const* value = "aabb";
//     };

template <class T, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, angle_t<T> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << val.degree() << "°";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, comp<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "comp" << char('0' + D);
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, vec<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "vec" << char('0' + D);
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, dir<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "dir" << char('0' + D);
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, pos<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "pos" << char('0' + D);
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, size<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "size" << char('0' + D);
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, color3 const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << "color3(" << val.r << ", " << val.g << ", " << val.b << ")";
    return out << ss.str();
}
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, color4 const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << "color4(" << val.r << ", " << val.g << ", " << val.b << ", " << val.a << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, aabb<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "aabb" << char('0' + D);
    ss << "(" << val.min << ", " << val.max << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, box<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "box" << char('0' + D);
    ss << "(" << val.center << ", " << val.half_extents << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, triangle<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "triangle" << char('0' + D);
    ss << "(" << val.pos0 << ", " << val.pos1 << ", " << val.pos2 << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, sphere<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "sphere" << char('0' + D);
    ss << "(" << val.center << ", " << val.radius << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, ball<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "ball" << char('0' + D);
    ss << "(" << val.center << ", " << val.radius << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, line<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "line" << char('0' + D);
    ss << "(" << val.pos << ", " << val.dir << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, hyperplane<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << (D == 3 ? "plane" : "hyperplane") << char('0' + D);
    ss << "(" << val.normal << ", " << val.dis << ")";
    return out << ss.str();
}

template <int C, int R, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, mat<C, R, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "mat";
    if (C == R)
        ss << char('0' + C);
    else
        ss << char('0' + C) << 'x' << char('0' + R);
    ss << "(";
    for (auto i = 0; i < C; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
} // namespace tg
