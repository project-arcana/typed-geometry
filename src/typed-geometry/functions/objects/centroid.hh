#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/ellipse.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
#include <typed-geometry/types/objects/pyramid.hh>
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
    return (t.pos0 + t.pos1 + t.pos2) / fractional_result<ScalarT>(3);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid_of(quad<D, ScalarT> const& q)
{
    const auto c1 = centroid_of(q.pos00, q.pos11);
    const auto c2 = centroid_of(q.pos01, q.pos10);
    return centroid_of(c1, c2);
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr pos<DomainD, ScalarT> centroid_of(sphere<ObjectD, ScalarT, DomainD, TraitsT> const& s)
{
    return s.center;
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr pos<DomainD, ScalarT> centroid_of(ellipse<ObjectD, ScalarT, DomainD, TraitsT> const& e)
{
    return e.center;
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<D, fractional_result<ScalarT>> centroid_of(hemisphere<D, ScalarT, TraitsT> const& h)
{
    using frac_t = fractional_result<ScalarT>;
    frac_t offset;
    if constexpr (D == 2)
    {
        if constexpr (std::is_same_v<TraitsT, default_object_tag>)
            offset = frac_t(4) / (frac_t(3) * tg::pi_scalar<frac_t>);
        else if constexpr (std::is_same_v<TraitsT, boundary_tag>)
            offset = frac_t(2) / (frac_t(2) + tg::pi_scalar<frac_t>);
        else // boundary_no_caps_tag
            offset = frac_t(2) / tg::pi_scalar<frac_t>;
    }
    else // D == 3
    {
        if constexpr (std::is_same_v<TraitsT, default_object_tag>)
            offset = frac_t(3) / frac_t(8);
        else if constexpr (std::is_same_v<TraitsT, boundary_tag>)
            offset = frac_t(1) / frac_t(3);
        else // boundary_no_caps_tag
            offset = frac_t(0.5);
    }
    return h.center + offset * h.radius * h.normal;
}

template <class BaseT, class TraitsT>
[[nodiscard]] constexpr pos<3, fractional_result<typename BaseT::scalar_t>> centroid_of(pyramid<BaseT, TraitsT> const& py)
{
    using frac_t = fractional_result<typename BaseT::scalar_t>;
    frac_t offset;
    if constexpr (std::is_same_v<TraitsT, default_object_tag>)
        offset = frac_t(0.25); // == 1/4
    else if constexpr (std::is_same_v<TraitsT, boundary_tag>)
    {
        const auto aBase = area_of(py.base);
        const auto aMantle = area_of(boundary_no_caps_of(py));
        offset = aMantle / (frac_t(3) * (aMantle + aBase)); // weighted average of aMantle * 1/3 + aBase * 0
    }
    else // boundary_no_caps_tag
        offset = frac_t(1) / frac_t(3);
    return centroid_of(py.base) + offset * py.height * normal_of(py.base);
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
