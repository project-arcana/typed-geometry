#pragma once

#include "../../types/mat.hh"
#include "../../types/vec.hh"

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr mat<1, 1, ScalarT> outer_product(vec<1, ScalarT> const& lhs, vec<1, ScalarT> const& rhs)
{
    mat<1, 1, ScalarT> res;
    res[0] = lhs * rhs[0];
    return res;
}
template <class ScalarT>
TG_NODISCARD constexpr mat<2, 2, ScalarT> outer_product(vec<2, ScalarT> const& lhs, vec<2, ScalarT> const& rhs)
{
    mat<2, 2, ScalarT> res;
    res[0] = lhs * rhs[0];
    res[1] = lhs * rhs[1];
    return res;
}
template <class ScalarT>
TG_NODISCARD constexpr mat<3, 3, ScalarT> outer_product(vec<3, ScalarT> const& lhs, vec<3, ScalarT> const& rhs)
{
    mat<3, 3, ScalarT> res;
    res[0] = lhs * rhs[0];
    res[1] = lhs * rhs[1];
    res[2] = lhs * rhs[2];
    return res;
}
template <class ScalarT>
TG_NODISCARD constexpr mat<4, 4, ScalarT> outer_product(vec<4, ScalarT> const& lhs, vec<4, ScalarT> const& rhs)
{
    mat<4, 4, ScalarT> res;
    res[0] = lhs * rhs[0];
    res[1] = lhs * rhs[1];
    res[2] = lhs * rhs[2];
    res[3] = lhs * rhs[3];
    return res;
}
} // namespace tg
