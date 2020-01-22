#pragma once

#include <typed-geometry/detail/noise_helper.hh>
#include <typed-geometry/functions/minmax.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
namespace noise
{
// simplex noise 1D-3D
// https://github.com/SRombauts/SimplexNoise/blob/master/src/SimplexNoise.cpp
/**
 * 1D Perlin simplex noise
 *
 *  Takes around 74ns on an AMD APU.
 *
 * @param[in] x ScalarT coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
template <class ScalarT>
ScalarT simplex_noise(pos<1, ScalarT> const& p)
{
    ScalarT n0, n1; // Noise contributions from the two "corners"

    // No need to skew the input space in 1D

    // Corners coordinates (nearest integer values):
    auto i0 = fastfloor(p.x);
    auto i1 = i0 + 1;
    // Distances to corners (between 0 and 1):
    auto x0 = p.x - i0;
    auto x1 = x0 - ScalarT(1);

    // Calculate the contribution from the first corner
    auto t0 = ScalarT(1) - x0 * x0;
    //  if(t0 < 0.0f) t0 = 0.0f; // not possible
    t0 *= t0;
    n0 = t0 * t0 * grad(hash(i0), x0);

    // Calculate the contribution from the second corner
    auto t1 = ScalarT(1) - x1 * x1;
    //  if(t1 < 0.0f) t1 = 0.0f; // not possible
    t1 *= t1;
    n1 = t1 * t1 * grad(hash(i1), x1);

    // The maximum value of this noise is 8*(3/4)^4 = 2.53125
    // A factor of 0.395 scales to fit exactly within [-1,1]
    return ScalarT(0.395) * (n0 + n1);
}

/*
// 2D
// https://github.com/ashima/webgl-noise/blob/master/src/noise2D.glsl
template <class ScalarT>
ScalarT simplex_noise(pos<2, ScalarT> const& v) // TODO allow seeding, perlin_noise_seed()!
{
    auto const C = vec<4, ScalarT>(ScalarT(0.211324865405187), ScalarT(0.366025403784439), ScalarT(-0.577350269189626), ScalarT(0.024390243902439));
    auto i = floor(v + dot(v, vec<2, ScalarT>(C.y)));
    auto x0 = v - i + dot(i, vec<2, ScalarT>(C.x));
    auto i1 = (x0.x > x0.y) ? vec<2, ScalarT>(1, 0) : vec<2, ScalarT>(0, 1);
    auto x12 = vec<4, ScalarT>(x0.x, x0.y, x0.x, x0.y) + vec<4, ScalarT>(C.x, C.x, C.z, C.z);
    x12.x -= i1.x;
    x12.y -= i1.y;
    i = mod289(i);
    auto p = permute(permute(i.y + pos<3, ScalarT>(0, i1.y, 1)) + i.x + vec<3, ScalarT>(0, i1.x, 1));

    // TODO unsure from source whether this is correct
    // i believe its just supposed to be clamped to 0 element wise
    auto m = comp<3, ScalarT>(ScalarT(0.5))
             - comp<3, ScalarT>(dot(x0, x0), dot(vec<2, ScalarT>(x12), vec<2, ScalarT>(x12)),
                                dot(vec<2, ScalarT>(x12.z, x12.w), vec<2, ScalarT>(x12.z, x12.w)));
    for (auto i = 0; i < 3; i++)
        m[i] = max(m[i], 0);
    m = m * m;
    m = m * m;

    auto x = 2 * fract(p * comp<3, ScalarT>(C.w)) - 1;
    auto h = comp<3, ScalarT>(abs(x) - ScalarT(0.5));
    auto ox = floor(x + ScalarT(0.5));
    auto a0 = comp<3, ScalarT>(x - ox);
    m *= ScalarT(1.79284291400159) - ScalarT(0.85373472095314) * (a0 * a0 + h * h);
    auto g = vec<3, ScalarT>::zero;
    g.x = a0[0] * x0[0] + h[0] * x0[1];
    // g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    g.y = a0[1] * x12[0] + h[1] * x12[1];
    g.z = a0[2] * x12[2] + h[2] * x12[3];
    return 130 * dot(vec<3, ScalarT>(m), vec<3, ScalarT>(g));
}*/

// 2D simplex_noise
// // https://github.com/SRombauts/SimplexNoise/blob/master/src/SimplexNoise.cpp
template <class ScalarT>
ScalarT simplex_noise(pos<2, ScalarT> const& p)
{
    ScalarT n0, n1, n2; // Noise contributions from the three corners

    // Skewing/Unskewing factors for 2D
    static ScalarT const F2 = ScalarT(0.366025403); // F2 = (sqrt(3) - 1) / 2
    static ScalarT const G2 = ScalarT(0.211324865); // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

    // Skew the input space to determine which simplex cell we're in
    auto const s = (p.x + p.y) * F2; // Hairy factor for 2D
    auto const xs = p.x + s;
    auto const ys = p.y + s;
    auto const i = fastfloor(xs);
    auto const j = fastfloor(ys);

    // Unskew the cell origin back to (x,y) space
    auto const t = static_cast<ScalarT>(i + j) * G2;
    auto const X0 = i - t;
    auto const Y0 = j - t;
    auto const x0 = p.x - X0; // The x,y distances from the cell origin
    auto const y0 = p.y - Y0;

    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    tg::i32 i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if (x0 > y0)
    { // lower triangle, XY order: (0,0)->(1,0)->(1,1)
        i1 = 1;
        j1 = 0;
    }
    else
    { // upper triangle, YX order: (0,0)->(0,1)->(1,1)
        i1 = 0;
        j1 = 1;
    }

    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6

    auto const x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    auto const y1 = y0 - j1 + G2;
    auto const x2 = x0 - ScalarT(1) + ScalarT(2) * G2; // Offsets for last corner in (x,y) unskewed coords
    auto const y2 = y0 - ScalarT(1) + ScalarT(2) * G2;

    // Work out the hashed gradient indices of the three simplex corners
    auto const gi0 = hash(i + hash(j));
    auto const gi1 = hash(i + i1 + hash(j + j1));
    auto const gi2 = hash(i + 1 + hash(j + 1));

    // Calculate the contribution from the first corner
    ScalarT t0 = ScalarT(0.5) - x0 * x0 - y0 * y0;
    if (t0 < 0)
    {
        n0 = ScalarT(0);
    }
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * grad(gi0, x0, y0);
    }

