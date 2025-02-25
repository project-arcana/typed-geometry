#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/quadric.hh>

#include "coordinates.hh"
#include "project.hh"

// closest_points(a, b) returns points {p_a, p_b} such that contains(a, p_a) and contains(b, p_b) and |p_a - p_b| is minimal
// closest_points_parameters(a, b) return parameters {t_a, t_b} such that {a[t_a], b[t_a]} are the closest points

namespace tg
{
// Base case for point/point:
template <int D, class ScalarT>
[[nodiscard]] constexpr pair<pos<D, ScalarT>, pos<D, ScalarT>> closest_points(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return {a, b};
}

// Default implementation for point/object and object/point:
template <int D, class ScalarT, class ObjectT>
[[nodiscard]] constexpr auto closest_points(ObjectT const& o, pos<D, ScalarT> const& p) -> pair<decltype(project(p, o)), pos<D, ScalarT>>
{
    return {project(p, o), p};
}
template <int D, class ScalarT, class ObjectT>
[[nodiscard]] constexpr auto closest_points(pos<D, ScalarT> const& p, ObjectT const& o) -> pair<pos<D, ScalarT>, decltype(project(p, o))>
{
    return {p, project(p, o)};
}


// =========== Object Implementations ===========

template <class ScalarT>
[[nodiscard]] constexpr pair<ScalarT, ScalarT> closest_points_parameters(line<3, ScalarT> const& l0, line<3, ScalarT> const& l1)
{
    auto d0d1 = dot(l0.dir, l1.dir);
    auto b0 = dot(l1.pos - l0.pos, l0.dir);
    auto b1 = dot(l1.pos - l0.pos, l1.dir);
    auto [t0, t1] = inverse(mat<2, 2, ScalarT>::from_cols({ScalarT(1), d0d1}, {-d0d1, ScalarT(-1)})) * vec<2, ScalarT>(b0, b1);
    return {t0, t1};
}

template <class ScalarT>
[[nodiscard]] constexpr pair<pos<3, ScalarT>, pos<3, ScalarT>> closest_points(line<3, ScalarT> const& l0, line<3, ScalarT> const& l1)
{
    auto [t0, t1] = closest_points_parameters(l0, l1);
    return {l0[t0], l1[t1]};
}

template <class ScalarT>
[[nodiscard]] constexpr pair<ScalarT, ScalarT> closest_points_parameters(segment<3, ScalarT> const& s, line<3, ScalarT> const& l)
{
    auto ls = inf_of(s);
    auto len = length(s);

    auto [ts, tl] = closest_points_parameters(ls, l);
    auto tClamped = clamp(ts, ScalarT(0), len);
    return {tClamped / len, coordinates(l, ls[tClamped])};
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pair<pos<3, ScalarT>, pos<3, ScalarT>> closest_points(aabb<D, ScalarT> const& bb0, aabb<D, ScalarT> const& bb1)
{
    auto p1 = project(bb0.min, bb1);
    auto p0 = project(p1, bb0);
    return {p0, p1};
}


// =========== Other Implementations ===========

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> closest_point(quadric<3, ScalarT> const& q)
{
    // Returns a point minimizing this quadric
    // Solving Ax = r with using an unrolled https://en.wikipedia.org/wiki/Cholesky_decomposition
    // Thanks to @jdumas and @sarah-ek for this optimized implementation!
    using std::fma;

    auto a00 = A00;
    auto a10 = A01;
    auto a20 = A02;
    auto a11 = A11;
    auto a21 = A12;
    auto a22 = A22;
    auto x0 = b0;
    auto x1 = b1;
    auto x2 = b2;

    auto d0 = ScalarT(1.0) / a00;
    auto l10 = a10 * -d0;
    auto l20 = a20 * -d0;

    a11 = fma(a10, l10, a11);
    a21 = fma(a20, l10, a21);
    a22 = fma(a20, l20, a22);

    auto d1 = ScalarT(1.0) / a11;
    auto l21 = a21 * -d1;
    a22 = fma(a21, l21, a22);

    auto d2 = ScalarT(1.0) / a22;

    x1 = fma(l10, x0, x1);
    x2 = fma(l20, x0, x2);
    x2 = fma(l21, x1, x2);

    x0 *= d0;
    x1 *= d1;
    x2 *= d2;

    x0 = fma(l20, x2, x0);
    x1 = fma(l21, x2, x1);
    x0 = fma(l10, x1, x0);

    return {x0, x1, x2};
}
template <class ScalarT>
[[nodiscard]] constexpr pos<2, ScalarT> closest_point(quadric<2, ScalarT> const& q)
{
    // Returns a point minimizing this quadric
    // (Point is unique if any plane was added with sigma > 0)
    // Solving Ax = r with some common subexpressions precomputed

    auto a = q.A00;
    auto b = q.A01;
    auto c = q.A11;

    auto denom = 1 / (a * c - b * b);
    auto nom0 = q.b0 * c - q.b1 * b;
    auto nom1 = q.b1 * a - q.b0 * b;

    return {nom0 * denom, nom1 * denom};
}

} // namespace tg
