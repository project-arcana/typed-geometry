#pragma once

#include <typed-geometry/detail/noise_helper.hh>
#include <typed-geometry/functions/basic/mix.hh>

namespace tg
{
namespace noise
{
// Classic perlin noise 1D-4D

/**
 * https://github.com/ashima/webgl-noise/blob/master/src/classicnoise2D.glsl
 * 2D Perlin noise
 *
 * @param[in] p     position to compute the noise at
 *
 * @return Noise value in the range[-1; 1]
 */
template <class ScalarT>
ScalarT perlin_noise(const pos<2, ScalarT>& p) // TODO allow seeding, perlin_noise_seed()!
{
    auto Pi = floor(pos<4, ScalarT>(p.x, p.y, p.x, p.y)) + vec<4, ScalarT>(ScalarT(0.0), ScalarT(0.0), ScalarT(1.0), ScalarT(1.0));
    auto Pf = fract(pos<4, ScalarT>(p.x, p.y, p.x, p.y)) - pos<4, ScalarT>(ScalarT(0.0), ScalarT(0.0), ScalarT(1.0), ScalarT(1.0));

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

// 1D perlin: calls 2D perlin noise with 0 as default 2nd coordinate
template <class ScalarT>
ScalarT perlin_noise(const pos<1, ScalarT>& p) // TODO allow seeding, perlin_noise_seed()!
{
    // TODO is 0 possible or does that make gcc buster fail in ci?
    return perlin_noise(pos<2, ScalarT>(p.x, ScalarT(0)));
}

template <class ScalarT>
ScalarT perlin_noise(const ScalarT x) // TODO allow seeding, perlin_noise_seed()!
{
    // TODO is 0 possible or does that make gcc buster fail in ci?
    return perlin_noise(pos<2, ScalarT>(x, ScalarT(0)));
}

/**
 * https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
 * 3D Perlin noise
 *
 * @param[in] p     position to compute the noise at
 *
 * @return Noise value in the range[-1; 1]
 */
template <class ScalarT>
ScalarT perlin_noise(const pos<3, ScalarT>& p)
{
    auto Pi0 = floor(p);                            // integer part for indexing
    auto Pi1 = Pi0 + vec<3, ScalarT>(ScalarT(1.0)); // integer part + 1
    Pi0 = mod289(Pi0);
    Pi1 = mod289(Pi1);
    auto Pf0 = vec<3, ScalarT>(fract(p));           // fractional part for interpolation
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

    auto subtract = comp<4, ScalarT>(sz0);
    auto stepped = comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gx0) - ScalarT(0.5));
    subtract *= stepped;
    gx0 -= subtract;

    subtract = comp<4, ScalarT>(sz0);
    stepped = comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gy0) - ScalarT(0.5));
    subtract *= stepped;
    gy0 -= subtract;

    auto gx1 = vec<4, ScalarT>(ixy1 / ScalarT(7.0));
    auto gy1 = fract(floor(gx1) / ScalarT(7.0)) - ScalarT(0.5);
    gx1 = fract(gx1);
    auto gz1 = vec<4, ScalarT>(pos<4, ScalarT>(ScalarT(0.5) - abs(gx1)) - abs(gy1));
    auto sz1 = step(gz1, vec<4, ScalarT>(ScalarT(0.0)));

    stepped = comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gx1) - ScalarT(0.5));
    subtract = comp<4, ScalarT>(sz1);
    subtract *= stepped;
    gx1 -= subtract;

    stepped = comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gy1) - ScalarT(0.5));
    subtract = comp<4, ScalarT>(sz1);
    subtract *= stepped;
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

/**
 * https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
 * 4D Perlin noise
 *
 * @param[in] p     position to compute the noise at
 *
 * @return Noise value in the range[-1; 1]
 */
