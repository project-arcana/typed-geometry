#pragma once

#include <typed-geometry/common/assert.hh>

#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/halfspace.hh>
#include <typed-geometry/types/objects/hyperplane.hh>
#include <typed-geometry/types/objects/line.hh>

#include <typed-geometry/detail/operators/ops_pos.hh>
#include <typed-geometry/functions/dot.hh>

// Header for all constructors that depend on functions

namespace tg
{
template <int D, class ScalarT>
constexpr box<D, ScalarT>::box(aabb<D, ScalarT> const& b)
{
    static_assert(is_floating_point<ScalarT>, "cannot be guaranteed for integers");
    auto half_e = (b.max - b.min) / ScalarT(2);
    center = b.min + half_e;
    for (auto i = 0; i < D; ++i)
    {
        auto v = vec_t();
        v[i] = ScalarT(1);
        half_extents[i] = v * half_e[i];
    }
}

template <int D, class ScalarT>
constexpr hyperplane<D, ScalarT>::hyperplane(dir_t n, pos_t p) : normal(n), dis(dot(vec<D, ScalarT>(p), n))
{
}
template <int D, class ScalarT>
constexpr halfspace<D, ScalarT>::halfspace(dir_t n, pos_t p) : normal(n), dis(dot(vec<D, ScalarT>(p), n))
{
}

template <int D, class ScalarT>
constexpr line<D, ScalarT> line<D, ScalarT>::from_points(pos_t a, pos_t b)
{
    return line(a, normalize(b - a));
}
}
