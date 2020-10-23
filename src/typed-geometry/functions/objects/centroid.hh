#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/quad.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include <typed-geometry/detail/operators/ops_pos.hh>
#include <typed-geometry/detail/operators/ops_vec.hh>

// returns the arithmetic mean of all points contained in an object
// has variadic versions

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> centroid_of(pos<D, ScalarT> const& p)
{
    return p;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid_of(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return (a + b) * fractional_result<ScalarT>(0.5);
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr pos<DomainD, ScalarT> centroid_of(box<ObjectD, ScalarT, DomainD, TraitsT> const& b)
{
    return b.center;
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid_of(aabb<D, ScalarT, TraitsT> const& b)
{
    return (b.min + b.max) * fractional_result<ScalarT>(0.5);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid_of(segment<D, ScalarT> const& s)
{
    return (s.pos0 + s.pos1) * fractional_result<ScalarT>(0.5);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid_of(triangle<D, ScalarT> const& t)
{
    return (t.pos0 + t.pos1 + t.pos2) / ScalarT(3);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid_of(quad<D, ScalarT> const& q)
{
    // TODO: The average of the four corners is not the same as the average of all points in the quad
    return (q.pos00 + q.pos01 + q.pos10 + q.pos11) * fractional_result<ScalarT>(0.25);
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr pos<DomainD, ScalarT> centroid_of(sphere<ObjectD, ScalarT, DomainD, TraitsT> const& p)
{
    return p.center;
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid_of(cylinder<D, ScalarT, TraitsT> const& c)
{
    return centroid_of(c.axis);
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid_of(capsule<D, ScalarT, TraitsT> const& c)
{
    return centroid_of(c.axis);
}


template <class ObjectT>
[[deprecated("use centroid_of")]] [[nodiscard]] constexpr auto centroid(ObjectT const& o) -> decltype(centroid_of(o))
{
    return centroid_of(o);
}
} // namespace tg
