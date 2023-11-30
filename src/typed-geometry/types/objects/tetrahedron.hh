#pragma once

#include <typed-geometry/types/objects/traits.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <int D, class ScalarT, class TraitsT = default_object_tag>
struct tetrahedron;

// Common tetrahedron types

using tetrahedron2 = tetrahedron<2, float>;
using tetrahedron3 = tetrahedron<3, float>;
using tetrahedron4 = tetrahedron<4, float>;

using ftetrahedron2 = tetrahedron<2, float>;
using ftetrahedron3 = tetrahedron<3, float>;
using ftetrahedron4 = tetrahedron<4, float>;

using dtetrahedron2 = tetrahedron<2, double>;
using dtetrahedron3 = tetrahedron<3, double>;
using dtetrahedron4 = tetrahedron<4, double>;

using itetrahedron2 = tetrahedron<2, int32_t>;
using itetrahedron3 = tetrahedron<3, int32_t>;
using itetrahedron4 = tetrahedron<4, int32_t>;

using utetrahedron2 = tetrahedron<2, uint32_t>;
using utetrahedron3 = tetrahedron<3, uint32_t>;
using utetrahedron4 = tetrahedron<4, uint32_t>;

// boundary
template <int D, class ScalarT>
using tetrahedron_boundary = tetrahedron<D, ScalarT, boundary_tag>;

// ======== IMPLEMENTATION ========

template <int D, class ScalarT, class TraitsT>
struct tetrahedron
{
    using pos_t = pos<D, ScalarT>;
    using vec_t = vec<D, ScalarT>;

    pos_t pos0;
    pos_t pos1;
    pos_t pos2;
    pos_t pos3;

    constexpr tetrahedron() = default;
    constexpr tetrahedron(pos_t p0, pos_t p1, pos_t p2, pos_t p3) : pos0(p0), pos1(p1), pos2(p2), pos3(p3) {}

    template <class OtherScalarT, class OtherTraitsT>
    explicit constexpr tetrahedron(tetrahedron<D, OtherScalarT, OtherTraitsT> const& v) : pos0(v.pos0), pos1(v.pos1), pos2(v.pos2), pos3(v.pos3)
    {
    }

    template <class Range, class = std::enable_if_t<tg::is_range<Range, pos_t>>>
    explicit constexpr tetrahedron(Range&& r)
    {
        auto it = tg::begin(r);
        auto end = tg::end(r);
        TG_CONTRACT(it != end);
        pos0 = pos_t(*it++);
        TG_CONTRACT(it != end);
        pos1 = pos_t(*it++);
        TG_CONTRACT(it != end);
        pos2 = pos_t(*it++);
        TG_CONTRACT((it != end));
        pos3 = pos_t(*it++);
        TG_CONTRACT(!(it != end));
    }

    [[nodiscard]] constexpr pos_t operator[](comp<4, ScalarT> const& barycoords) const;
    [[nodiscard]] constexpr pos_t operator[](comp<3, ScalarT> const& barycoords) const;

    [[nodiscard]] bool operator==(tetrahedron const& rhs) const
    {
        return pos0 == rhs.pos0 && pos1 == rhs.pos1 && pos2 == rhs.pos2 && pos3 == rhs.pos3;
    }
    [[nodiscard]] bool operator!=(tetrahedron const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, tetrahedron<D, ScalarT, TraitsT>& v)
{
    i(v.pos0, "pos0");
    i(v.pos1, "pos1");
    i(v.pos2, "pos2");
    i(v.pos3, "pos3");
}

// TODO: object_traits?

}
