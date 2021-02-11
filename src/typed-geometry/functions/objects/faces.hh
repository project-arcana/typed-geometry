#pragma once

#include <typed-geometry/types/array.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/pyramid.hh>

#include "vertices.hh"

namespace tg
{
// ======== pyramid faces struct ========
template <class BaseT, u64 N>
struct pyramid_faces
{
    using scalar_t = typename BaseT::scalar_t;
    using mantle_t = array<triangle<3, scalar_t>, N>;

    BaseT base;
    mantle_t mantle;

    constexpr pyramid_faces() = default;
    constexpr pyramid_faces(BaseT const& base, mantle_t const& mantle) : base(base), mantle(mantle) {}
};

// ======== faces_of implementation ========

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<box<2, ScalarT, 3>, 6> faces_of(aabb<3, ScalarT, TraitsT> const& b)
{
    static_assert(is_floating_point<ScalarT>, "cannot be guaranteed for integers");
    using face_t = box<2, ScalarT, 3>;
    using mat_t = typename face_t::mat_t;
    using vec_t = typename face_t::vec_t;

    const auto diag = (b.max - b.min) * ScalarT(0.5);
    const auto center = centroid_of(b);
    const auto faceX0 = face_t(center - vec_t(diag.x, 0, 0), mat_t(vec_t(0, 0, diag.z), vec_t(0, diag.y, 0)));
    const auto faceX1 = face_t(center + vec_t(diag.x, 0, 0), mat_t(vec_t(0, diag.y, 0), vec_t(0, 0, diag.z)));
    const auto faceY0 = face_t(center - vec_t(0, diag.y, 0), mat_t(vec_t(diag.x, 0, 0), vec_t(0, 0, diag.z)));
    const auto faceY1 = face_t(center + vec_t(0, diag.y, 0), mat_t(vec_t(0, 0, diag.z), vec_t(diag.x, 0, 0)));
    const auto faceZ0 = face_t(center - vec_t(0, 0, diag.z), mat_t(vec_t(0, diag.y, 0), vec_t(diag.x, 0, 0)));
    const auto faceZ1 = face_t(center + vec_t(0, 0, diag.z), mat_t(vec_t(diag.x, 0, 0), vec_t(0, diag.y, 0)));

    return {{faceX0, faceX1, faceY0, faceY1, faceZ0, faceZ1}};
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr array<box<2, ScalarT, 3>, 6> faces_of(box<3, ScalarT, 3, TraitsT> const& b)
{
    using face_t = box<2, ScalarT, 3>;
    using mat_t = typename face_t::mat_t;

    const auto he = b.half_extents;
    const auto faceX0 = face_t(b.center - he[0], mat_t(he[2], he[1]));
    const auto faceX1 = face_t(b.center + he[0], mat_t(he[1], he[2]));
    const auto faceY0 = face_t(b.center - he[1], mat_t(he[0], he[2]));
    const auto faceY1 = face_t(b.center + he[1], mat_t(he[2], he[0]));
    const auto faceZ0 = face_t(b.center - he[2], mat_t(he[1], he[0]));
    const auto faceZ1 = face_t(b.center + he[2], mat_t(he[0], he[1]));

    return {{faceX0, faceX1, faceY0, faceY1, faceZ0, faceZ1}};
}

template <class BaseT, typename = std::enable_if_t<!std::is_same_v<BaseT, sphere<2, typename BaseT::scalar_t, 3>>>>
[[nodiscard]] constexpr auto faces_of(pyramid<BaseT, boundary_no_caps_tag> const& py)
{
    using ScalarT = typename BaseT::scalar_t;
    static_assert(is_floating_point<ScalarT>, "cannot be guaranteed for integers");

    const auto apex = apex_of(py);
    const auto verts = vertices_of(py.base);
    auto triangles = array<triangle<3, ScalarT>, verts.size()>();
    for (size_t i = 0; i < verts.size(); ++i)
        triangles[i] = {apex, verts[i], verts[(i + 1) % verts.size()]};
    return triangles;
}

template <class BaseT, class TraitsT, typename = std::enable_if_t<!std::is_same_v<BaseT, sphere<2, typename BaseT::scalar_t, 3>>>>
[[nodiscard]] constexpr auto faces_of(pyramid<BaseT, TraitsT> const& py)
{
    auto mantle = faces_of(boundary_no_caps_of(py));
    return pyramid_faces<BaseT, mantle.size()>(py.base, mantle);
}

namespace detail
{
template <class T>
auto test_has_faces(int) -> decltype(void(faces_of(std::declval<T>())), std::true_type{});
template <class T>
std::false_type test_has_faces(char);
}

/// true if faces_of(obj) exists
template <class T>
constexpr bool has_faces_of = decltype(detail::test_has_faces<T>(0))::value;
}
