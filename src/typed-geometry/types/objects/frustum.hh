#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/functions/swizzling/swizzling-basic.hh>
#include <typed-geometry/functions/vector/dot.hh>
#include <typed-geometry/functions/vector/length.hh>
#include <typed-geometry/functions/vector/normalize.hh>
#include <typed-geometry/types/scalars/default.hh>
#include "../pos.hh"
#include "../vec.hh"
#include "plane.hh"

namespace tg
{
template <class ScalarT>
struct frustum;

// Common frustum types

using ffrustum = frustum<f32>;

using dfrustum = frustum<f64>;

// ======== IMPLEMENTATION ========

template <class ScalarT>
struct frustum
{
    // frustum represented by 6 planes
    array<tg::plane<3, ScalarT>, 6> planes;

    constexpr frustum() = default;
    explicit constexpr frustum(array<tg::plane<3, ScalarT>, 6> const& planes) : planes(planes)
    { /* TG_CONTRACT(..); */
    }

    // extract frustum from a view-projection-matrix (proj * view)
    explicit constexpr frustum(mat<4, 4, ScalarT> const& m)
    {
        // computing planes in order: left, right, bottom, top, near, far
        for (auto i = 0u; i < 3; ++i)
            for (auto j = 0u; j < 2; ++j)
            {
                // plane parameters from matrix (see http://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf)
                vec4 abcd;
                for (auto k = 0; k < 4; ++k)
                    abcd[k] = j == 0 ? m[k][3] + m[k][i] : m[k][3] - m[k][i];

                auto n = vec3(abcd);
                auto l = length(n);
                planes[2 * i + j] = plane3(dir3(n / l), -abcd.w / l);
            }
    }

    [[nodiscard]] bool operator==(frustum const& rhs) const
    {
        for (auto i = 0u; i < planes.size(); ++i)
        {
            if (planes[i] != rhs.planes[i])
                return false;
        }
        return true;
    }
    [[nodiscard]] bool operator!=(frustum const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, frustum<ScalarT>& v)
{
    i(v.planes, "planes");
}

} // namespace tg
