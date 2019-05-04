#pragma once

#include "../angle.hh"
#include "../dir.hh"
#include "../pos.hh"
#include "../scalar.hh"

namespace tg
{
template <int D, class ScalarT>
struct square;

// Common square types

using square2 = square<2, f32>;
using square3 = square<3, f32>;

using fsquare2 = square<2, f32>;
using fsquare3 = square<3, f32>;

using dsquare2 = square<2, f64>;
using dsquare3 = square<3, f64>;

using isquare2 = square<2, i32>;
using isquare3 = square<3, i32>;

using usquare2 = square<2, u32>;
using usquare3 = square<3, u32>;

// ======== IMPLEMENTATION ========
template <class ScalarT>
struct square<2, ScalarT>
{
    using pos_t = pos<3, ScalarT>;

    pos_t center;
    ScalarT length;
    angle<ScalarT> rotation; // about z-axis

    constexpr square() = default;
    constexpr square(pos_t c, ScalarT l, angle<ScalarT> r) : center(c), length(l), rotation(r){};
};

template <class ScalarT>
struct square<3, ScalarT>
{
    using dir_t = dir<3, ScalarT>;
    using pos_t = pos<3, ScalarT>;

    pos_t center;
    ScalarT length;
    dir_t normal;

    angle<ScalarT> rotation; // about axis defined by normal

    constexpr square() = default;
    constexpr square(pos_t c, ScalarT l, dir_t n = dir_t::pos_y, angle<ScalarT> r = 0) : center(c), length(l), normal(n), rotation(r) {}
};
} // namespace tg
