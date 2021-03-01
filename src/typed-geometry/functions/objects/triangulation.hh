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

// normal_of(t) points outwards
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<triangle<3, ScalarT>, 12> triangulation_of(aabb<3, ScalarT, TraitsT> const& b)
{
    int idx = 0;
    array<triangle<3, ScalarT>, 12> r;

    auto dir = b.max - b.min;

    auto add_face = [&](pos<3, ScalarT> const& p, vec<3, ScalarT> const& d0, vec<3, ScalarT> const& d1) {
        auto const p00 = p;
        auto const p10 = p + d0;
        auto const p01 = p + d1;
        auto const p11 = p + d0 + d1;

        // TODO: orientation guarantees?
        r[idx++] = {p00, p01, p11};
        r[idx++] = {p00, p11, p10};
    };

    add_face(b.min, {dir.x, 0, 0}, {0, dir.y, 0});
    add_face(b.min, {0, dir.y, 0}, {0, 0, dir.z});
    add_face(b.min, {0, 0, dir.z}, {dir.x, 0, 0});

    add_face(b.max, {0, -dir.y, 0}, {-dir.x, 0, 0});
    add_face(b.max, {0, 0, -dir.z}, {0, -dir.y, 0});
    add_face(b.max, {-dir.x, 0, 0}, {0, 0, -dir.z});

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
