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

#include <typed-geometry/functions/basic/mix.hh>

// returns the arithmetic mean of all points contained in an object
// has variadic versions

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> centroid(pos<D, ScalarT> const& p)
{
    return p;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> centroid(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return mix(a, b, ScalarT(0.5));
}

template <int ObjectD, class ScalarT, int DomainD>
[[nodiscard]] constexpr pos<DomainD, ScalarT> centroid(box<ObjectD, ScalarT, DomainD> const& b)
{
    return b.center;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid(aabb<D, ScalarT> const& p)
{
    return mix(p.min, p.max, ScalarT(0.5));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid(segment<D, ScalarT> const& p)
{
    return mix(p.pos0, p.pos1, fractional_result<ScalarT>(0.5));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid(triangle<D, ScalarT> const& p)
{
    return (p.pos0 + p.pos1 + p.pos2) / ScalarT(3);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid(quad<D, ScalarT> const& q)
{
    return (q.pos00 + q.pos01 + q.pos10 + q.pos11) * ScalarT(0.25);
}

template <int ObjectD, class ScalarT, int DomainD>
[[nodiscard]] constexpr pos<DomainD, ScalarT> centroid(sphere<ObjectD, ScalarT, DomainD> const& p)
{
    return p.center;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid(cylinder<D, ScalarT> const& c)
{
    return centroid(c.axis);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid(capsule<D, ScalarT> const& c)
{
    return centroid(c.axis);
}
} // namespace tg