    // Calculate the contribution from the second corner
    ScalarT t1 = ScalarT(0.5) - x1 * x1 - y1 * y1;
    if (t1 < 0)
    {
        n1 = ScalarT(0);
    }
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * grad(gi1, x1, y1);
    }

    // Calculate the contribution from the third corner
    ScalarT t2 = ScalarT(0.5) - x2 * x2 - y2 * y2;
    if (t2 < 0)
    {
        n2 = ScalarT(0);
    }
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * grad(gi2, x2, y2);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return ScalarT(45.23065) * (n0 + n1 + n2);
}

template <class ScalarT>
ScalarT simplex_noise(const ScalarT x, const ScalarT y)
{
    return simplex_noise(pos<2, ScalarT>(x, y));
}

template <class ScalarT>
ScalarT simplex_noise(const ScalarT x)
{
    return simplex_noise(pos<1, ScalarT>(x));
}

/**
 * 3D Perlin simplex noise
 *
 * @param[in] x ScalarT coordinate
 * @param[in] y ScalarT coordinate
 * @param[in] z ScalarT coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */

// See https://github.com/SRombauts/SimplexNoise/blob/master/src/SimplexNoise.cpp
template <class ScalarT>
ScalarT simplex_noise(pos<3, ScalarT> const& p)
{
    ScalarT n0, n1, n2, n3; // Noise contributions from the four corners

    // Skewing/Unskewing factors for 3D
    static auto const F3 = ScalarT(1.0 / 3.0);
    static auto const G3 = ScalarT(1.0 / 6.0);

    // Skew the input space to determine which simplex cell we're in
    auto s = (p.x + p.y + p.z) * F3; // Very nice and simple skew factor for 3D
    auto i = tg::i32(fastfloor(p.x + s));
    auto j = tg::i32(fastfloor(p.y + s));
    auto k = tg::i32(fastfloor(p.z + s));
    auto t = (i + j + k) * G3;
    auto X0 = i - t; // Unskew the cell origin back to (x,y,z) space
    auto Y0 = j - t;
    auto Z0 = k - t;
    ScalarT x0 = p.x - X0; // The x,y,z distances from the cell origin
    ScalarT y0 = p.y - Y0;
    ScalarT z0 = p.z - Z0;

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // Determine which simplex we are in.
    tg::i32 i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
    tg::i32 i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords
    if (x0 >= y0)
    {
        if (y0 >= z0)
        {
            i1 = 1;
            j1 = 0;
            k1 = 0;
            i2 = 1;
            j2 = 1;
            k2 = 0; // X Y Z order
        }
        else if (x0 >= z0)
        {
            i1 = 1;
            j1 = 0;
            k1 = 0;
            i2 = 1;
            j2 = 0;
            k2 = 1; // X Z Y order
        }
        else
        {
            i1 = 0;
            j1 = 0;
            k1 = 1;
            i2 = 1;
            j2 = 0;
            k2 = 1; // Z X Y order
        }
    }
    else
    { // x0<y0
        if (y0 < z0)
        {
            i1 = 0;
            j1 = 0;
            k1 = 1;
            i2 = 0;
            j2 = 1;
            k2 = 1; // Z Y X order
        }
        else if (x0 < z0)
        {
            i1 = 0;
            j1 = 1;
            k1 = 0;
            i2 = 0;
            j2 = 1;
            k2 = 1; // Y Z X order
        }
        else
        {
            i1 = 0;
            j1 = 1;
            k1 = 0;
            i2 = 1;
            j2 = 1;
            k2 = 0; // Y X Z order
        }
    }

    // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
    // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
    // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
    // c = 1/6.
    ScalarT x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
    ScalarT y1 = y0 - j1 + G3;
    ScalarT z1 = z0 - k1 + G3;
    ScalarT x2 = x0 - i2 + ScalarT(2) * G3; // Offsets for third corner in (x,y,z) coords
    ScalarT y2 = y0 - j2 + ScalarT(2) * G3;
    ScalarT z2 = z0 - k2 + ScalarT(2) * G3;
    ScalarT x3 = x0 - ScalarT(1) + ScalarT(3) * G3; // Offsets for last corner in (x,y,z) coords
    ScalarT y3 = y0 - ScalarT(1) + ScalarT(3) * G3;
    ScalarT z3 = z0 - ScalarT(1) + ScalarT(3) * G3;

    // Work out the hashed gradient indices of the four simplex corners
    auto gi0 = hash(i + hash(j + hash(k)));
    auto gi1 = hash(i + i1 + hash(j + j1 + hash(k + k1)));
    auto gi2 = hash(i + i2 + hash(j + j2 + hash(k + k2)));
    auto gi3 = hash(i + 1 + hash(j + 1 + hash(k + 1)));

    // Calculate the contribution from the four corners
    auto t0 = ScalarT(0.6) - x0 * x0 - y0 * y0 - z0 * z0;
    if (t0 < 0)
    {
        n0 = ScalarT(0);
    }
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * grad(gi0, x0, y0, z0);
    }
    auto t1 = ScalarT(0.6) - x1 * x1 - y1 * y1 - z1 * z1;
    if (t1 < 0)
    {
        n1 = ScalarT(0);
    }
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * grad(gi1, x1, y1, z1);
    }
    auto t2 = ScalarT(0.6) - x2 * x2 - y2 * y2 - z2 * z2;
    if (t2 < 0)
    {
        n2 = ScalarT(0);
    }
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * grad(gi2, x2, y2, z2);
    }
    auto t3 = ScalarT(0.6) - x3 * x3 - y3 * y3 - z3 * z3;
    if (t3 < 0)
    {
        n3 = ScalarT(0);
    }
    else
    {
        t3 *= t3;
        n3 = t3 * t3 * grad(gi3, x3, y3, z3);
    }
    // Add contributions from each corner to get the final noise value.
    // The result is scaled to stay just inside [-1,1]
    return ScalarT(32) * (n0 + n1 + n2 + n3);
}

