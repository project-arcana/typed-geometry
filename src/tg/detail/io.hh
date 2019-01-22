#pragma once

#include <sstream>
#include <string>

#include "../types/types.hh"

#include "traits.hh"

namespace tg
{
template <int D, class ScalarT, template <int, class> class Type>
std::string to_string(Type<D, ScalarT> const& v)
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
std::ostream& operator<<(std::ostream& out, pos<D, ScalarT> const& v)
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
std::ostream& operator<<(std::ostream& out, size<D, ScalarT> const& v)
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
std::ostream& operator<<(std::ostream& out, box<D, ScalarT> const& v)
{
    using T = vec<D, ScalarT>;
    out << type_name_prefix<T>;
    out << type_name<T>;
    out << type_name_suffix<T>;
    out << "(" << v.min << ", " << v.max << ")";
    return out;
}
} // namespace tg
