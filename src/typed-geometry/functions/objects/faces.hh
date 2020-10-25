#pragma once

#include <typed-geometry/types/array.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/pyramid.hh>

namespace tg
{
template <class ScalarT, class TraitsT>
[[nodiscard]] array<box<2, ScalarT, 3>, 6> faces_of(aabb<3, ScalarT, TraitsT> const& b)
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
[[nodiscard]] array<box<2, ScalarT, 3>, 6> faces_of(box<3, ScalarT, 3, TraitsT> const& b)
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

template <class BaseT>
[[nodiscard]] auto faces_of(pyramid_boundary_no_caps<BaseT> const& py)
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
}
