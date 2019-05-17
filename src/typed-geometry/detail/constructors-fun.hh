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
template <class ScalarT>
constexpr dir<1, ScalarT>::dir(ScalarT x) : x(x)
{
    TG_CONTRACT(dot(*this, *this) <= ScalarT(1.001) && "dirs must be normalized");
}
template <class ScalarT>
constexpr dir<2, ScalarT>::dir(ScalarT x, ScalarT y) : x(x), y(y)
{
    TG_CONTRACT(dot(*this, *this) <= ScalarT(1.001) && "dirs must be normalized");
}
template <class ScalarT>
constexpr dir<3, ScalarT>::dir(ScalarT x, ScalarT y, ScalarT z) : x(x), y(y), z(z)
{
    TG_CONTRACT(dot(*this, *this) <= ScalarT(1.001) && "dirs must be normalized");
}
template <class ScalarT>
constexpr dir<4, ScalarT>::dir(ScalarT x, ScalarT y, ScalarT z, ScalarT w) : x(x), y(y), z(z), w(w)
{
    TG_CONTRACT(dot(*this, *this) <= ScalarT(1.001) && "dirs must be normalized");
}

template <class ScalarT>
constexpr dir<1, ScalarT>::dir(vec<1, ScalarT> const& v) : x(v.x)
{
    TG_CONTRACT(dot(*this, *this) <= ScalarT(1.001) && "dirs must be normalized");
}
template <class ScalarT>
constexpr dir<2, ScalarT>::dir(vec<2, ScalarT> const& v) : x(v.x), y(v.y)
{
    TG_CONTRACT(dot(*this, *this) <= ScalarT(1.001) && "dirs must be normalized");
}
template <class ScalarT>
constexpr dir<3, ScalarT>::dir(vec<3, ScalarT> const& v) : x(v.x), y(v.y), z(v.z)
{
    TG_CONTRACT(dot(*this, *this) <= ScalarT(1.001) && "dirs must be normalized");
}
template <class ScalarT>
constexpr dir<4, ScalarT>::dir(vec<4, ScalarT> const& v) : x(v.x), y(v.y), z(v.z), w(v.w)
{
    TG_CONTRACT(dot(*this, *this) <= ScalarT(1.001) && "dirs must be normalized");
}

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
