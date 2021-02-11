#pragma once

#include <typed-geometry/types/array.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/pyramid.hh>
#include <typed-geometry/types/objects/quad.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr array<pos<D, ScalarT>, 2> vertices_of(segment<D, ScalarT> const& s)
{
    return {{s.pos0, s.pos1}};
}

template <int D, class ScalarT>
[[nodiscard]] constexpr array<pos<D, ScalarT>, 3> vertices_of(triangle<D, ScalarT> const& t)
{
    return {{t.pos0, t.pos1, t.pos2}};
}

template <int D, class ScalarT>
[[nodiscard]] constexpr array<pos<D, ScalarT>, 4> vertices_of(quad<D, ScalarT> const& q)
{
    return {{q.pos00, q.pos10, q.pos11, q.pos01}}; // in ccw order
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<pos<1, ScalarT>, 2> vertices_of(aabb<1, ScalarT, TraitsT> const& bb)
{
    return {{bb.min, bb.max}};
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<pos<2, ScalarT>, 4> vertices_of(aabb<2, ScalarT, TraitsT> const& bb)
{
    auto const p10 = pos<2, ScalarT>(bb.max.x, bb.min.y);
    auto const p01 = pos<2, ScalarT>(bb.min.x, bb.max.y);

    return {{bb.min, p10, bb.max, p01}}; // in ccw order
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<pos<3, ScalarT>, 8> vertices_of(aabb<3, ScalarT, TraitsT> const& bb)
{
    auto const p001 = pos<3, ScalarT>(bb.min.x, bb.min.y, bb.max.z);
    auto const p010 = pos<3, ScalarT>(bb.min.x, bb.max.y, bb.min.z);
    auto const p011 = pos<3, ScalarT>(bb.min.x, bb.max.y, bb.max.z);
    auto const p100 = pos<3, ScalarT>(bb.max.x, bb.min.y, bb.min.z);
    auto const p101 = pos<3, ScalarT>(bb.max.x, bb.min.y, bb.max.z);
    auto const p110 = pos<3, ScalarT>(bb.max.x, bb.max.y, bb.min.z);

    return {{bb.min, p001, p010, p011, p100, p101, p110, bb.max}};
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<pos<4, ScalarT>, 16> vertices_of(aabb<4, ScalarT, TraitsT> const& bb)
{
    auto const p0001 = pos<4, ScalarT>(bb.min.x, bb.min.y, bb.min.z, bb.max.w);
    auto const p0010 = pos<4, ScalarT>(bb.min.x, bb.min.y, bb.max.z, bb.min.w);
    auto const p0011 = pos<4, ScalarT>(bb.min.x, bb.min.y, bb.max.z, bb.max.w);
    auto const p0100 = pos<4, ScalarT>(bb.min.x, bb.max.y, bb.min.z, bb.min.w);
    auto const p0101 = pos<4, ScalarT>(bb.min.x, bb.max.y, bb.min.z, bb.max.w);
    auto const p0110 = pos<4, ScalarT>(bb.min.x, bb.max.y, bb.max.z, bb.min.w);
    auto const p0111 = pos<4, ScalarT>(bb.min.x, bb.max.y, bb.max.z, bb.max.w);
    auto const p1000 = pos<4, ScalarT>(bb.max.x, bb.min.y, bb.min.z, bb.min.w);
    auto const p1001 = pos<4, ScalarT>(bb.max.x, bb.min.y, bb.min.z, bb.max.w);
    auto const p1010 = pos<4, ScalarT>(bb.max.x, bb.min.y, bb.max.z, bb.min.w);
    auto const p1011 = pos<4, ScalarT>(bb.max.x, bb.min.y, bb.max.z, bb.max.w);
    auto const p1100 = pos<4, ScalarT>(bb.max.x, bb.max.y, bb.min.z, bb.min.w);
    auto const p1101 = pos<4, ScalarT>(bb.max.x, bb.max.y, bb.min.z, bb.max.w);
    auto const p1110 = pos<4, ScalarT>(bb.max.x, bb.max.y, bb.max.z, bb.min.w);

    return {{bb.min, p0001, p0010, p0011, p0100, p0101, p0110, p0111, p1001, p1010, p1011, p1100, p1101, p1110, bb.max}};
}

template <class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr array<pos<DomainD, ScalarT>, 2> vertices_of(box<1, ScalarT, DomainD, TraitsT> const& b)
{
    auto const p0 = b[comp<1, ScalarT>(-1)];
    auto const p1 = b[comp<1, ScalarT>(1)];
    return {{p0, p1}};
}

template <class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr array<pos<DomainD, ScalarT>, 4> vertices_of(box<2, ScalarT, DomainD, TraitsT> const& b)
{
    auto const p00 = b[comp<2, ScalarT>(-1, -1)];
    auto const p10 = b[comp<2, ScalarT>(1, -1)];
    auto const p11 = b[comp<2, ScalarT>(1, 1)];
    auto const p01 = b[comp<2, ScalarT>(-1, 1)];

    return {{p00, p10, p11, p01}}; // in ccw order
}

template <class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr array<pos<DomainD, ScalarT>, 8> vertices_of(box<3, ScalarT, DomainD, TraitsT> const& b)
{
    auto const p000 = b[comp<3, ScalarT>(-1, -1, -1)];
    auto const p100 = b[comp<3, ScalarT>(1, -1, -1)];
    auto const p110 = b[comp<3, ScalarT>(1, 1, -1)];
    auto const p010 = b[comp<3, ScalarT>(-1, 1, -1)];
    auto const p001 = b[comp<3, ScalarT>(-1, -1, 1)];
    auto const p101 = b[comp<3, ScalarT>(1, -1, 1)];
    auto const p111 = b[comp<3, ScalarT>(1, 1, 1)];
    auto const p011 = b[comp<3, ScalarT>(-1, 1, 1)];

    return {{p000, p100, p110, p010, p001, p101, p111, p011}};
}

template <class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr array<pos<DomainD, ScalarT>, 16> vertices_of(box<4, ScalarT, DomainD, TraitsT> const& b)
{
    auto const p0000 = b[comp<4, ScalarT>(-1, -1, -1, -1)];
    auto const p1000 = b[comp<4, ScalarT>(1, -1, -1, -1)];
    auto const p1100 = b[comp<4, ScalarT>(1, 1, -1, -1)];
    auto const p0100 = b[comp<4, ScalarT>(-1, 1, -1, -1)];
    auto const p0010 = b[comp<4, ScalarT>(-1, -1, 1, -1)];
    auto const p1010 = b[comp<4, ScalarT>(1, -1, 1, -1)];
    auto const p1110 = b[comp<4, ScalarT>(1, 1, 1, -1)];
    auto const p0110 = b[comp<4, ScalarT>(-1, 1, 1, -1)];

    auto const p0001 = b[comp<4, ScalarT>(-1, -1, -1, 1)];
    auto const p1001 = b[comp<4, ScalarT>(1, -1, -1, 1)];
    auto const p1101 = b[comp<4, ScalarT>(1, 1, -1, 1)];
    auto const p0101 = b[comp<4, ScalarT>(-1, 1, -1, 1)];
    auto const p0011 = b[comp<4, ScalarT>(-1, -1, 1, 1)];
    auto const p1011 = b[comp<4, ScalarT>(1, -1, 1, 1)];
    auto const p1111 = b[comp<4, ScalarT>(1, 1, 1, 1)];
    auto const p0111 = b[comp<4, ScalarT>(-1, 1, 1, 1)];

    return {{p0000, p1000, p1100, p0100, p0010, p1010, p1110, p0110, p0001, p1001, p1101, p0101, p0011, p1011, p1111, p0111}};
}

template <class BaseT, class TraitsT, typename = std::enable_if_t<!std::is_same_v<BaseT, sphere<2, typename BaseT::scalar_t, 3>>>>
[[nodiscard]] constexpr auto vertices_of(pyramid<BaseT, TraitsT> const& py)
{
    using ScalarT = typename BaseT::scalar_t;
    auto const vertsBase = vertices_of(py.base);
    auto res = array<pos<3, ScalarT>, vertsBase.size() + 1>();
    for (size_t i = 0; i < vertsBase.size(); ++i)
        res[i] = vertsBase[i];
    res[vertsBase.size()] = apex_of(py);
    return res;
}


template <class ObjectT>
[[deprecated("use vertices_of")]] [[nodiscard]] constexpr auto vertices(ObjectT const& o) -> decltype(vertices_of(o))
{
    return vertices_of(o);
}

namespace detail
{
template <class T>
auto test_has_vertices(int) -> decltype(void(vertices_of(std::declval<T>())), std::true_type{});
template <class T>
std::false_type test_has_vertices(char);
}

/// true if vertices_of(obj) exists
template <class T>
constexpr bool has_vertices_of = decltype(detail::test_has_vertices<T>(0))::value;
}
