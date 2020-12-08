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
[[nodiscard]] constexpr array<pos<2, ScalarT>, 4> vertices_of(aabb<2, ScalarT, TraitsT> const& bb)
{
    auto p00 = pos<3, ScalarT>(bb.min.x, bb.min.y);
    auto p10 = pos<3, ScalarT>(bb.max.x, bb.min.y);
    auto p11 = pos<3, ScalarT>(bb.max.x, bb.max.y);
    auto p01 = pos<3, ScalarT>(bb.min.x, bb.max.y);

    return {{p00, p10, p11, p01}}; // in ccw order
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<pos<3, ScalarT>, 8> vertices_of(aabb<3, ScalarT, TraitsT> const& bb)
{
    auto p000 = pos<3, ScalarT>(bb.min.x, bb.min.y, bb.min.z);
    auto p001 = pos<3, ScalarT>(bb.min.x, bb.min.y, bb.max.z);
    auto p010 = pos<3, ScalarT>(bb.min.x, bb.max.y, bb.min.z);
    auto p011 = pos<3, ScalarT>(bb.min.x, bb.max.y, bb.max.z);
    auto p100 = pos<3, ScalarT>(bb.max.x, bb.min.y, bb.min.z);
    auto p101 = pos<3, ScalarT>(bb.max.x, bb.min.y, bb.max.z);
    auto p110 = pos<3, ScalarT>(bb.max.x, bb.max.y, bb.min.z);
    auto p111 = pos<3, ScalarT>(bb.max.x, bb.max.y, bb.max.z);

    return {{p000, p001, p010, p011, p100, p101, p110, p111}};
}

template <class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr array<pos<DomainD, ScalarT>, 4> vertices_of(box<2, ScalarT, DomainD, TraitsT> const& b)
{
    const auto p00 = b[comp<2, ScalarT>(-1, -1)];
    const auto p10 = b[comp<2, ScalarT>(1, -1)];
    const auto p11 = b[comp<2, ScalarT>(1, 1)];
    const auto p01 = b[comp<2, ScalarT>(-1, 1)];

    return {{p00, p10, p11, p01}}; // in ccw order
}

template <class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr array<pos<DomainD, ScalarT>, 8> vertices_of(box<3, ScalarT, DomainD, TraitsT> const& b)
{
    const auto p000 = b[comp<3, ScalarT>(-1, -1, -1)];
    const auto p100 = b[comp<3, ScalarT>(1, -1, -1)];
    const auto p110 = b[comp<3, ScalarT>(1, 1, -1)];
    const auto p010 = b[comp<3, ScalarT>(-1, 1, -1)];
    const auto p001 = b[comp<3, ScalarT>(-1, -1, 1)];
    const auto p101 = b[comp<3, ScalarT>(1, -1, 1)];
    const auto p111 = b[comp<3, ScalarT>(1, 1, 1)];
    const auto p011 = b[comp<3, ScalarT>(-1, 1, 1)];

    return {{p000, p100, p110, p010, p001, p101, p111, p011}};
}

template <class BaseT, class TraitsT>
[[nodiscard]] constexpr auto vertices_of(pyramid<BaseT, TraitsT> const& py)
{
    using ScalarT = typename BaseT::scalar_t;
    static_assert(!std::is_same_v<BaseT, sphere<2, ScalarT, 3>>, "not possible for cones");

    const auto vertsBase = vertices_of(py.base);
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
