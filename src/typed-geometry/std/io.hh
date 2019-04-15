#pragma once

#include <sstream>
#include <string>

#include "../types/types.hh"

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

// guideline:
// - the output should be valid C++ (and reconstruct the type)

// how to add a new type:
// - extend traits.hh:
//     template <int D, class ScalarT>
//     struct type_name_t<box<D, ScalarT>>
//     {
//         static constexpr char const* value = "box";
//     };

template <class T, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, angle<T> const& a)
{
    std::basic_ostringstream<CharT, Traits> ss;
    ss.flags(out.flags());
    ss.imbue(out.getloc());
    ss.precision(out.precision());

    ss << a.degree() << "°";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, vec<D, ScalarT> const& v)
{
    using T = vec<D, ScalarT>;

    std::basic_ostringstream<CharT, Traits> ss;
    ss.flags(out.flags());
    ss.imbue(out.getloc());
    ss.precision(out.precision());

    ss << type_name_prefix<T>;
    ss << type_name<T>;
    ss << type_name_suffix<T>;
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << v[i];
    ss << ")";
    return out << ss.str();
}
template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, pos<D, ScalarT> const& p)
{
    using T = pos<D, ScalarT>;

    std::basic_ostringstream<CharT, Traits> ss;
    ss.flags(out.flags());
    ss.imbue(out.getloc());
    ss.precision(out.precision());

    ss << type_name_prefix<T>;
    ss << type_name<T>;
    ss << type_name_suffix<T>;
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << p[i];
    ss << ")";
    return out << ss.str();
}
template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, size<D, ScalarT> const& s)
{
    using T = size<D, ScalarT>;

    std::basic_ostringstream<CharT, Traits> ss;
    ss.flags(out.flags());
    ss.imbue(out.getloc());
    ss.precision(out.precision());

    ss << type_name_prefix<T>;
    ss << type_name<T>;
    ss << type_name_suffix<T>;
    ss << "(";
    for (auto i = 0; i < D; ++i)
        ss << (i > 0 ? ", " : "") << s[i];
    ss << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, box<D, ScalarT> const& b)
{
    using T = box<D, ScalarT>;

    std::basic_ostringstream<CharT, Traits> ss;
    ss.flags(out.flags());
    ss.imbue(out.getloc());
    ss.precision(out.precision());

    ss << type_name_prefix<T>;
    ss << type_name<T>;
    ss << type_name_suffix<T>;
    ss << "(" << b.min << ", " << b.max << ")";
    return out << ss.str();
}

template <int D, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, triangle<D, ScalarT> const& t)
{
    using T = triangle<D, ScalarT>;

    std::basic_ostringstream<CharT, Traits> ss;
    ss.flags(out.flags());
    ss.imbue(out.getloc());
    ss.precision(out.precision());

    ss << type_name_prefix<T>;
    ss << type_name<T>;
    ss << type_name_suffix<T>;
    ss << "(" << t.v0 << ", " << t.v1 << ", " << t.v2 << ")";
    return out << ss.str();
}

template <int C, int R, class ScalarT, class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, mat<C, R, ScalarT> const& m)
{
    using T = mat<C, R, ScalarT>;

    std::basic_ostringstream<CharT, Traits> ss;
    ss.flags(out.flags());
    ss.imbue(out.getloc());
    ss.precision(out.precision());

    ss << type_name_prefix<T>;
    ss << type_name<T>;
    if (C == R)
        ss << '0' + C;
    else
        ss << char('0' + C) << 'x' << char('0' + R);
    ss << "(";
    for (auto i = 0; i < C; ++i)
        ss << (i > 0 ? ", " : "") << m[i];
    ss << ")";
    return out << ss.str();
}
} // namespace tg
