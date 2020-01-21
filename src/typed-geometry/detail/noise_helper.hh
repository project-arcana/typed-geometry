#pragma once

#include <typed-geometry/types/vec.hh>

namespace tg
{
namespace noise
{
template <int D, class ScalarT>
pos<D, ScalarT> mod289(const pos<D, ScalarT>& x)
{
    return pos<D, ScalarT>(x - floor(x * (ScalarT(1) / ScalarT(289))) * ScalarT(289));
}

template <int D, class ScalarT>
pos<D, ScalarT> permute(const pos<D, ScalarT>& x)
{
    auto ret = comp<D, ScalarT>(x) * ((comp<D, ScalarT>(x) * ScalarT(34)) + ScalarT(1));
    return mod289(pos<D, ScalarT>(ret));
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
    return pos<D, ScalarT>(ret * ret * ret * (ret * (ret * ScalarT(6) - ScalarT(15)) + ScalarT(10)));
}

template <int D, class ScalarT>
vec<D, ScalarT> step(const vec<D, ScalarT>& edge, const vec<D, ScalarT>& x)
{
    // see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/step.xhtml
    auto ret = vec<D, ScalarT>::zero;
    for (auto i = 0; i < D; i++)
    {
        if (x[i] >= edge[i])
            ret[i] = ScalarT(1);
        // else: 0
    }
    return ret;
}

} // namespace noise
} // namespace tg
