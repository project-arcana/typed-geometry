#pragma once

#include "../types/mat.hh"
#include "../types/pos.hh"
#include "../types/size.hh"
#include "../types/vec.hh"

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
};
} // namespace detail


} // namespace tg
