#pragma once

#include "../array.hh"
#include "../pos.hh"
#include "../scalar.hh"
#include "../vec.hh"

namespace tg
{
template <int D, class ScalarT>
struct triangle;

// Common triangle types

using triangle2 = triangle<2, f32>;
using triangle3 = triangle<3, f32>;
using triangle4 = triangle<4, f32>;

using ftriangle2 = triangle<2, f32>;
using ftriangle3 = triangle<3, f32>;
using ftriangle4 = triangle<4, f32>;

using dtriangle2 = triangle<2, f64>;
using dtriangle3 = triangle<3, f64>;
using dtriangle4 = triangle<4, f64>;

using itriangle2 = triangle<2, i32>;
using itriangle3 = triangle<3, i32>;
using itriangle4 = triangle<4, i32>;

using utriangle2 = triangle<2, u32>;
using utriangle3 = triangle<3, u32>;
using utriangle4 = triangle<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct triangle
{
    using vec_t = vec<D, ScalarT>;
    using pos_t = pos<D, ScalarT>;

    pos_t pos0;
    pos_t pos1;
    pos_t pos2;

    constexpr triangle() = default;
    constexpr triangle(pos_t p0, pos_t p1, pos_t p2) : pos0(p0), pos1(p1), pos2(p2) {}
    constexpr triangle(array<pos_t, 3> const& v) : pos0(v[0]), pos1(v[1]), pos2(v[2]) {}
};
} // namespace tg
