#pragma once

#include <array>

#include "pos.hh"
#include "scalar.hh"
#include "vec.hh"

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

    pos_t v0;
    pos_t v1;
    pos_t v2;

    constexpr triangle() = default;
    constexpr triangle(pos_t v0, pos_t v1, pos_t v2) : v0(v0), v1(v1), v2(v2) {}
    constexpr triangle(std::array<pos_t, 3> const& v) : v0(v[0]), v1(v[1]), v2(v[2]) {}
};
} // namespace tg
