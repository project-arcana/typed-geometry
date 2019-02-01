#pragma once
#include <tg/types/vec.hh>
#include <tg/types/mat.hh>

namespace tg
{
template <int D, class ScalarT>
constexpr tg::mat<D, D, ScalarT> outer_product(tg::vec<D, ScalarT> const &lhs, tg::vec<D, ScalarT> const &rhs)
{
    tg::mat<D, D, ScalarT> res;
    for (int i = 0; i < D; i++)
    {
        res[i] = lhs * rhs[i];
    }
    return res;
}
} // namespace tg
