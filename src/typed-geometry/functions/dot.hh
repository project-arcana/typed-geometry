#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr ScalarT dot(vec<1, ScalarT> const& a, vec<1, ScalarT> const& b)
{
    return a.x * ScalarT(b.x);
}
template <class ScalarT>
TG_NODISCARD constexpr ScalarT dot(vec<2, ScalarT> const& a, vec<2, ScalarT> const& b)
{
    return a.x * ScalarT(b.x) + //
           a.y * ScalarT(b.y);
}
template <class ScalarT>
TG_NODISCARD constexpr ScalarT dot(vec<3, ScalarT> const& a, vec<3, ScalarT> const& b)
{
    return a.x * ScalarT(b.x) + //
           a.y * ScalarT(b.y) + //
           a.z * ScalarT(b.z);
}
template <class ScalarT>
TG_NODISCARD constexpr ScalarT dot(vec<4, ScalarT> const& a, vec<4, ScalarT> const& b)
{
    return a.x * ScalarT(b.x) + //
           a.y * ScalarT(b.y) + //
           a.z * ScalarT(b.z) + //
           a.w * ScalarT(b.w);
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT dot(dir<1, ScalarT> const& a, dir<1, ScalarT> const& b)
{
    return a.x * b.x;
}
template <class ScalarT>
TG_NODISCARD constexpr ScalarT dot(dir<2, ScalarT> const& a, dir<2, ScalarT> const& b)
{
    return a.x * b.x + //
           a.y * b.y;
}
template <class ScalarT>
TG_NODISCARD constexpr ScalarT dot(dir<3, ScalarT> const& a, dir<3, ScalarT> const& b)
{
    return a.x * b.x + //
           a.y * b.y + //
           a.z * b.z;
}
template <class ScalarT>
TG_NODISCARD constexpr ScalarT dot(dir<4, ScalarT> const& a, dir<4, ScalarT> const& b)
{
    return a.x * b.x + //
           a.y * b.y + //
           a.z * b.z + //
           a.w * b.w;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr ScalarT dot(dir<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return dot(vec<D, ScalarT>(a), b);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr ScalarT dot(vec<D, ScalarT> const& a, dir<D, ScalarT> const& b)
{
    return dot(a, vec<D, ScalarT>(b));
}
} // namespace tg
