#pragma once

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/objects/pyramid.hh>

namespace tg
{
template <class BaseT, class TraitsT>
[[nodiscard]] constexpr pos<3, typename BaseT::scalar_t> apex_of(pyramid<BaseT, TraitsT> const& p)
{
    return centroid_of(p.base) + normal(p.base) * p.height;
}
} // namespace tg
