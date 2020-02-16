#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr vec<3, ScalarT> cross(vec<3, ScalarT> const& a, vec_or_dir<3, ScalarT> const& b)
{
    return {
        a.y * b.z - a.z * b.y, //
        a.z * b.x - a.x * b.z, //
        a.x * b.y - a.y * b.x  //
    };
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT cross(vec<2, ScalarT> const& a, vec_or_dir<2, ScalarT> const& b)
{
    return a.x * b.y - a.y * b.x;
}

template <class ScalarT>
[[nodiscard]] constexpr vec<3, ScalarT> cross(dir<3, ScalarT> const& a, vec_or_dir<3, ScalarT> const& b)
{
    return {
        a.y * b.z - a.z * b.y, //
        a.z * b.x - a.x * b.z, //
        a.x * b.y - a.y * b.x  //
    };
}

template <class ScalarT>
[[nodiscard]] constexpr ScalarT cross(dir<2, ScalarT> const& a, vec_or_dir<2, ScalarT> const& b)
{
    return a.x * b.y - a.y * b.x;
}

template <class ScalarT>
[[nodiscard]] constexpr mat<3, 3, ScalarT> cross_product_matrix(vec<3, ScalarT> const& v)
{
    mat<3, 3, ScalarT> m;
    m[1][0] = -v.z;
    m[2][0] = v.y;
    m[2][1] = -v.x;

    m[0][1] = v.z;
    m[0][2] = -v.y;
    m[1][2] = v.x;
    return m;
}

} // namespace tg
