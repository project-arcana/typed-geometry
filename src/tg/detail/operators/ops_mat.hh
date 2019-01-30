#pragma once

#include "../../types/mat.hh"

namespace tg
{
// mat * vec (of same dimension)
template <int C, class ScalarT>
constexpr vec<1, ScalarT> operator*(mat<C, 1, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v)};
}
template <int C, class ScalarT>
constexpr vec<2, ScalarT> operator*(mat<C, 2, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v), dot(m.row(1), v)};
}
template <int C, class ScalarT>
constexpr vec<3, ScalarT> operator*(mat<C, 3, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v), dot(m.row(1), v), dot(m.row(2), v)};
}
template <int C, class ScalarT>
constexpr vec<4, ScalarT> operator*(mat<C, 4, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v), dot(m.row(1), v), dot(m.row(2), v), dot(m.row(3), v)};
}

} // namespace tg
