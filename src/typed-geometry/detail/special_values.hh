#pragma once

#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/sphere.hh>

#include <typed-geometry/functions/basic/limits.hh>

/*
 * Special values of certain types:
 *
 * tg::zero<T>()           - vec, pos, size, mat
 * tg::ones<T>()           - vec, pos, size, mat
 * tg::unit_x/y/z/w<T>()   - vec
 * tg::unit<T>()           - size
 * tg::identity<T>()       - mat
 * tg::diag<T>(scalar/vec) - mat
 */

namespace tg
{
namespace detail
{
template <class>
struct special_values
{
};
} // namespace detail

template <class T>
[[nodiscard]] constexpr T min()
{
    return detail::limits<T>::min();
}
template <class T>
[[nodiscard]] constexpr T max()
{
    return detail::limits<T>::max();
}

template <class T>
[[nodiscard]] constexpr T zero()
{
    return {}; // all our types zero-initialize by default
}
template <class T>
[[nodiscard]] constexpr auto ones() -> decltype(detail::special_values<T>::ones())
{
    return detail::special_values<T>::ones();
}
template <class T>
[[nodiscard]] constexpr auto unit() -> decltype(detail::special_values<T>::unit())
{
    return detail::special_values<T>::unit();
}
template <class T>
[[nodiscard]] constexpr auto unit_x() -> decltype(detail::special_values<T>::unit_x())
{
    return detail::special_values<T>::unit_x();
}
template <class T>
[[nodiscard]] constexpr auto unit_y() -> decltype(detail::special_values<T>::unit_y())
{
    return detail::special_values<T>::unit_y();
}
template <class T>
[[nodiscard]] constexpr auto unit_z() -> decltype(detail::special_values<T>::unit_z())
{
    return detail::special_values<T>::unit_z();
}
template <class T>
[[nodiscard]] constexpr auto unit_w() -> decltype(detail::special_values<T>::unit_w())
{
    return detail::special_values<T>::unit_w();
}
template <class T>
[[nodiscard]] constexpr auto identity() -> decltype(detail::special_values<T>::identity())
{
    return detail::special_values<T>::identity();
}
template <class T, class V>
[[nodiscard]] constexpr auto diag(V const& v) -> decltype(detail::special_values<T>::diag(v))
{
    return detail::special_values<T>::diag(v);
}

// Implementation:

namespace detail
{
template <class ScalarT>
struct special_values<vec<1, ScalarT>>
{
    static constexpr vec<1, ScalarT> ones() { return {ScalarT(1)}; }
    static constexpr vec<1, ScalarT> unit_x() { return {ScalarT(1)}; }
};
template <class ScalarT>
struct special_values<vec<2, ScalarT>>
{
    static constexpr vec<2, ScalarT> ones() { return {ScalarT(1), ScalarT(1)}; }
    static constexpr vec<2, ScalarT> unit_x() { return {ScalarT(1), ScalarT(0)}; }
    static constexpr vec<2, ScalarT> unit_y() { return {ScalarT(0), ScalarT(1)}; }
};
template <class ScalarT>
struct special_values<vec<3, ScalarT>>
{
    static constexpr vec<3, ScalarT> ones() { return {ScalarT(1), ScalarT(1), ScalarT(1)}; }
    static constexpr vec<3, ScalarT> unit_x() { return {ScalarT(1), ScalarT(0), ScalarT(0)}; }
    static constexpr vec<3, ScalarT> unit_y() { return {ScalarT(0), ScalarT(1), ScalarT(0)}; }
    static constexpr vec<3, ScalarT> unit_z() { return {ScalarT(0), ScalarT(0), ScalarT(1)}; }
};
template <class ScalarT>
struct special_values<vec<4, ScalarT>>
{
    static constexpr vec<4, ScalarT> ones() { return {ScalarT(1), ScalarT(1), ScalarT(1), ScalarT(1)}; }
    static constexpr vec<4, ScalarT> unit_x() { return {ScalarT(1), ScalarT(0), ScalarT(0), ScalarT(0)}; }
    static constexpr vec<4, ScalarT> unit_y() { return {ScalarT(0), ScalarT(1), ScalarT(0), ScalarT(0)}; }
    static constexpr vec<4, ScalarT> unit_z() { return {ScalarT(0), ScalarT(0), ScalarT(1), ScalarT(0)}; }
    static constexpr vec<4, ScalarT> unit_w() { return {ScalarT(0), ScalarT(0), ScalarT(0), ScalarT(1)}; }
};

template <int D, class ScalarT>
struct special_values<pos<D, ScalarT>>
{
    static constexpr pos<D, ScalarT> ones() { return pos<D, ScalarT>{ScalarT(1)}; }
};

template <int D, class ScalarT>
struct special_values<size<D, ScalarT>>
{
    static constexpr size<D, ScalarT> ones() { return size<D, ScalarT>{ScalarT(1)}; }
    static constexpr size<D, ScalarT> unit() { return size<D, ScalarT>{ScalarT(1)}; }
};

template <int C, int R, class ScalarT>
struct special_values<mat<C, R, ScalarT>>
{
    static constexpr mat<C, R, ScalarT> ones()
    {
        mat<C, R, ScalarT> m;
        for (auto c = 0; c < C; ++c)
            m[c] = vec<R, ScalarT>(ScalarT(1));
        return m;
    }
    static constexpr mat<C, R, ScalarT> diag(ScalarT v)
    {
        mat<C, R, ScalarT> m;
        if constexpr (C >= 1 && R >= 1)
            m[0].x = v;
        if constexpr (C >= 2 && R >= 2)
            m[1].y = v;
        if constexpr (C >= 3 && R >= 3)
            m[2].z = v;
        if constexpr (C >= 4 && R >= 4)
            m[3].w = v;
        return m;
    }
    static constexpr mat<C, R, ScalarT> diag(vec<detail::min(C, R), ScalarT> const& v)
    {
        mat<C, R, ScalarT> m;
        if constexpr (C >= 1 && R >= 1)
            m[0].x = v.x;
        if constexpr (C >= 2 && R >= 2)
            m[1].y = v.y;
        if constexpr (C >= 3 && R >= 3)
            m[2].z = v.z;
        if constexpr (C >= 4 && R >= 4)
            m[3].w = v.w;
        return m;
    }
    static constexpr mat<C, R, ScalarT> identity() { return diag(ScalarT(1)); }
};
} // namespace detail

// set special values
template <class ScalarT>
const vec<1, ScalarT> vec<1, ScalarT>::zero = {ScalarT(0)};
template <class ScalarT>
const vec<1, ScalarT> vec<1, ScalarT>::one = {ScalarT(1)};
template <class ScalarT>
const vec<1, ScalarT> vec<1, ScalarT>::unit_x = {ScalarT(1)};

template <class ScalarT>
const vec<2, ScalarT> vec<2, ScalarT>::zero = {ScalarT(0), ScalarT(0)};
template <class ScalarT>
const vec<2, ScalarT> vec<2, ScalarT>::one = {ScalarT(1), ScalarT(1)};
template <class ScalarT>
const vec<2, ScalarT> vec<2, ScalarT>::unit_x = {ScalarT(1), ScalarT(0)};
template <class ScalarT>
const vec<2, ScalarT> vec<2, ScalarT>::unit_y = {ScalarT(0), ScalarT(1)};

template <class ScalarT>
const vec<3, ScalarT> vec<3, ScalarT>::zero = {ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const vec<3, ScalarT> vec<3, ScalarT>::one = {ScalarT(1), ScalarT(1), ScalarT(1)};
template <class ScalarT>
const vec<3, ScalarT> vec<3, ScalarT>::unit_x = {ScalarT(1), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const vec<3, ScalarT> vec<3, ScalarT>::unit_y = {ScalarT(0), ScalarT(1), ScalarT(0)};
template <class ScalarT>
const vec<3, ScalarT> vec<3, ScalarT>::unit_z = {ScalarT(0), ScalarT(0), ScalarT(1)};

template <class ScalarT>
const vec<4, ScalarT> vec<4, ScalarT>::zero = {ScalarT(0), ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const vec<4, ScalarT> vec<4, ScalarT>::one = {ScalarT(1), ScalarT(1), ScalarT(1), ScalarT(1)};
template <class ScalarT>
const vec<4, ScalarT> vec<4, ScalarT>::unit_x = {ScalarT(1), ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const vec<4, ScalarT> vec<4, ScalarT>::unit_y = {ScalarT(0), ScalarT(1), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const vec<4, ScalarT> vec<4, ScalarT>::unit_z = {ScalarT(0), ScalarT(0), ScalarT(1), ScalarT(0)};
template <class ScalarT>
const vec<4, ScalarT> vec<4, ScalarT>::unit_w = {ScalarT(0), ScalarT(0), ScalarT(0), ScalarT(1)};

template <class ScalarT>
const dir<1, ScalarT> dir<1, ScalarT>::pos_x = {ScalarT(1)};
template <class ScalarT>
const dir<1, ScalarT> dir<1, ScalarT>::neg_x = {ScalarT(-1)};

template <class ScalarT>
const dir<2, ScalarT> dir<2, ScalarT>::pos_x = {ScalarT(1), ScalarT(0)};
template <class ScalarT>
const dir<2, ScalarT> dir<2, ScalarT>::pos_y = {ScalarT(0), ScalarT(1)};
template <class ScalarT>
const dir<2, ScalarT> dir<2, ScalarT>::neg_x = {ScalarT(-1), ScalarT(0)};
template <class ScalarT>
const dir<2, ScalarT> dir<2, ScalarT>::neg_y = {ScalarT(0), ScalarT(-1)};

template <class ScalarT>
const dir<3, ScalarT> dir<3, ScalarT>::pos_x = {ScalarT(1), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const dir<3, ScalarT> dir<3, ScalarT>::pos_y = {ScalarT(0), ScalarT(1), ScalarT(0)};
template <class ScalarT>
const dir<3, ScalarT> dir<3, ScalarT>::pos_z = {ScalarT(0), ScalarT(0), ScalarT(1)};
template <class ScalarT>
const dir<3, ScalarT> dir<3, ScalarT>::neg_x = {ScalarT(-1), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const dir<3, ScalarT> dir<3, ScalarT>::neg_y = {ScalarT(0), ScalarT(-1), ScalarT(0)};
template <class ScalarT>
const dir<3, ScalarT> dir<3, ScalarT>::neg_z = {ScalarT(0), ScalarT(0), ScalarT(-1)};

template <class ScalarT>
const dir<4, ScalarT> dir<4, ScalarT>::pos_x = {ScalarT(1), ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const dir<4, ScalarT> dir<4, ScalarT>::pos_y = {ScalarT(0), ScalarT(1), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const dir<4, ScalarT> dir<4, ScalarT>::pos_z = {ScalarT(0), ScalarT(0), ScalarT(1), ScalarT(0)};
template <class ScalarT>
const dir<4, ScalarT> dir<4, ScalarT>::pos_w = {ScalarT(0), ScalarT(0), ScalarT(0), ScalarT(1)};
template <class ScalarT>
const dir<4, ScalarT> dir<4, ScalarT>::neg_x = {ScalarT(-1), ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const dir<4, ScalarT> dir<4, ScalarT>::neg_y = {ScalarT(0), ScalarT(-1), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const dir<4, ScalarT> dir<4, ScalarT>::neg_z = {ScalarT(0), ScalarT(0), ScalarT(-1), ScalarT(0)};
template <class ScalarT>
const dir<4, ScalarT> dir<4, ScalarT>::neg_w = {ScalarT(0), ScalarT(0), ScalarT(0), ScalarT(-1)};

template <class ScalarT>
const pos<1, ScalarT> pos<1, ScalarT>::zero = {ScalarT(0)};
template <class ScalarT>
const pos<2, ScalarT> pos<2, ScalarT>::zero = {ScalarT(0), ScalarT(0)};
template <class ScalarT>
const pos<3, ScalarT> pos<3, ScalarT>::zero = {ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const pos<4, ScalarT> pos<4, ScalarT>::zero = {ScalarT(0), ScalarT(0), ScalarT(0), ScalarT(0)};

template <class ScalarT>
const size<1, ScalarT> size<1, ScalarT>::zero = {ScalarT(0)};
template <class ScalarT>
const size<1, ScalarT> size<1, ScalarT>::ones = {ScalarT(1)};
template <class ScalarT>
const size<1, ScalarT> size<1, ScalarT>::unit = {ScalarT(1)};

template <class ScalarT>
const size<2, ScalarT> size<2, ScalarT>::zero = {ScalarT(0), ScalarT(0)};
template <class ScalarT>
const size<2, ScalarT> size<2, ScalarT>::ones = {ScalarT(1), ScalarT(1)};
template <class ScalarT>
const size<2, ScalarT> size<2, ScalarT>::unit = {ScalarT(1), ScalarT(1)};

template <class ScalarT>
const size<3, ScalarT> size<3, ScalarT>::zero = {ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const size<3, ScalarT> size<3, ScalarT>::ones = {ScalarT(1), ScalarT(1), ScalarT(1)};
template <class ScalarT>
const size<3, ScalarT> size<3, ScalarT>::unit = {ScalarT(1), ScalarT(1), ScalarT(1)};

template <class ScalarT>
const size<4, ScalarT> size<4, ScalarT>::zero = {ScalarT(0), ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
const size<4, ScalarT> size<4, ScalarT>::ones = {ScalarT(1), ScalarT(1), ScalarT(1), ScalarT(1)};
template <class ScalarT>
const size<4, ScalarT> size<4, ScalarT>::unit = {ScalarT(1), ScalarT(1), ScalarT(1), ScalarT(1)};

template <int C, int R, class ScalarT>
const mat<C, R, ScalarT> mat<C, R, ScalarT>::zero = {};
template <int C, int R, class ScalarT>
const mat<C, R, ScalarT> mat<C, R, ScalarT>::identity = tg::identity<mat<C, R, ScalarT>>();
template <int C, int R, class ScalarT>
const mat<C, R, ScalarT> mat<C, R, ScalarT>::ones = tg::ones<mat<C, R, ScalarT>>();

template <int C, int R, class ScalarT>
constexpr mat<C, R, ScalarT> mat<C, R, ScalarT>::diag(ScalarT v)
{
    return tg::diag<mat<C, R, ScalarT>>(v);
}
template <int C, int R, class ScalarT>
constexpr mat<C, R, ScalarT> mat<C, R, ScalarT>::diag(vec<detail::min(C, R), ScalarT> const& v)
{
    return tg::diag<mat<C, R, ScalarT>>(v);
}

// Objects


template <int D, class ScalarT, class TraitsT>
const aabb<D, ScalarT, TraitsT> aabb<D, ScalarT, TraitsT>::minus_one_to_one = {tg::pos<D, ScalarT>(ScalarT(-1)), tg::pos<D, ScalarT>(ScalarT(1))};
template <int D, class ScalarT, class TraitsT>
const aabb<D, ScalarT, TraitsT> aabb<D, ScalarT, TraitsT>::unit_from_zero = {tg::pos<D, ScalarT>(ScalarT(0)), tg::pos<D, ScalarT>(ScalarT(1))};
template <int D, class ScalarT, class TraitsT>
const aabb<D, ScalarT, TraitsT> aabb<D, ScalarT, TraitsT>::unit_centered = {tg::pos<D, ScalarT>(ScalarT(-0.5)), tg::pos<D, ScalarT>(ScalarT(0.5))};

template <int D, class ScalarT, class TraitsT>
const box<D, ScalarT, D, TraitsT> box<D, ScalarT, D, TraitsT>::minus_one_to_one = {tg::pos<D, ScalarT>::zero, tg::mat<D, D, ScalarT>::identity};
template <int D, class ScalarT, class TraitsT>
const box<D, ScalarT, D, TraitsT> box<D, ScalarT, D, TraitsT>::unit_from_zero
    = {tg::pos<D, ScalarT>(ScalarT(0.5)), tg::mat<D, D, ScalarT>::diag(ScalarT(0.5))};
template <int D, class ScalarT, class TraitsT>
const box<D, ScalarT, D, TraitsT> box<D, ScalarT, D, TraitsT>::unit_centered = {tg::pos<D, ScalarT>::zero, tg::mat<D, D, ScalarT>::diag(ScalarT(0.5))};

template <int D, class ScalarT, class TraitsT>
const sphere<D, ScalarT, D, TraitsT> sphere<D, ScalarT, D, TraitsT>::unit = {pos<D, ScalarT>::zero, ScalarT(1)};

} // namespace tg
