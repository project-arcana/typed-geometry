#pragma once

#include <typed-geometry/functions/matrix/inverse.hh>
#include <typed-geometry/functions/objects/distance.hh>
#include <typed-geometry/functions/objects/normal.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] pos<D, ScalarT> circumcenter_of(triangle<D, ScalarT> const& t)
{
    auto const a2 = distance_sqr(t.pos0, t.pos1);
    auto const b2 = distance_sqr(t.pos1, t.pos2);
    auto const c2 = distance_sqr(t.pos2, t.pos0);

    auto A = a2 * (b2 + c2 - a2);
    auto B = b2 * (c2 + a2 - b2);
    auto C = c2 * (a2 + b2 - c2);
    auto const f = ScalarT(1) / (A + B + C);
    A *= f;
    B *= f;
    C *= f;
    return t.pos0 * B + t.pos1 * C + t.pos2 * A;
}

template <int D, class ScalarT>
[[nodiscard]] ScalarT circumradius_of(triangle<D, ScalarT> const& t)
{
    auto const a2 = distance_sqr(t.pos0, t.pos1);
    auto const b2 = distance_sqr(t.pos1, t.pos2);
    auto const c2 = distance_sqr(t.pos2, t.pos0);
    return sqrt(a2 * b2 * c2) / (4 * area_of(t));
}

template <int D, class ScalarT>
[[nodiscard]] sphere<2, ScalarT, D> circumcircle_of(triangle<D, ScalarT> const& t)
{
    auto const center = circumcenter_of(t);
    auto const r = distance(center, t.pos0);

    if constexpr (D == 2)
        return {center, r};
    else
        return {center, r, normal_of(t)};
}

template <class ObjectT>
[[deprecated("use circumcircle_of")]] [[nodiscard]] constexpr auto circumcircle(ObjectT const& o) -> decltype(circumcircle_of(o))
{
    return circumcircle_of(o);
}

template <int D, class ScalarT>
[[nodiscard]] pos<D, ScalarT> incenter_of(triangle<D, ScalarT> const& t)
{
    auto a = distance(t.pos0, t.pos1);
    auto b = distance(t.pos1, t.pos2);
    auto c = distance(t.pos2, t.pos0);
    auto const f = ScalarT(1) / (a + b + c);
    a *= f;
    b *= f;
    c *= f;
    return t.pos0 * b + t.pos1 * c + t.pos2 * a;
}

template <int D, class ScalarT>
[[nodiscard]] ScalarT inradius_of(triangle<D, ScalarT> const& t)
{
    auto const a = distance(t.pos0, t.pos1);
    auto const b = distance(t.pos1, t.pos2);
    auto const c = distance(t.pos2, t.pos0);
    auto const s = (a + b + c) / 2;
    return sqrt(max(ScalarT(0), (s - a) * (s - b) * (s - c) / s));
}

template <int D, class ScalarT>
[[nodiscard]] sphere<2, ScalarT, D> incircle_of(triangle<D, ScalarT> const& t)
{
    auto const center = incenter_of(t);
    auto const r = inradius_of(t);

    if constexpr (D == 2)
        return {center, r};
    else
        return {center, r, normal_of(t)};
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

/// returns the minimum altitude/height of a triangle
template <int D, class ScalarT>
[[nodiscard]] ScalarT min_height_of(triangle<D, ScalarT> const& t)
{
    auto const area = area_of(t);
    return area / tg::max(distance(t.pos0, t.pos1), distance(t.pos1, t.pos2), distance(t.pos2, t.pos0));
}

/// returns the maximum altitude/height of a triangle
template <int D, class ScalarT>
[[nodiscard]] ScalarT max_height_of(triangle<D, ScalarT> const& t)
{
    auto const area = area_of(t);
    return area / tg::min(distance(t.pos0, t.pos1), distance(t.pos1, t.pos2), distance(t.pos2, t.pos0));
}
}