template <class ScalarT>
ScalarT simplex_noise(ScalarT const x, ScalarT const y, ScalarT const z)
{
    return simplex_noise(pos<3, ScalarT>(x, y, z));
}

// 4D simplex noise
// https://github.com/ashima/webgl-noise/blob/master/src/noise4D.glsl

template <class ScalarT>
ScalarT simplex_noise(pos<4, ScalarT> const& p)
{
    const auto C = vec<4, ScalarT>(ScalarT(0.138196601125011),   // (5 - sqrt(5))/20  G4
                                   ScalarT(0.276393202250021),   // 2 * G4
                                   ScalarT(0.414589803375032),   // 3 * G4
                                   ScalarT(-0.447213595499958)); // -1 + 4 * G4

    // First corner
    auto i = floor(p + dot(p, vec<4, ScalarT>(ScalarT(0.309016994374947451)))); // (sqrt(5) - 1)/4 = 0.30901..
    auto x0 = p - i + dot(i, C.xxxx);

    // Other corners

    // Rank sorting originally contributed by Bill Licea-Kane, AMD (formerly ATI)
    vec<4, ScalarT> i0;
    auto isX = step(vec<3, ScalarT>(x0.y, x0.z, x0.w), vec<3, ScalarT>(x0.x));
    auto isYZ = step(x0.zww, x0.yyz);
    //  i0.x = dot( isX, vec3( 1.0 ) );
    i0.x = isX.x + isX.y + isX.z;
    i0.yzw = 1.0 - isX;
    //  i0.y += dot( isYZ.xy, vec2( 1.0 ) );
    i0.y += isYZ.x + isYZ.y;
    i0.zw += 1.0 - isYZ.xy;
    i0.z += isYZ.z;
    i0.w += 1.0 - isYZ.z;

    // i0 now contains the unique values 0,1,2,3 in each channel
    auto i3 = clamp(i0, 0.0, 1.0);
    auto i2 = clamp(i0 - 1.0, 0.0, 1.0);
    auto i1 = clamp(i0 - 2.0, 0.0, 1.0);

    //  x0 = x0 - 0.0 + 0.0 * C.xxxx
    //  x1 = x0 - i1  + 1.0 * C.xxxx
    //  x2 = x0 - i2  + 2.0 * C.xxxx
    //  x3 = x0 - i3  + 3.0 * C.xxxx
    //  x4 = x0 - 1.0 + 4.0 * C.xxxx
    auto x1 = x0 - i1 + C.xxxx;
    auto x2 = x0 - i2 + C.yyyy;
    auto x3 = x0 - i3 + C.zzzz;
    auto x4 = x0 + C.wwww;

    // Permutations
    i = mod289(i);
    float j0 = permute(permute(permute(permute(i.w) + i.z) + i.y) + i.x);
    vec4 j1 = permute(permute(permute(permute(i.w + vec4(i1.w, i2.w, i3.w, 1.0)) + i.z + vec4(i1.z, i2.z, i3.z, 1.0)) + i.y + vec4(i1.y, i2.y, i3.y, 1.0))
                      + i.x + vec4(i1.x, i2.x, i3.x, 1.0));

    // Gradients: 7x7x6 points over a cube, mapped onto a 4-cross polytope
    // 7*7*6 = 294, which is close to the ring size 17*17 = 289.
    auto ip = vec<4, ScalarT>(ScalarT(1.0 / 294.0), ScalarT(1.0 / 49.0), ScalarT(1.0 / 7.0), 0);

    auto p0 = grad4(j0, ip);
    auto p1 = grad4(j1.x, ip);
    auto p2 = grad4(j1.y, ip);
    auto p3 = grad4(j1.z, ip);
    auto p4 = grad4(j1.w, ip);

    // Normalise gradients
    auto norm = taylorInvSqrt(pos<4, ScalarT>(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;
    p4 *= taylorInvSqrt(dot(p4, p4));

    // Mix contributions from the five corners
    // TODO is this okay? arithmetic operations like this for vec and scalarT??
    auto m0 = max(ScalarT(0.6) - vec<3, ScalarT>(dot(x0, x0), dot(x1, x1), dot(x2, x2)), ScalarT(0));
    auto m1 = max(ScalarT(0.6) - vec<2, ScalarT>(dot(x3, x3), dot(x4, x4)), ScalarT(0));
    m0 = m0 * m0;
    m1 = m1 * m1;
    return ScalarT(49) * (dot(m0 * m0, vec3(dot(p0, x0), dot(p1, x1), dot(p2, x2))) + dot(m1 * m1, vec2(dot(p3, x3), dot(p4, x4))));
}

template <class ScalarT>
ScalarT simplex_noise(ScalarT const x, ScalarT const y, ScalarT const z, ScalarT const w)
{
    return simplex_noise(pos<4, ScalarT>(x, y, z, w));
}


} // namespace noise
} // namespace tg
