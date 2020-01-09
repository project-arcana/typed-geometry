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
    return mod289(((x * ScalarT(34.0)) + ScalarT(1.0)) * x);
}

template <class ScalarT>
pos<4, ScalarT> taylorInvSqrt(pos<4, ScalarT> r)
{
    return ScalarT(1.79284291400159) - ScalarT(0.85373472095314) * r;
}

template <class ScalarT>
pos<2, ScalarT> fade(pos<2, ScalarT> t)
{
    return t * t * t * (t * (t * ScalarT(6.0) - ScalarT(15.0)) + ScalarT(10.0));
}
} // namespace noise
} // namespace tg
