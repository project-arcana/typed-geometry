#pragma once

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/objects/pyramid.hh>

#include "centroid.hh"
#include "normal.hh"

namespace tg
{
template <class BaseT, class TraitsT>
[[nodiscard]] constexpr pos<3, typename BaseT::scalar_t> apex_of(pyramid<BaseT, TraitsT> const& p)
{
    return centroid(p.base) + normal(p.base) * p.height;
}
} // namespace tg
