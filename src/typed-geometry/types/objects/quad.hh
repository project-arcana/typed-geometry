#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../array.hh"
#include "../pos.hh"
#include "../vec.hh"

namespace tg
{
template <int D, class ScalarT>
struct quad;

// Common quad types

using quad2 = quad<2, f32>;
using quad3 = quad<3, f32>;
using quad4 = quad<4, f32>;

using fquad2 = quad<2, f32>;
using fquad3 = quad<3, f32>;
using fquad4 = quad<4, f32>;

using dquad2 = quad<2, f64>;
using dquad3 = quad<3, f64>;
using dquad4 = quad<4, f64>;

using iquad2 = quad<2, i32>;
using iquad3 = quad<3, i32>;
using iquad4 = quad<4, i32>;

using uquad2 = quad<2, u32>;
using uquad3 = quad<3, u32>;
using uquad4 = quad<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct quad
{
    using vec_t = vec<D, ScalarT>;
    using pos_t = pos<D, ScalarT>;

    pos_t pos00;
    pos_t pos10;
    pos_t pos11;
    pos_t pos01;

    constexpr quad() = default;
    constexpr quad(pos_t p00, pos_t p10, pos_t p11, pos_t p01) : pos00(p00), pos10(p10), pos11(p11), pos01(p01) {}

    template <class Range, class = std::void_t<decltype(pos_t(tg::begin(std::declval<Range>())))>>
    explicit constexpr quad(Range&& r)
    {
        auto it = tg::begin(r);
        auto end = tg::end(r);
        TG_CONTRACT(it != end);
        pos00 = pos_t(*it++);
        TG_CONTRACT(it != end);
        pos10 = pos_t(*it++);
        TG_CONTRACT(it != end);
        pos11 = pos_t(*it++);
        TG_CONTRACT(it != end);
        pos01 = pos_t(*it++);
        TG_CONTRACT(!(it != end));
    }

    [[nodiscard]] constexpr pos_t operator[](comp<2, ScalarT> const& barycoords) const;

    [[nodiscard]] bool operator==(quad const& rhs) const { return pos00 == rhs.pos00 && pos10 == rhs.pos10 && pos11 == rhs.pos11 && pos01 == rhs.pos01; }
    [[nodiscard]] bool operator!=(quad const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
