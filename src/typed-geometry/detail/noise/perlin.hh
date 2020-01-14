#pragma once

//#include <typed-geometry/common/random.hh>
//#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/detail/noise_helper.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
namespace noise
{
// classic perlin noise
// (see https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83)
// (and https://github.com/ashima/webgl-noise/blob/master/src/classicnoise2D.glsl)

// 2D
template <class ScalarT>
ScalarT perlin_noise(const pos<2, ScalarT>& P) // TODO allow seeding, perlin_noise_seed()!
{
    auto Pi = floor(pos<4, ScalarT>(P.x, P.y, P.x, P.y)) + vec<4, ScalarT>(ScalarT(0.0), ScalarT(0.0), ScalarT(1.0), ScalarT(1.0));
    auto Pf = fract(pos<4, ScalarT>(P.x, P.y, P.x, P.y)) - pos<4, ScalarT>(ScalarT(0.0), ScalarT(0.0), ScalarT(1.0), ScalarT(1.0));

    Pi = mod289(Pi); // to avoid truncation effects in permutation

    auto ix = pos<4, ScalarT>(Pi.x, Pi.z, Pi.x, Pi.z);
    auto iy = pos<4, ScalarT>(Pi.y, Pi.y, Pi.w, Pi.w);
    auto fx = pos<4, ScalarT>(Pf.x, Pf.z, Pf.x, Pf.z);
    auto fy = pos<4, ScalarT>(Pf.y, Pf.y, Pf.w, Pf.w);

    auto i = permute(iy + vec<4, ScalarT>(permute(ix)));

    auto gx = fract(i * ScalarT(0.0243902439)) * ScalarT(2.0) - ScalarT(1.0); // 1/41 = 0.024...
    auto gy = abs(gx) - ScalarT(0.5);
    auto tx = floor(vec<4, ScalarT>(gx) + ScalarT(0.5));
    gx = gx - tx;

    auto g00 = vec<2, ScalarT>(gx.x, gy.x);
    auto g10 = vec<2, ScalarT>(gx.y, gy.y);
    auto g01 = vec<2, ScalarT>(gx.z, gy.z);
    auto g11 = vec<2, ScalarT>(gx.w, gy.w);

    auto norm = taylorInvSqrt(pos<4, ScalarT>(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;

    auto n00 = dot(g00, vec<2, ScalarT>(fx.x, fy.x));
    auto n10 = dot(g10, vec<2, ScalarT>(fx.y, fy.y));
    auto n01 = dot(g01, vec<2, ScalarT>(fx.z, fy.z));
    auto n11 = dot(g11, vec<2, ScalarT>(fx.w, fy.w));

    auto fade_xy = fade(vec<2, ScalarT>(Pf));
    auto n_x = mix(vec<2, ScalarT>(n00, n01), vec<2, ScalarT>(n10, n11), fade_xy.x);
    auto n_xy = mix(n_x.x, n_x.y, fade_xy.y);
    return ScalarT(2.3) * n_xy;
}

template <class ScalarT>
ScalarT perlin_noise(const ScalarT x, const ScalarT y) // TODO allow seeding, perlin_noise_seed()!
{
    return perlin_noise(pos<2, ScalarT>(x, y));
}

template <class ScalarT>
ScalarT perlin_noise(const pos<3, ScalarT>& P)
{
    auto Pi0 = floor(P);                            // integer part for indexing
    auto Pi1 = Pi0 + vec<3, ScalarT>(ScalarT(1.0)); // integer part + 1
    Pi0 = mod289(Pi0);
    Pi1 = mod289(Pi1);
    auto Pf0 = vec<3, ScalarT>(fract(P));                            // fractional part for interpolation
    auto Pf1 = Pf0 - vec<3, ScalarT>(ScalarT(1.0)); // fractional part - 1.0
    auto ix = pos<4, ScalarT>(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
    auto iy = vec<4, ScalarT>(Pi0.y, Pi0.y, Pi1.y, Pi1.y);
    auto iz0 = vec<4, ScalarT>(Pi0.z);
    auto iz1 = vec<4, ScalarT>(Pi1.z);

    auto ixy = permute(permute(ix) + iy);
    auto ixy0 = permute(ixy + iz0);
    auto ixy1 = permute(ixy + iz1);

    auto gx0 = vec<4, ScalarT>(ixy0 / ScalarT(7.0));
    auto gy0 = fract(floor(gx0) / ScalarT(7.0)) - ScalarT(0.5);
    gx0 = fract(gx0);
    auto gz0 = vec<4, ScalarT>(ScalarT(0.5)) - abs(gx0) - abs(gy0);
    auto sz0 = step(gz0, vec<4, ScalarT>(ScalarT(0.0)));

    // workaround for vec * vec operation
    auto subtract = sz0;
    auto stepped = step(vec<4, ScalarT>::zero, gx0) - ScalarT(0.5);
    for (auto i = 0; i < 4; i++)
        subtract[i] *= stepped[i];
    gx0 -= subtract;
    // workaround for vec * vec operation
    subtract = sz0;
    stepped = step(vec<4, ScalarT>::zero, gy0) - ScalarT(0.5);
    for (auto i = 0; i < 4; i++)
        subtract[i] *= stepped[i];
    gy0 -= subtract;

    auto gx1 = vec<4, ScalarT>(ixy1 / ScalarT(7.0));
    auto gy1 = fract(floor(gx1) / ScalarT(7.0)) - ScalarT(0.5);
    gx1 = fract(gx1);
    auto gz1 = vec<4, ScalarT>(pos<4, ScalarT>(ScalarT(0.5) - abs(gx1)) - abs(gy1));
    auto sz1 = step(gz1, vec<4, ScalarT>(ScalarT(0.0)));
    // workaround for vec * vec operation
    stepped = step(vec<4, ScalarT>::zero, gx1) - ScalarT(0.5);
    subtract = sz1;
    for (auto i = 0; i < 4; i++)
        subtract[i] *= stepped[i];
    gx1 -= subtract;
    // workaround for vec * vec operation
    stepped = step(vec<4, ScalarT>::zero, gy1) - ScalarT(0.5);
    subtract = sz1;
    for (auto i = 0; i < 4; i++)
        subtract[i] *= stepped[i];
    gy1 -= subtract;

    auto g000 = vec<3, ScalarT>(gx0.x, gy0.x, gz0.x);
    auto g100 = vec<3, ScalarT>(gx0.y, gy0.y, gz0.y);
    auto g010 = vec<3, ScalarT>(gx0.z, gy0.z, gz0.z);
    auto g110 = vec<3, ScalarT>(gx0.w, gy0.w, gz0.w);
    auto g001 = vec<3, ScalarT>(gx1.x, gy1.x, gz1.x);
    auto g101 = vec<3, ScalarT>(gx1.y, gy1.y, gz1.y);
    auto g011 = vec<3, ScalarT>(gx1.z, gy1.z, gz1.z);
    auto g111 = vec<3, ScalarT>(gx1.w, gy1.w, gz1.w);

    auto norm0 = taylorInvSqrt(pos<4, ScalarT>(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
    g000 *= norm0.x;
    g010 *= norm0.y;
    g100 *= norm0.z;
    g110 *= norm0.w;
    auto norm1 = taylorInvSqrt(pos<4, ScalarT>(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
    g001 *= norm1.x;
    g011 *= norm1.y;
    g101 *= norm1.z;
    g111 *= norm1.w;

    auto n000 = dot(g000, Pf0);
    auto n100 = dot(g100, vec<3, ScalarT>(Pf1.x, Pf0.y, Pf0.z));
    auto n010 = dot(g010, vec<3, ScalarT>(Pf0.x, Pf1.y, Pf0.z));
    auto n110 = dot(g110, vec<3, ScalarT>(Pf1.x, Pf1.y, Pf0.z));
    auto n001 = dot(g001, vec<3, ScalarT>(Pf0.x, Pf0.y, Pf1.z));
    auto n101 = dot(g101, vec<3, ScalarT>(Pf1.x, Pf0.y, Pf1.z));
    auto n011 = dot(g011, vec<3, ScalarT>(Pf0.x, Pf1.y, Pf1.z));
    auto n111 = dot(g111, Pf1);

    auto fade_xyz = fade(Pf0);
    auto n_z = mix(vec<4, ScalarT>(n000, n100, n010, n110), vec<4, ScalarT>(n001, n101, n011, n111), fade_xyz.z);
    auto n_yz = mix(vec<2, ScalarT>(n_z.x, n_z.y), vec<2, ScalarT>(n_z.z, n_z.w), fade_xyz.y);
    auto n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
    return ScalarT(2.2) * n_xyz;
}

template <class ScalarT>
ScalarT perlin_noise(const ScalarT x, const ScalarT y, const ScalarT z) // TODO allow seeding, perlin_noise_seed()!
{
    return perlin_noise(pos<3, ScalarT>(x, y, z));
}

} // namespace noise
} // namespace tg
