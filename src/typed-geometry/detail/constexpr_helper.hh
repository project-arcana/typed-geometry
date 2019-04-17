#pragma once

#include <typed-geometry/types/vec.hh>

namespace tg
{
namespace detail
{
template <class ScalarT>
constexpr ScalarT& csubscript(vec<1, ScalarT>& v, int)
{
    return v.x;
}
template <class ScalarT>
constexpr ScalarT const& csubscript(vec<1, ScalarT> const& v, int)
{
    return v.x;
}
template <class ScalarT>
constexpr ScalarT const& csubscript(vec<2, ScalarT> const& v, int i)
{
    if (i == 0)
        return v.x;
    else
        return v.y;
}
template <class ScalarT>
constexpr ScalarT& csubscript(vec<2, ScalarT>& v, int i)
{
    if (i == 0)
        return v.x;
    else
        return v.y;
}
template <class ScalarT>
constexpr ScalarT const& csubscript(vec<3, ScalarT> const& v, int i)
{
    if (i == 0)
        return v.x;
    else if (i == 1)
        return v.y;
    else
        return v.z;
}
template <class ScalarT>
constexpr ScalarT& csubscript(vec<3, ScalarT>& v, int i)
{
    if (i == 0)
        return v.x;
    else if (i == 1)
        return v.y;
    else
        return v.z;
}
template <class ScalarT>
constexpr ScalarT& csubscript(vec<4, ScalarT>& v, int i)
{
    if (i == 0)
        return v.x;
    else if (i == 1)
        return v.y;
    else if (i == 2)
        return v.z;
    else
        return v.w;
}
template <class ScalarT>
constexpr ScalarT const& csubscript(vec<4, ScalarT> const& v, int i)
{
    if (i == 0)
        return v.x;
    else if (i == 1)
        return v.y;
    else if (i == 2)
        return v.z;
    else
        return v.w;
}
} // namespace detail
} // namespace tg
