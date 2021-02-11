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
    auto [t0, t1] = inverse(mat<2, 2, ScalarT>::from_cols({1, d0d1}, {-d0d1, -1})) * vec2(b0, b1);
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


// =========== Other Implementations ===========

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> closest_point(quadric<3, ScalarT> const& q)
{
    // Returns a point minimizing this quadric
    // (Point is unique if any plane was added with sigma > 0)
    // Solving Ax = r with some common subexpressions precomputed

    auto a = q.A00;
    auto b = q.A01;
    auto c = q.A02;
    auto d = q.A11;
    auto e = q.A12;
    auto f = q.A22;
    auto r0 = q.b0;
    auto r1 = q.b1;
    auto r2 = q.b2;

    auto ad = a * d;
    auto ae = a * e;
    auto af = a * f;
    auto bc = b * c;
    auto be = b * e;
    auto bf = b * f;
    auto df = d * f;
    auto ce = c * e;
    auto cd = c * d;

    auto be_cd = be - cd;
    auto bc_ae = bc - ae;
    auto ce_bf = ce - bf;

    auto denom = 1 / (a * df + 2 * b * ce - ae * e - bf * b - cd * c);
    auto nom0 = r0 * (df - e * e) + r1 * ce_bf + r2 * be_cd;
    auto nom1 = r0 * ce_bf + r1 * (af - c * c) + r2 * bc_ae;
    auto nom2 = r0 * be_cd + r1 * bc_ae + r2 * (ad - b * b);

    return {nom0 * denom, nom1 * denom, nom2 * denom};
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
