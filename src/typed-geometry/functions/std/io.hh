#pragma once

#include <sstream>
#include <string>

#include <typed-geometry/types/types.hh>

#include <typed-geometry/detail/tg_traits.hh>

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
template <class CharT, class StreamTraits>
std::basic_ostringstream<CharT, StreamTraits> temp_sstream(std::basic_ostream<CharT, StreamTraits> const& out)
{
    std::basic_ostringstream<CharT, StreamTraits> ss;
    ss.flags(out.flags());
    ss.imbue(out.getloc());
    ss.precision(out.precision());
    return ss;
}
}

//
// =============================== Random ===============================
//

template <class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, xorshift const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << "rng(" << std::hex << val.state() << ")";
    return out << ss.str();
}

//
// =============================== Scalars ===============================
//

template <class T, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, angle_t<T> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << val.degree() << "°";
    return out << ss.str();
}

template <class T, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, fwd_diff<T> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << "<" << val.value << ", " << val.derivative << ">";
    return out << ss.str();
}

template <class T, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, interval<T> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << "[" << val.min << ".." << val.max << "]";
    return out << ss.str();
}

template <int w, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, fixed_uint<w> const& val)
{
    CharT buf[w * 20 + 1];
    auto const end = buf + sizeof(buf);
    auto begin = end;
    *(--begin) = '\0';
    auto u = val;
    while (u > 0)
    {
        auto const n = u64(u % 10u);
        u /= 10u;
        *(--begin) = CharT('0' + n);
    }
    if (begin + 1 == end)
        *(--begin) = CharT('0');
    return out << begin;
}

template <int w, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, fixed_int<w> const& val)
{
    if (val < 0)
    {
        out << '-';
        return out << fixed_uint<w>(-val);
    }
    return out << fixed_uint<w>(val);
}

//
// =============================== Comps ===============================
//

template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, comp<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "comp" << char('0' + D);
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, vec<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "vec" << char('0' + D);
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, dir<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "dir" << char('0' + D);
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, pos<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "pos" << char('0' + D);
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, size<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "size" << char('0' + D);
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << val[i];
    ss << ")";
    return out << ss.str();
}
template <class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, color3 const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << "color3(" << val.r << ", " << val.g << ", " << val.b << ")";
    return out << ss.str();
}
template <class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, color4 const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << "color4(" << val.r << ", " << val.g << ", " << val.b << ", " << val.a << ")";
    return out << ss.str();
}
template <class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, quaternion<ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "quat";
    ss << "(" << val.x << ", " << val.y << ", " << val.z << ", " << val.w << ")";
    return out << ss.str();
}

//
// =============================== Transformations ===============================
//

template <int C, int R, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, mat<C, R, ScalarT> const& val)
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

//
// =============================== Objects ===============================
//

template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, line<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "line" << char('0' + D);
    ss << "(" << val.pos << ", " << val.dir << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, ray<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "ray" << char('0' + D);
    ss << "(" << val.origin << ", " << val.dir << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, segment<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "segment" << char('0' + D);
    ss << "(" << val.pos0 << ", " << val.pos1 << ")";
    return out << ss.str();
}


template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, halfspace<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "halfspace" << char('0' + D);
    ss << "(" << val.normal << ", " << val.dis << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, plane<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << (D == 3 ? "plane" : "plane") << char('0' + D);
    ss << "(" << val.normal << ", " << val.dis << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, triangle<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "triangle" << char('0' + D);
    ss << "(" << val.pos0 << ", " << val.pos1 << ", " << val.pos2 << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, quad<D, ScalarT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "quad" << char('0' + D);
    ss << "(" << val.pos00 << ", " << val.pos10 << ", " << val.pos11 << ", " << val.pos01 << ")";
    return out << ss.str();
}


template <int D, class ScalarT, class TraitsT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, aabb<D, ScalarT, TraitsT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "aabb" << char('0' + D) << TraitsT::suffix;
    ss << "(" << val.min << ", " << val.max << ")";
    return out << ss.str();
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, box<ObjectD, ScalarT, DomainD, TraitsT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "box" << char('0' + ObjectD);
    if constexpr (ObjectD != DomainD)
        ss << "in" << char('0' + DomainD);
    ss << TraitsT::suffix;
    ss << "(" << val.center << ", " << val.half_extents << ")";
    return out << ss.str();
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, ellipse<ObjectD, ScalarT, DomainD, TraitsT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "ellipse" << char('0' + ObjectD);
    if constexpr (ObjectD != DomainD)
        ss << "in" << char('0' + DomainD);
    ss << TraitsT::suffix;
    ss << "(" << val.center << ", " << val.semi_axes << ")";
    return out << ss.str();
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, sphere<ObjectD, ScalarT, DomainD, TraitsT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "sphere" << char('0' + ObjectD);
    if constexpr (ObjectD != DomainD)
        ss << "in" << char('0' + DomainD);
    ss << TraitsT::suffix;

    if constexpr (ObjectD == 2 && DomainD == 3)
        ss << "(" << val.center << ", " << val.radius << ", " << val.normal << ")";
    else
        ss << "(" << val.center << ", " << val.radius << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class TraitsT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, capsule<D, ScalarT, TraitsT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "capsule" << char('0' + D) << TraitsT::suffix;
    ss << "(" << val.axis << ", " << val.radius << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class TraitsT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, cylinder<D, ScalarT, TraitsT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "cylinder" << char('0' + D) << TraitsT::suffix;
    ss << "(" << val.axis << ", " << val.radius << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class TraitsT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, hemisphere<D, ScalarT, TraitsT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "hemisphere" << char('0' + D) << TraitsT::suffix;
    ss << "(" << val.center << ", " << val.radius << ", " << val.normal << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class TraitsT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, inf_cone<D, ScalarT, TraitsT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "inf_cone" << char('0' + D) << TraitsT::suffix;
    ss << "(" << val.apex << ", " << val.opening_dir << ", " << val.opening_angle << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class TraitsT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, inf_cylinder<D, ScalarT, TraitsT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "inf_cylinder" << char('0' + D) << TraitsT::suffix;
    ss << "(" << val.axis << ", " << val.radius << ")";
    return out << ss.str();
}

template <class BaseT, class TraitsT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, pyramid<BaseT, TraitsT> const& val)
{
    using ScalarT = typename BaseT::scalar_t;
    auto ss = detail::temp_sstream(out);
    ss << type_name_prefix<ScalarT> << "pyramid" << TraitsT::suffix;
    ss << "(" << val.base << ", " << val.height << ")";
    return out << ss.str();
}


//
// =============================== Bezier ===============================
//

template <int D, class ControlPointT, class CharT, class StreamTraits>
std::basic_ostream<CharT, StreamTraits>& operator<<(std::basic_ostream<CharT, StreamTraits>& out, bezier<D, ControlPointT> const& val)
{
    auto ss = detail::temp_sstream(out);
    ss << "bezier" << char('0' + D);
    ss << "(";
    ss << val.control_points[0];
    for (auto i = 1; i <= D; ++i)
        ss << ", " << val.control_points[i];
    ss << ")";
    return out << ss.str();
}
} // namespace tg