template <class ScalarT>
ScalarT perlin_noise(const pos<4, ScalarT>& p)
{
    auto Pi0 = floor(p);           // integer part for indexing
    auto Pi1 = Pi0 + ScalarT(1.0); // integer part + 1
    Pi0 = mod289(Pi0);
    Pi1 = mod289(Pi1);
    auto Pf0 = vec<4, ScalarT>(fract(p)); // fractional part for interpolation
    auto Pf1 = Pf0 - ScalarT(1.0);        // fractional part - 1.0
    auto ix = pos<4, ScalarT>(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
    auto iy = vec<4, ScalarT>(Pi0.y, Pi0.y, Pi1.y, Pi1.y);
    auto iz0 = vec<4, ScalarT>(Pi0.z);
    auto iz1 = vec<4, ScalarT>(Pi1.z);
    auto iw0 = vec<4, ScalarT>(Pi0.w);
    auto iw1 = vec<4, ScalarT>(Pi1.w);

    auto ixy = permute(permute(ix) + iy);
    auto ixy0 = permute(ixy + iz0);
    auto ixy1 = permute(ixy + iz1);
    auto ixy00 = permute(ixy0 + iw0);
    auto ixy01 = permute(ixy0 + iw1);
    auto ixy10 = permute(ixy1 + iw0);
    auto ixy11 = permute(ixy1 + iw1);

    auto gx00 = vec<4, ScalarT>(ixy00 / ScalarT(7.0));
    auto gy00 = floor(gx00) / ScalarT(7.0);
    auto gz00 = floor(gy00) / ScalarT(6.0);
    gx00 = fract(gx00) - ScalarT(0.5);
    gy00 = fract(gy00) - ScalarT(0.5);
    gz00 = fract(gz00) - ScalarT(0.5);
    auto gw00 = vec<4, ScalarT>(pos<4, ScalarT>(0.75) - abs(gx00) - abs(gy00) - abs(gz00));
    auto sw00 = step(gw00, vec<4, ScalarT>::zero);
    gx00 -= comp<4, ScalarT>(sw00) * comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gx00) - ScalarT(0.5));
    gy00 -= comp<4, ScalarT>(sw00) * comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gy00) - ScalarT(0.5));

    auto gx01 = vec<4, ScalarT>(ixy01 / ScalarT(7.0));
    auto gy01 = floor(gx01) / ScalarT(7.0);
    auto gz01 = floor(gy01) / ScalarT(6.0);
    gx01 = fract(gx01) - ScalarT(0.5);
    gy01 = fract(gy01) - ScalarT(0.5);
    gz01 = fract(gz01) - ScalarT(0.5);
    auto gw01 = vec<4, ScalarT>(0.75) - abs(gx01) - abs(gy01) - abs(gz01);
    auto sw01 = step(gw01, vec<4, ScalarT>::zero);
    gx01 -= comp<4, ScalarT>(sw01) * comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gx01) - ScalarT(0.5));
    gy01 -= comp<4, ScalarT>(sw01) * comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gy01) - ScalarT(0.5));

    auto gx10 = vec<4, ScalarT>(ixy10 / ScalarT(7.0));
    auto gy10 = floor(gx10) / ScalarT(7.0);
    auto gz10 = floor(gy10) / ScalarT(6.0);
    gx10 = fract(gx10) - ScalarT(0.5);
    gy10 = fract(gy10) - ScalarT(0.5);
    gz10 = fract(gz10) - ScalarT(0.5);
    auto gw10 = vec<4, ScalarT>(0.75) - abs(gx10) - abs(gy10) - abs(gz10);
    auto sw10 = step(gw10, vec<4, ScalarT>::zero);
    gx10 -= comp<4, ScalarT>(sw10) * comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gx10) - ScalarT(0.5));
    gy10 -= comp<4, ScalarT>(sw10) * comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gy10) - ScalarT(0.5));

    auto gx11 = vec<4, ScalarT>(ixy11 / ScalarT(7.0));
    auto gy11 = floor(gx11) / ScalarT(7.0);
    auto gz11 = floor(gy11) / ScalarT(6.0);
    gx11 = fract(gx11) - ScalarT(0.5);
    gy11 = fract(gy11) - ScalarT(0.5);
    gz11 = fract(gz11) - ScalarT(0.5);
    auto gw11 = vec<4, ScalarT>(0.75) - abs(gx11) - abs(gy11) - abs(gz11);
    auto sw11 = step(gw11, vec<4, ScalarT>::zero);
    gx11 -= comp<4, ScalarT>(sw11) * comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gx11) - ScalarT(0.5));
    gy11 -= comp<4, ScalarT>(sw11) * comp<4, ScalarT>(step(vec<4, ScalarT>::zero, gy11) - ScalarT(0.5));

    auto g0000 = vec<4, ScalarT>(gx00.x, gy00.x, gz00.x, gw00.x);
    auto g1000 = vec<4, ScalarT>(gx00.y, gy00.y, gz00.y, gw00.y);
    auto g0100 = vec<4, ScalarT>(gx00.z, gy00.z, gz00.z, gw00.z);
    auto g1100 = vec<4, ScalarT>(gx00.w, gy00.w, gz00.w, gw00.w);
    auto g0010 = vec<4, ScalarT>(gx10.x, gy10.x, gz10.x, gw10.x);
    auto g1010 = vec<4, ScalarT>(gx10.y, gy10.y, gz10.y, gw10.y);
    auto g0110 = vec<4, ScalarT>(gx10.z, gy10.z, gz10.z, gw10.z);
    auto g1110 = vec<4, ScalarT>(gx10.w, gy10.w, gz10.w, gw10.w);
    auto g0001 = vec<4, ScalarT>(gx01.x, gy01.x, gz01.x, gw01.x);
    auto g1001 = vec<4, ScalarT>(gx01.y, gy01.y, gz01.y, gw01.y);
    auto g0101 = vec<4, ScalarT>(gx01.z, gy01.z, gz01.z, gw01.z);
    auto g1101 = vec<4, ScalarT>(gx01.w, gy01.w, gz01.w, gw01.w);
    auto g0011 = vec<4, ScalarT>(gx11.x, gy11.x, gz11.x, gw11.x);
    auto g1011 = vec<4, ScalarT>(gx11.y, gy11.y, gz11.y, gw11.y);
    auto g0111 = vec<4, ScalarT>(gx11.z, gy11.z, gz11.z, gw11.z);
    auto g1111 = vec<4, ScalarT>(gx11.w, gy11.w, gz11.w, gw11.w);

    auto norm00 = taylorInvSqrt(pos<4, ScalarT>(dot(g0000, g0000), dot(g0100, g0100), dot(g1000, g1000), dot(g1100, g1100)));
    g0000 *= norm00.x;
    g0100 *= norm00.y;
    g1000 *= norm00.z;
    g1100 *= norm00.w;

    auto norm01 = taylorInvSqrt(pos<4, ScalarT>(dot(g0001, g0001), dot(g0101, g0101), dot(g1001, g1001), dot(g1101, g1101)));
    g0001 *= norm01.x;
    g0101 *= norm01.y;
    g1001 *= norm01.z;
    g1101 *= norm01.w;

    auto norm10 = taylorInvSqrt(pos<4, ScalarT>(dot(g0010, g0010), dot(g0110, g0110), dot(g1010, g1010), dot(g1110, g1110)));
    g0010 *= norm10.x;
    g0110 *= norm10.y;
    g1010 *= norm10.z;
    g1110 *= norm10.w;

    auto norm11 = taylorInvSqrt(pos<4, ScalarT>(dot(g0011, g0011), dot(g0111, g0111), dot(g1011, g1011), dot(g1111, g1111)));
    g0011 *= norm11.x;
    g0111 *= norm11.y;
    g1011 *= norm11.z;
    g1111 *= norm11.w;

    auto n0000 = dot(g0000, Pf0);
    auto n1000 = dot(g1000, vec<4, ScalarT>(Pf1.x, Pf0.y, Pf0.z, Pf0.w));
    auto n0100 = dot(g0100, vec<4, ScalarT>(Pf0.x, Pf1.y, Pf0.z, Pf0.w));
    auto n1100 = dot(g1100, vec<4, ScalarT>(Pf1.x, Pf1.y, Pf0.z, Pf0.w));
    auto n0010 = dot(g0010, vec<4, ScalarT>(Pf0.x, Pf0.y, Pf1.z, Pf0.w));
    auto n1010 = dot(g1010, vec<4, ScalarT>(Pf1.x, Pf0.y, Pf1.z, Pf0.w));
    auto n0110 = dot(g0110, vec<4, ScalarT>(Pf0.x, Pf1.y, Pf1.z, Pf0.w));
    auto n1110 = dot(g1110, vec<4, ScalarT>(Pf1.x, Pf1.y, Pf1.z, Pf0.w));
    auto n0001 = dot(g0001, vec<4, ScalarT>(Pf0.x, Pf0.y, Pf0.z, Pf1.w));
    auto n1001 = dot(g1001, vec<4, ScalarT>(Pf1.x, Pf0.y, Pf0.z, Pf1.w));
    auto n0101 = dot(g0101, vec<4, ScalarT>(Pf0.x, Pf1.y, Pf0.z, Pf1.w));
    auto n1101 = dot(g1101, vec<4, ScalarT>(Pf1.x, Pf1.y, Pf0.z, Pf1.w));
    auto n0011 = dot(g0011, vec<4, ScalarT>(Pf0.x, Pf0.y, Pf1.z, Pf1.w));
    auto n1011 = dot(g1011, vec<4, ScalarT>(Pf1.x, Pf0.y, Pf1.z, Pf1.w));
    auto n0111 = dot(g0111, vec<4, ScalarT>(Pf0.x, Pf1.y, Pf1.z, Pf1.w));
    auto n1111 = dot(g1111, Pf1);

    auto fade_xyzw = fade(Pf0);
    auto n_0w = mix(vec<4, ScalarT>(n0000, n1000, n0100, n1100), vec<4, ScalarT>(n0001, n1001, n0101, n1101), fade_xyzw.w);
    auto n_1w = mix(vec<4, ScalarT>(n0010, n1010, n0110, n1110), vec<4, ScalarT>(n0011, n1011, n0111, n1111), fade_xyzw.w);
    auto n_zw = mix(n_0w, n_1w, fade_xyzw.z);
    auto n_yzw = mix(vec<2, ScalarT>(n_zw), vec<2, ScalarT>(n_zw.z, n_zw.w), fade_xyzw.y);
    auto n_xyzw = mix(n_yzw.x, n_yzw.y, fade_xyzw.x);
    return ScalarT(2.2) * n_xyzw;
}

template <class ScalarT>
ScalarT perlin_noise(ScalarT const x, ScalarT const y, ScalarT const z, ScalarT const w)
{
    return perlin_noise(pos<4, ScalarT>(x, y, z, w));
}
} // namespace noise

using noise::perlin_noise;
} // namespace tg
