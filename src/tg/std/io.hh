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

template <int D, class ScalarT>
std::ostream& operator<<(std::ostream& out, vec<D, ScalarT> const& v)
{
    using T = vec<D, ScalarT>;
    out << type_name_prefix<T>;
    out << type_name<T>;
    out << type_name_suffix<T>;
    out << "(";
    for (auto i = 0; i < D; ++i)
        out << (i > 0 ? ", " : "") << v[i];
    out << ")";
    return out;
}
template <int D, class ScalarT>
std::ostream& operator<<(std::ostream& out, pos<D, ScalarT> const& p)
{
    using T = vec<D, ScalarT>;
    out << type_name_prefix<T>;
    out << type_name<T>;
    out << type_name_suffix<T>;
    out << "(";
    for (auto i = 0; i < D; ++i)
        out << (i > 0 ? ", " : "") << p[i];
    out << ")";
    return out;
}
template <int D, class ScalarT>
std::ostream& operator<<(std::ostream& out, size<D, ScalarT> const& s)
{
    using T = vec<D, ScalarT>;
    out << type_name_prefix<T>;
    out << type_name<T>;
    out << type_name_suffix<T>;
    out << "(";
    for (auto i = 0; i < D; ++i)
        out << (i > 0 ? ", " : "") << s[i];
    out << ")";
    return out;
}

template <int D, class ScalarT>
std::ostream& operator<<(std::ostream& out, box<D, ScalarT> const& b)
{
    using T = box<D, ScalarT>;
    out << type_name_prefix<T>;
    out << type_name<T>;
    out << type_name_suffix<T>;
    out << "(" << b.min << ", " << b.max << ")";
    return out;
}

template <int D, class ScalarT>
std::ostream& operator<<(std::ostream& out, triangle<D, ScalarT> const& t)
{
    using T = triangle<D, ScalarT>;
    out << type_name_prefix<T>;
    out << type_name<T>;
    out << type_name_suffix<T>;
    out << "(" << t.v0 << ", " << t.v1 << ", " << t.v2 << ")";
    return out;
}

template <int C, int R, class ScalarT>
std::ostream& operator<<(std::ostream& out, mat<C, R, ScalarT> const& m)
{
    using T = mat<C, R, ScalarT>;
    out << type_name_prefix<T>;
    out << type_name<T>;
    out << type_name_suffix<T>;
    out << "(";
    for (auto i = 0; i < C; ++i)
        out << (i > 0 ? ", " : "") << m[i];
    out << ")";
    return out;
}
} // namespace tg
