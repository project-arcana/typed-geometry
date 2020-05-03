#pragma once

#include <typed-geometry/types/comp.hh>

#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include <typed-geometry/functions/vector/interpolate.hh>

// defines operator[] for several object types
// obj[coord] indexes into objects that are "coordinate addressable" like triangle or segment

// respects the following identity:
//   p == obj[coordinates(obj, p)]

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> segment<D, ScalarT>::operator[](ScalarT t) const
{
    return mix(this->pos0, this->pos1, t);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> ray<D, ScalarT>::operator[](ScalarT t) const
{
    return this->origin + this->dir * t;
}
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> line<D, ScalarT>::operator[](ScalarT t) const
{
    return this->pos + this->dir * t;
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<D, ScalarT> aabb<D, ScalarT, TraitsT>::operator[](comp<D, ScalarT> const& c) const
{
    return this->min + (this->max - this->min) * size(c);
}

/// Note that the box goes from -1 to 1 instead of the usual 0 to 1
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<D, ScalarT> box<D, ScalarT, D, TraitsT>::operator[](comp<D, ScalarT> const& c) const
{
    return this->center + this->half_extents * vec(c);
}
/// Note that the box goes from -1 to 1 instead of the usual 0 to 1
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<3, ScalarT> box<2, ScalarT, 3, TraitsT>::operator[](comp<2, ScalarT> const& c) const
{
    return this->center + this->half_extents * vec(c);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> triangle<D, ScalarT>::operator[](comp<3, ScalarT> const& barycoords) const
{
    return interpolate(*this, barycoords[0], barycoords[1], barycoords[2]);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> triangle<D, ScalarT>::operator[](comp<2, ScalarT> const& barycoords) const
{
    return interpolate(*this, barycoords[0], barycoords[1], 1 - barycoords[0] - barycoords[1]);
}
}
