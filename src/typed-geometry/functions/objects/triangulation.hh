#pragma once

#include <typed-geometry/functions/objects/faces.hh>

// triangulation_of(obj) returns a collection of triangles approximating the surface of obj
// sometimes, a second parameter can be provided for fine control

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr array<triangle<D, ScalarT>, 1> triangulation_of(triangle<D, ScalarT> const& t)
{
    return {{t}};
}

template <int D, class ScalarT>
[[nodiscard]] constexpr array<triangle<D, ScalarT>, 2> triangulation_of(quad<D, ScalarT> const& q)
{
    // TODO: orientation guarantees?
    return {{
        {q.pos00, q.pos10, q.pos11}, //
        {q.pos00, q.pos11, q.pos01},
    }};
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<triangle<D, ScalarT>, 2> triangulation_of(box<2, ScalarT, D, TraitsT> const& b)
{
    auto const p00 = b.center - b.half_extents[0] - b.half_extents[1];
    auto const p10 = b.center + b.half_extents[0] - b.half_extents[1];
    auto const p01 = b.center - b.half_extents[0] + b.half_extents[1];
    auto const p11 = b.center + b.half_extents[0] + b.half_extents[1];

    // TODO: orientation guarantees?
    return {{
        {p00, p10, p11}, //
        {p00, p11, p01},
    }};
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<triangle<3, ScalarT>, 12> triangulation_of(box<3, ScalarT, 3, TraitsT> const& b)
{
    int idx = 0;
    array<triangle<3, ScalarT>, 12> r;

    for (auto const& f : faces_of(b))
        for (auto const& t : triangulation_of(f))
            r[idx++] = t;

    return r;
}

/// normal_of(t) points outwards
/// guarantees that the result has 8 unique vertices (for non-empty aabbs)
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<triangle<3, ScalarT>, 12> triangulation_of(aabb<3, ScalarT, TraitsT> const& b)
{
    array<triangle<3, ScalarT>, 12> r;

    auto const v000 = pos<3, ScalarT>(b.min.x, b.min.y, b.min.z);
    auto const v001 = pos<3, ScalarT>(b.min.x, b.min.y, b.max.z);
    auto const v010 = pos<3, ScalarT>(b.min.x, b.max.y, b.min.z);
    auto const v011 = pos<3, ScalarT>(b.min.x, b.max.y, b.max.z);
    auto const v100 = pos<3, ScalarT>(b.max.x, b.min.y, b.min.z);
    auto const v101 = pos<3, ScalarT>(b.max.x, b.min.y, b.max.z);
    auto const v110 = pos<3, ScalarT>(b.max.x, b.max.y, b.min.z);
    auto const v111 = pos<3, ScalarT>(b.max.x, b.max.y, b.max.z);

    r[0] = {v000, v010, v110};
    r[1] = {v000, v110, v100};
    r[2] = {v000, v001, v011};
    r[3] = {v000, v011, v010};
    r[4] = {v000, v100, v101};
    r[5] = {v000, v101, v001};
    r[6] = {v111, v001, v101};
    r[7] = {v111, v011, v001};
    r[8] = {v111, v100, v110};
    r[9] = {v111, v101, v100};
    r[10] = {v111, v010, v011};
    r[11] = {v111, v110, v010};

    return r;
}

namespace detail
{
template <class T>
auto test_has_triangulation(int) -> decltype(void(triangulation_of(std::declval<T>())), std::true_type{});
template <class T>
std::false_type test_has_triangulation(char);
}

/// true if triangulation_of(obj) exists
template <class T>
constexpr bool has_triangulation_of = decltype(detail::test_has_triangulation<T>(0))::value;
}
