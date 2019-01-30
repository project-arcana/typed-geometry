#pragma once

#include "../types/mat.hh"
#include "../types/pos.hh"
#include "../types/size.hh"
#include "../types/vec.hh"
#include "scalars/scalar_math.hh"

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
constexpr T zero()
{
    return {}; // all our types zero-initialize by default
}
template <class T>
constexpr auto ones() -> decltype(detail::special_values<T>::ones())
{
    return detail::special_values<T>::ones();
}
template <class T>
constexpr auto unit() -> decltype(detail::special_values<T>::unit())
{
    return detail::special_values<T>::unit();
}
template <class T>
constexpr auto unit_x() -> decltype(detail::special_values<T>::unit_x())
{
    return detail::special_values<T>::unit_x();
}
template <class T>
constexpr auto unit_y() -> decltype(detail::special_values<T>::unit_y())
{
    return detail::special_values<T>::unit_y();
}
template <class T>
constexpr auto unit_z() -> decltype(detail::special_values<T>::unit_z())
{
    return detail::special_values<T>::unit_z();
}
template <class T>
constexpr auto unit_w() -> decltype(detail::special_values<T>::unit_w())
{
    return detail::special_values<T>::unit_w();
}
template <class T>
constexpr auto identity() -> decltype(detail::special_values<T>::identity())
{
    return detail::special_values<T>::identity();
}
template <class T, class V>
constexpr auto diag(V const& v) -> decltype(detail::special_values<T>::diag(v))
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
    static constexpr mat<C, R, ScalarT> ones() { return mat<C, R, ScalarT>() + ScalarT(1); }
    static constexpr mat<C, R, ScalarT> identity()
    {
        mat<C, R, ScalarT> m;
        mat_set_00(m, ScalarT(1));
        mat_set_11(m, ScalarT(1));
        mat_set_22(m, ScalarT(1));
        mat_set_33(m, ScalarT(1));
        return m;
    }
    static constexpr mat<C, R, ScalarT> diag(ScalarT v)
    {
        mat<C, R, ScalarT> m;
        mat_set_00(m, v);
        mat_set_11(m, v);
        mat_set_22(m, v);
        mat_set_33(m, v);
        return m;
    }
    static constexpr mat<C, R, ScalarT> diag(vec<min(C, R), ScalarT> const& v)
    {
        mat<C, R, ScalarT> m;
        mat_set_00(m, ScalarT(v));
        mat_set_11(m, ScalarT(v));
        mat_set_22(m, ScalarT(v));
        mat_set_33(m, ScalarT(v));
        return m;
    }

    static constexpr void mat_set_00(mat<C, R, ScalarT>& m, ScalarT v) { m[0][0] = v; }
    static constexpr void mat_set_00(mat<C, R, ScalarT>& m, vec<min(C, R), ScalarT> const& v) { m[0][0] = v.x; }

    template <int CC, class = enable_if<CC >= 2 && R >= 2>>
    static constexpr void mat_set_11(mat<CC, R, ScalarT>& m, ScalarT v)
    {
        m[1][1] = v;
    }
    template <int CC, class = enable_if<CC >= 2 && R >= 2>>
    static constexpr void mat_set_11(mat<CC, R, ScalarT>& m, vec<min(C, R), ScalarT> const& v)
    {
        m[1][1] = v.y;
    }
    static constexpr void mat_set_11(...)
    { /* nothing */
    }

    template <int CC, class = enable_if<CC >= 3 && R >= 3>>
    static constexpr void mat_set_22(mat<CC, R, ScalarT>& m, ScalarT v)
    {
        m[2][2] = v;
    }
    template <int CC, class = enable_if<CC >= 3 && R >= 3>>
    static constexpr void mat_set_22(mat<CC, R, ScalarT>& m, vec<min(C, R), ScalarT> const& v)
    {
        m[2][2] = v.z;
    }
    static constexpr void mat_set_22(...)
    { /* nothing */
    }

    template <int CC, class = enable_if<CC >= 4 && R >= 4>>
    static constexpr void mat_set_33(mat<CC, R, ScalarT>& m, ScalarT v)
    {
        m[3][3] = v;
    }
    template <int CC, class = enable_if<CC >= 4 && R >= 4>>
    static constexpr void mat_set_33(mat<CC, R, ScalarT>& m, vec<min(C, R), ScalarT> const& v)
    {
        m[3][3] = v.w;
    }
    static constexpr void mat_set_33(...)
    { /* nothing */
    }
};
} // namespace detail


} // namespace tg
