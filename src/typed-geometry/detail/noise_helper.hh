#pragma once

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
namespace noise
{
template <class ScalarT>
pos<4, ScalarT> mod289(pos<4, ScalarT> x)
{
    return pos<4, ScalarT>(x - floor(x * (ScalarT(1.0) / ScalarT(289.0))) * ScalarT(289.0));
}

template <class ScalarT>
pos<4, ScalarT> permute(pos<4, ScalarT> x)
{
    // TODO * operator for pos?
    auto ret = x;
    auto fx = floor(x * (ScalarT(1.0) / ScalarT(289.0)));
    for (auto i = 0; i < 4; i++)
        ret[i] -= fx[i] * ScalarT(289.0);
    return ret;
}

template <class ScalarT>
pos<4, ScalarT> taylorInvSqrt(pos<4, ScalarT> r)
{
    return ScalarT(1.79284291400159) - ScalarT(0.85373472095314) * r;
}

template <class ScalarT>
pos<2, ScalarT> fade(vec<2, ScalarT> t)
{
    // TODO * operator for pos?
    auto ret = pos<2, ScalarT>::zero;
    for (auto i = 0; i < 2; i++)
        ret[i] = t[i] * t[i] * t[i] * (t[i] * (t[i] * ScalarT(6.0) - ScalarT(15.0)) + ScalarT(10.0));
    return ret;
}
} // namespace noise
} // namespace tg
