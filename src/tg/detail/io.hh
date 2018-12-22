#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include "../types/types.hh"

#include "traits.hh"

namespace tg
{
template <int D, class ScalarT>
std::string to_string(vec<D, ScalarT> const& v)
{
    std::ostringstream ss;
    ss << v;
    return ss.str();
}

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
} // namespace tg