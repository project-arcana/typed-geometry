#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr ScalarT dot(vec<1, ScalarT> const& a, vec<1, ScalarT> const& b)
{
    return a.x * b.x;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT dot(vec<2, ScalarT> const& a, vec<2, ScalarT> const& b)
{
    return a.x * b.x + //
           a.y * b.y;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT dot(vec<3, ScalarT> const& a, vec<3, ScalarT> const& b)
{
    return a.x * b.x + //
           a.y * b.y + //
           a.z * b.z;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT dot(vec<4, ScalarT> const& a, vec<4, ScalarT> const& b)
{
    return a.x * b.x + //
           a.y * b.y + //
           a.z * b.z + //
           a.w * b.w;
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT dot(dir<1, ScalarT> const& a, dir<1, ScalarT> const& b)
{
    return a.x * b.x;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT dot(dir<2, ScalarT> const& a, dir<2, ScalarT> const& b)
{
    return a.x * b.x + //
           a.y * b.y;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT dot(dir<3, ScalarT> const& a, dir<3, ScalarT> const& b)
{
    return a.x * b.x + //
           a.y * b.y + //
           a.z * b.z;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT dot(dir<4, ScalarT> const& a, dir<4, ScalarT> const& b)
{
    return a.x * b.x + //
           a.y * b.y + //
           a.z * b.z + //
           a.w * b.w;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT dot(dir<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return dot(vec<D, ScalarT>(a), b);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT dot(vec<D, ScalarT> const& a, dir<D, ScalarT> const& b)
{
    return dot(a, vec<D, ScalarT>(b));
}
template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT dot(dir<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return dot(vec<D, ScalarT>(a), vec<D, ScalarT>(b));
}
template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT dot(vec<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return dot(a, vec<D, ScalarT>(b));
}
template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT dot(pos<D, ScalarT> const& a, dir<D, ScalarT> const& b)
{
    return dot(vec<D, ScalarT>(a), vec<D, ScalarT>(b));
}
template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT dot(pos<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return dot(vec<D, ScalarT>(a), b);
}

template <int D, class ScalarT>
[[deprecated("dot product between positions is ill-defined. did you mean dot(vec, pos) or dot(vec, vec)?")]] [[nodiscard]] constexpr ScalarT dot(
    pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return dot(vec(a), vec(b));
}
} // namespace tg
