#pragma once

#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr box<D, ScalarT> aabb(pos<D, ScalarT> const& v)
{
    return {v, v};
}

template <int D, class ScalarT>
TG_NODISCARD constexpr box<D, ScalarT> aabb(box<D, ScalarT> const& b)
{
    return b;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr box<D, ScalarT> aabb(triangle<D, ScalarT> const& t)
{
    return aabb(t.v0, t.v1, t.v2);
}

template <class PrimA, class PrimB, class... PrimsT>
TG_NODISCARD constexpr auto aabb(PrimA const& pa, PrimB const& pb, PrimsT const&... prims) -> decltype(aabb(pa))
{
    auto ba = aabb(pa);
    auto bb = aabb(pb);
    static_assert(is_same<decltype(ba), decltype(bb)>, "all arguments must have the same aabb box type");
    auto b = decltype(ba)(min(ba.min, bb.min), max(ba.max, bb.max));
    return aabb(b, prims...);
}


} // namespace tg
