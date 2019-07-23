#pragma once

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
/*
 * sum_of_pos is a helper struct for guaranteeing syntactic soundness when dealing with sums of positions
 * it is the result of operator+(pos, pos)
 * it can only be added onto other sum_of_pos
 * to get back to a pos one needs to use operator/(sum_of_pos, ScalarT)
 */
template <int D, class ScalarT>
struct sum_of_pos
{
    using scalar_t = ScalarT;
    using div_t = decltype(ScalarT() / 0.0f);
    using pos_t = pos<D, ScalarT>;

    constexpr sum_of_pos() = default;
    /* implicit */ constexpr sum_of_pos(pos_t const& p) : accum(p) {}

    constexpr pos<D, div_t> operator/(div_t const& rhs) const { return accum / rhs; }
    constexpr pos<D, div_t> operator*(div_t const& rhs) const { return accum * rhs; }

    constexpr sum_of_pos operator+(sum_of_pos const& rhs) const { return accum + vec(rhs); }
    constexpr sum_of_pos operator+(pos_t const& rhs) const { return accum + vec(rhs); }

private:
    pos_t accum;
};
}
