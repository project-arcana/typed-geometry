#pragma once

#include <typed-geometry/detail/noise_helper.hh>
#include <typed-geometry/functions/minmax.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
namespace noise
{
// simplex noise 2D-4D
// https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83

// 2D
template <class ScalarT>
ScalarT simplex_noise(const pos<2, ScalarT>& v) // TODO allow seeding, perlin_noise_seed()!
{
    const vec<4, ScalarT> C = vec<4, ScalarT>(ScalarT(0.211324865405187), ScalarT(0.366025403784439), ScalarT(-0.577350269189626), ScalarT(0.024390243902439));
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
}

template <class ScalarT>
ScalarT simplex_noise(const ScalarT x, const ScalarT y)
{
    return simplex_noise(pos<2, ScalarT>(x, y));
}

template <class ScalarT>
ScalarT simplex_noise(const ScalarT x)
{
    return simplex_noise(pos<2, ScalarT>(x, ScalarT(0)));
}

template <class ScalarT>
ScalarT simplex_noise(pos<1, ScalarT> const& p)
{
    return simplex_noise(p.x);
}

//	3D
// TODO sometimes returns values slightly above 1.0? e.g. 1.02...
// TODO note that passing the same value for all three variables results in jumps!
// TODO compare with other simplex3d? vec<3, ScalarT>(1) - .. casting issues?
template <class ScalarT>
ScalarT simplex_noise(pos<3, ScalarT> const& v)
{
    const auto C = vec<2, ScalarT>(ScalarT(1.0 / 6.0), ScalarT(1.0 / 3.0));
    const auto D = vec<4, ScalarT>(0.0, 0.5, 1.0, 2.0);

    // first corner
    auto i = floor(v + dot(v, vec<3, ScalarT>(C.y)));
    auto x0 = v - i + dot(i, vec<3, ScalarT>(C.x));

    // other corners
    auto g = step(vec<3, ScalarT>(x0.y, x0.z, x0.x), vec<3, ScalarT>(x0.x, x0.y, x0.z));
    auto l = vec<3, ScalarT>(1) - g;
    vec<3, ScalarT> i1;
    i1.x = min(g.x, l.z);
    i1.y = min(g.y, l.x);
    i1.z = min(g.z, l.y);

    vec<3, ScalarT> i2;
    i2.x = max(g.x, l.z);
    i2.y = max(g.y, l.x);
    i2.z = max(g.z, l.y);

    //  x0 = x0 - 0. + 0.0 * C
    vec<3, ScalarT> x1 = x0 - i1 + vec<3, ScalarT>(C.x);
    vec<3, ScalarT> x2 = x0 - i2 + vec<3, ScalarT>(C.y);
    vec<3, ScalarT> x3 = x0 - vec<3, ScalarT>(D.y);

    // permutations
    i = mod289(i);
    auto p = permute(permute(permute(i.z + pos<4, ScalarT>(0, i1.z, i2.z, 1)) + i.y + vec<4, ScalarT>(ScalarT(0), i1.y, i2.y, ScalarT(1))) + i.x
                     + vec<4, ScalarT>(0, i1.x, i2.x, 1));

    // gradients
    // Gradients: 7x7 points over a square, mapped onto an octahedron.
    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
    auto n_ = ScalarT(0.142857142857); // 1.0/7.0
    auto ns = n_ * vec<3, ScalarT>(D.w, D.y, D.z) - vec<3, ScalarT>(D.x, D.z, D.x);

    auto j = p - ScalarT(49) * floor(p * ns.z * ns.z); //  mod(p,N*N)

    auto x_ = floor(j * ns.z);
    auto y_ = floor(j - ScalarT(7) * x_); // mod(j,N)

    auto x = x_ * ns.x + vec<4, ScalarT>(ns.y);
    auto y = y_ * ns.x + vec<4, ScalarT>(ns.y);
    auto h = vec<4, ScalarT>::one - abs(x) - abs(y);

    auto b0 = vec<4, ScalarT>(x.x, x.y, y.x, y.y);
    auto b1 = vec<4, ScalarT>(x.z, x.w, y.z, y.w);

    auto s0 = floor(b0) * ScalarT(2) + ScalarT(1);
    auto s1 = floor(b1) * ScalarT(2) + ScalarT(1);
    auto sh = -step(h, vec<4, ScalarT>::zero);

    auto a0 = vec<4, ScalarT>(b0.x, b0.z, b0.y, b0.w) + comp<4, ScalarT>(s0.x, s0.z, s0.y, s0.w) * comp<4, ScalarT>(sh.x, sh.x, sh.y, sh.y);
    auto a1 = vec<4, ScalarT>(b1.x, b1.z, b1.y, b1.w) + comp<4, ScalarT>(s1.x, s1.z, s1.y, s1.w) * comp<4, ScalarT>(sh.z, sh.z, sh.w, sh.w);

    auto p0 = vec<3, ScalarT>(a0.x, a0.y, h.x);
    auto p1 = vec<3, ScalarT>(a0.z, a0.w, h.y);
    auto p2 = vec<3, ScalarT>(a1.x, a1.y, h.z);
    auto p3 = vec<3, ScalarT>(a1.z, a1.w, h.w);

    // normalize gradients
    auto norm = taylorInvSqrt(pos<4, ScalarT>(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    // mix final noise value
    auto m = comp<4, ScalarT>(0.6) - comp<4, ScalarT>(ScalarT(dot(x0, x0)), ScalarT(dot(x1, x1)), ScalarT(dot(x2, x2)), ScalarT(dot(x3, x3)));
    for (auto i = 0; i < 4; i++)
    {
        m[i] = max(m[i], ScalarT(0));
    }
    m = m * m;
    return ScalarT(42) * dot(vec<4, ScalarT>(m * m), vec<4, ScalarT>(dot(p0, x0), dot(p1, x1), dot(p2, x2), dot(p3, x3)));
}

template <class ScalarT>
ScalarT simplex_noise(ScalarT const x, ScalarT const y, ScalarT const z)
{
    return simplex_noise(pos<3, ScalarT>(x, y, z));
}

} // namespace noise
} // namespace tg

/*float snoise(vec3 v)
  {
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i);
  vec4 p = permute( permute( permute(
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                dot(p2,x2), dot(p3,x3) ) );
  }*/
