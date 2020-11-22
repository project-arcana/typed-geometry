#pragma once

#include <typed-geometry/functions/matrix/inverse.hh>
#include <typed-geometry/functions/objects/distance.hh>
#include <typed-geometry/functions/objects/normal.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <class ScalarT>
[[nodiscard]] sphere<2, ScalarT> circumcircle_of(triangle<2, ScalarT> const& t)
{
    auto const& a = t.pos0;
    auto const& b = t.pos1;
    auto const& c = t.pos2;

    auto const d = ScalarT(2) * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    auto const a_len_sqr = distance_sqr_to_origin(a);
    auto const b_len_sqr = distance_sqr_to_origin(b);
    auto const c_len_sqr = distance_sqr_to_origin(c);
    auto const x = (a_len_sqr * (b.y - c.y) + b_len_sqr * (c.y - a.y) + c_len_sqr * (a.y - b.y)) / d;
    auto const y = (a_len_sqr * (c.x - b.x) + b_len_sqr * (a.x - c.x) + c_len_sqr * (b.x - a.x)) / d;

    auto const center = pos<2, ScalarT>(x, y);
    auto const r = distance(center, a);

    return {center, r};
}


template <class ObjectT>
[[deprecated("use circumcircle_of")]] [[nodiscard]] constexpr auto circumcircle(ObjectT const& o) -> decltype(circumcircle_of(o))
{
    return circumcircle_of(o);
}

/// given a 2D triangle, returns a 3x3 matrix M such that
/// M * (a, b, c) = (p, 1)
/// where a,b,c are barycentric coordinates for p
template <class ScalarT>
[[nodiscard]] mat<3, 3, ScalarT> from_barycoord_matrix_of(triangle<2, ScalarT> const& t)
{
    return mat<3, 3, ScalarT>::from_cols(vec<3, ScalarT>(t.pos0, 1.f), //
                                         vec<3, ScalarT>(t.pos1, 1.f), //
                                         vec<3, ScalarT>(t.pos2, 1.f));
}

/// given a 3D triangle, returns a 4x4 matrix M such that
/// M * (a, b, c, d) = (p + normal * d, 1)
/// where a,b,c are barycentric coordinates for p
/// and d is the signed distance in normal direction
template <class ScalarT>
[[nodiscard]] mat<4, 4, ScalarT> from_barycoord_matrix_of(triangle<3, ScalarT> const& t)
{
    auto const n = normal_of(t);
    return mat<4, 4, ScalarT>::from_cols(vec<4, ScalarT>(t.pos0, 1.f), //
                                         vec<4, ScalarT>(t.pos1, 1.f), //
                                         vec<4, ScalarT>(t.pos2, 1.f), //
                                         vec<4, ScalarT>(n, 0.f));
}

/// given a 2D triangle, returns a 3x3 matrix M such that
/// M * (p, 1) = (a, b, c)
/// where a,b,c are barycentric coordinates for p
template <class ScalarT>
[[nodiscard]] mat<3, 3, ScalarT> to_barycoord_matrix_of(triangle<2, ScalarT> const& t)
{
    return inverse(from_barycoord_matrix_of(t));
}

/// given a 3D triangle, returns a 4x4 matrix M such that
/// M * (p, 1) = (a, b, c, d)
/// where a,b,c are barycentric coordinates for p
/// and d is the signed distance in normal direction
template <class ScalarT>
[[nodiscard]] mat<4, 4, ScalarT> to_barycoord_matrix_of(triangle<3, ScalarT> const& t)
{
    return inverse(from_barycoord_matrix_of(t));
}

/// returns a transformation matrix M such that
/// M * p = p'
/// where p is a point in "from" and p' in "to" with the same barycentric coordinates
/// (i.e. it's the matrix transforming the first triangle into the second)
template <int D, class ScalarT>
[[nodiscard]] mat<D + 1, D + 1, ScalarT> transformation_from_to(triangle<D, ScalarT> const& from, triangle<D, ScalarT> const& to)
{
    return from_barycoord_matrix_of(to) * to_barycoord_matrix_of(from);
}
}
