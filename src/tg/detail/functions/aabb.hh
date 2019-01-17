#pragma once

#include "../../types/box.hh"
#include "../../types/triangle.hh"

namespace tg
{
template <int D, class ScalarT>
box<D, ScalarT> aabb(pos<D, ScalarT> const& v)
{
    return {v, v};
}

template <int D, class ScalarT>
box<D, ScalarT> aabb(triangle<D, ScalarT> const& v)
{
    return aabb(v.v0, v.v1, v.v2);
}

template <int D, class ScalarT>
box<D, ScalarT> aabb(box<D, ScalarT> const& b)
{
    return b;
}

template <int D, class ScalarT, class PrimT, class... PrimsT>
box<D, ScalarT> aabb(box<D, ScalarT> const& b, PrimT const& prim, PrimsT const&... prims)
{
    auto bp = aabb(prim);
    auto b2 = {min(b.min, bp.min), max(b.max, bp.max)};
    return aabb(b2, prims...);
}

template <class PrimT, class... PrimsT>
auto aabb(PrimT const& prim, PrimsT const&... prims) -> decltype(aabb(prim))
{
    return aabb(aabb(prim), prims...);
}

} // namespace tg
