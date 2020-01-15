#pragma once

#include <typed-geometry/detail/noise_helper.hh>
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
    const vec4 C = vec<4, ScalarT>(ScalarT(0.211324865405187), ScalarT(0.366025403784439), ScalarT(-0.577350269189626), ScalarT(0.024390243902439));
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
} // namespace noise
} // namespace tg
