#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../array.hh"
#include "../pos.hh"
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

    template <class Range, class = std::void_t<decltype(pos_t(tg::begin(std::declval<Range>())))>>
    explicit constexpr triangle(Range&& r)
    {
        auto it = tg::begin(r);
        auto end = tg::end(r);
        TG_CONTRACT(it != end);
        pos0 = pos_t(*it++);
        TG_CONTRACT(it != end);
        pos1 = pos_t(*it++);
        TG_CONTRACT(it != end);
        pos2 = pos_t(*it++);
        TG_CONTRACT(!(it != end));
    }

    [[nodiscard]] constexpr pos_t operator[](comp<3, ScalarT> const& barycoords) const;
    [[nodiscard]] constexpr pos_t operator[](comp<2, ScalarT> const& barycoords) const;

    [[nodiscard]] bool operator==(triangle const& rhs) const { return pos0 == rhs.pos0 && pos1 == rhs.pos1 && pos2 == rhs.pos2; }
    [[nodiscard]] bool operator!=(triangle const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT>
constexpr void introspect(I&& i, triangle<D, ScalarT>& v)
{
    i(v.pos0, "pos0");
    i(v.pos1, "pos1");
    i(v.pos2, "pos2");
}
} // namespace tg
