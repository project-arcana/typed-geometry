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
[[nodiscard]] constexpr array<segment<D, ScalarT>, 3> edges_of(triangle<D, ScalarT> const& t)
{
    return {{{t.pos0, t.pos1}, {t.pos1, t.pos2}, {t.pos2, t.pos0}}};
}

template <int D, class ScalarT>
[[nodiscard]] constexpr array<segment<D, ScalarT>, 4> edges_of(quad<D, ScalarT> const& q)
{
    return {{{q.pos00, q.pos10}, {q.pos10, q.pos11}, {q.pos11, q.pos01}, {q.pos01, q.pos00}}};
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<segment<2, ScalarT>, 4> edges_of(aabb<2, ScalarT, TraitsT> const& bb)
{
    const auto vs = vertices_of(bb);
    return {{{vs[0], vs[1]}, {vs[1], vs[2]}, {vs[2], vs[3]}, {vs[3], vs[0]}}};
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<segment<3, ScalarT>, 12> edges_of(aabb<3, ScalarT, TraitsT> const& bb)
{
    auto p000 = pos<3, ScalarT>(bb.min.x, bb.min.y, bb.min.z);
    auto p001 = pos<3, ScalarT>(bb.min.x, bb.min.y, bb.max.z);
    auto p010 = pos<3, ScalarT>(bb.min.x, bb.max.y, bb.min.z);
    auto p011 = pos<3, ScalarT>(bb.min.x, bb.max.y, bb.max.z);
    auto p100 = pos<3, ScalarT>(bb.max.x, bb.min.y, bb.min.z);
    auto p101 = pos<3, ScalarT>(bb.max.x, bb.min.y, bb.max.z);
    auto p110 = pos<3, ScalarT>(bb.max.x, bb.max.y, bb.min.z);
    auto p111 = pos<3, ScalarT>(bb.max.x, bb.max.y, bb.max.z);

    return {{
        {p000, p100},
        {p001, p101},
        {p010, p110},
        {p011, p111}, // x dir

        {p000, p010},
        {p001, p011},
        {p100, p110},
        {p101, p111}, // y dir

        {p000, p001},
        {p010, p011},
        {p100, p101},
        {p110, p111}, // z dir
    }};
}

template <class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr array<segment<DomainD, ScalarT>, 4> edges_of(box<2, ScalarT, DomainD, TraitsT> const& b)
{
    const auto vs = vertices_of(b);
    return {{{vs[0], vs[1]}, {vs[1], vs[2]}, {vs[2], vs[3]}, {vs[3], vs[0]}}};
}

template <class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr array<segment<DomainD, ScalarT>, 12> edges_of(box<3, ScalarT, DomainD, TraitsT> const& b)
{
    const auto vs = vertices_of(b);
    return {{
        // clang-format off
        {vs[0], vs[1]}, {vs[1], vs[2]}, {vs[2], vs[3]}, {vs[3], vs[0]}, // lower face
        {vs[4], vs[5]}, {vs[5], vs[6]}, {vs[6], vs[7]}, {vs[7], vs[4]}, // upper face
        {vs[0], vs[4]}, {vs[1], vs[5]}, {vs[2], vs[6]}, {vs[3], vs[7]}  // vertical edges
        // clang-format on
    }};
}

template <class BaseT, class TraitsT, typename = std::enable_if_t<!std::is_same_v<BaseT, sphere<2, typename BaseT::scalar_t, 3>>>>
[[nodiscard]] constexpr auto edges_of(pyramid<BaseT, TraitsT> const& py)
{
    using ScalarT = typename BaseT::scalar_t;
    const auto apex = apex_of(py);
    const auto edgesBase = edges_of(py.base);
    auto res = array<segment<3, ScalarT>, edgesBase.size() * 2>();
    for (size_t i = 0; i < edgesBase.size(); ++i)
    {
        res[i] = edgesBase[i];
        res[i + edgesBase.size()] = segment<3, ScalarT>(edgesBase[i].pos0, apex);
    }
    return res;
}


template <class ObjectT>
[[deprecated("use edges_of")]] [[nodiscard]] constexpr auto edges(ObjectT const& o) -> decltype(edges_of(o))
{
    return edges_of(o);
}

namespace detail
{
template <class T>
auto test_has_edges(int) -> decltype(void(edges_of(std::declval<T>())), std::true_type{});
template <class T>
std::false_type test_has_edges(char);
}

/// true if edges(obj) exists
template <class T>
constexpr bool has_edges_of = decltype(detail::test_has_edges<T>(0))::value;
}
