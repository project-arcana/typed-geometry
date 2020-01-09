#pragma once

//#include <typed-geometry/common/random.hh>
//#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/detail/noise_helper.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
// classic 2D perlin noise (https://github.com/ashima/webgl-noise/blob/master/src/classicnoise2D.glsl)
template <class ScalarT>
ScalarT perlin_noise(pos<2, ScalarT> P) // TODO allow overloading with ScalarT x, ScalarT y too AND seeding, perlin_noise_seed()!
{
    auto Pi = floor(pos<4, ScalarT>(P.x, P.y, P.x, P.y)) + vec<4, ScalarT>(ScalarT(0.0), ScalarT(0.0), ScalarT(1.0), ScalarT(1.0));
    auto Pf = fract(pos<4, ScalarT>(P.x, P.y, P.x, P.y)) - pos<4, ScalarT>(ScalarT(0.0), ScalarT(0.0), ScalarT(1.0), ScalarT(1.0));

    Pi = noise::mod289(Pi); // to avoid truncation effects in permutation

    auto ix = pos<4, ScalarT>(Pi.x, Pi.z, Pi.x, Pi.z);
    auto iy = pos<4, ScalarT>(Pi.y, Pi.y, Pi.w, Pi.w);
    auto fx = pos<4, ScalarT>(Pf.x, Pf.z, Pf.x, Pf.z);
    auto fy = pos<4, ScalarT>(Pf.y, Pf.y, Pf.w, Pf.w);

    auto i = noise::permute(iy + vec<4, ScalarT>(noise::permute(ix)));

    auto gx = fract(i * (ScalarT(1.0) / ScalarT(41.0))) * ScalarT(2.0) - ScalarT(1.0);
    auto gy = abs(gx) - ScalarT(0.5);
    auto tx = floor(vec<4, ScalarT>(gx) + ScalarT(0.5));
    gx = gx - tx;

    auto g00 = vec<2, ScalarT>(gx.x, gy.x);
    auto g10 = vec<2, ScalarT>(gx.y, gy.y);
    auto g01 = vec<2, ScalarT>(gx.z, gy.z);
    auto g11 = vec<2, ScalarT>(gx.w, gy.w);

    auto norm = noise::taylorInvSqrt(pos<4, ScalarT>(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;

    auto n00 = dot(g00, vec<2, ScalarT>(fx.x, fy.x));
    auto n10 = dot(g10, vec<2, ScalarT>(fx.y, fy.y));
    auto n01 = dot(g01, vec<2, ScalarT>(fx.z, fy.z));
    auto n11 = dot(g11, vec<2, ScalarT>(fx.w, fy.w));

    auto fade_xy = noise::fade(vec<2, ScalarT>(Pf));
    auto n_x = mix(vec<2, ScalarT>(n00, n01), vec<2, ScalarT>(n10, n11), fade_xy.x);
    auto n_xy = mix(n_x.x, n_x.y, fade_xy.y);
    return ScalarT(2.3) * n_xy;
}
} // namespace tg
