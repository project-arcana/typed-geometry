#pragma once

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
namespace noise
{
template <int D, class ScalarT>
pos<D, ScalarT> mod289(const pos<D, ScalarT>& x)
{
    return pos<D, ScalarT>(x - floor(x * (ScalarT(1.0) / ScalarT(289.0))) * ScalarT(289.0));
}

template <class ScalarT>
pos<4, ScalarT> permute(const pos<4, ScalarT>& x)
{
    auto ret = comp<4, ScalarT>(x) * ((comp<4, ScalarT>(x) * ScalarT(34.0)) + ScalarT(1.0));
    return mod289(pos<4, ScalarT>(ret));
}

template <int D, class ScalarT>
pos<D, ScalarT> taylorInvSqrt(const pos<D, ScalarT>& r)
{
    return ScalarT(1.79284291400159) - ScalarT(0.85373472095314) * r;
}

template <int D, class ScalarT>
pos<D, ScalarT> fade(const vec<D, ScalarT>& t)
{
    auto ret = comp<D, ScalarT>(t);
    return pos<D, ScalarT>(ret * ret * ret * (ret * (ret * ScalarT(6.0) - ScalarT(15.0)) + ScalarT(10.0)));
}

template <class ScalarT>
vec<4, ScalarT> step(const vec<4, ScalarT>& edge, const vec<4, ScalarT>& x)
{
    // see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/step.xhtml
    auto ret = vec<4, ScalarT>::zero;
    for (auto i = 0; i < 4; i++)
    {
        if (x[i] >= edge[i])
            ret[i] = 1;
        // else: 0
    }
    return ret;
}

} // namespace noise
} // namespace tg
