#pragma once

#include "../../types/pos.hh"
#include "../op_helper.hh"

namespace tg
{
template <int D, class ScalarT>
constexpr ScalarT const* data_ptr(pos<D, ScalarT> const& v)
{
    return &v.x;
}
template <int D, class ScalarT>
constexpr ScalarT* data_ptr(pos<D, ScalarT>& v)
{
    return &v.x;
}

// -- operator@ --

// TODO
// template <int D, class ScalarT>
// constexpr pos<D, ScalarT> operator+(pos<D, ScalarT> const& a, vec<D, ScalarT> const& b)
// {
//     return detail::compwise<ScalarT>(a, b, detail::add<ScalarT>);
// }
template <int D, class ScalarT>
constexpr pos<D, ScalarT> operator+(pos<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, b, detail::add<ScalarT>);
}

template <class ScalarT>
constexpr pos<1, ScalarT> operator+(pos<1, ScalarT> const& a, vec<1, ScalarT> const& b)
{
    return a.x + b.x;
}
template <class ScalarT>
constexpr pos<2, ScalarT> operator+(pos<2, ScalarT> const& a, vec<2, ScalarT> const& b)
{
    pos<2, ScalarT> r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}
template <class ScalarT>
constexpr pos<3, ScalarT> operator+(pos<3, ScalarT> const& a, vec<3, ScalarT> const& b)
{
    pos<3, ScalarT> r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}
template <class ScalarT>
constexpr pos<4, ScalarT> operator+(pos<4, ScalarT> const& a, vec<4, ScalarT> const& b)
{
    pos<4, ScalarT> r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    r.w = a.w + b.w;
    return r;
}
// TODO
// template <int D, class ScalarT>
// constexpr pos<D, ScalarT> operator-(pos<D, ScalarT> const& a, vec<D, ScalarT> const& b)
// {
//     return detail::compwise<ScalarT>(a, b, detail::sub<ScalarT>);
// }
template <int D, class ScalarT>
constexpr pos<D, ScalarT> operator-(pos<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, b, detail::sub<ScalarT>);
}
template <int D, class ScalarT>
constexpr pos<D, ScalarT> operator-(pos<D, ScalarT> const& a)
{
    return detail::compwise<ScalarT>(a, detail::neg<ScalarT>);
}

template <class ScalarT>
constexpr pos<1, ScalarT> operator-(pos<1, ScalarT> const& a, vec<1, ScalarT> const& b)
{
    return a.x - b.x;
}
template <class ScalarT>
constexpr pos<2, ScalarT> operator-(pos<2, ScalarT> const& a, vec<2, ScalarT> const& b)
{
    pos<2, ScalarT> r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}
template <class ScalarT>
constexpr pos<3, ScalarT> operator-(pos<3, ScalarT> const& a, vec<3, ScalarT> const& b)
{
    pos<3, ScalarT> r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}
template <class ScalarT>
constexpr pos<4, ScalarT> operator-(pos<4, ScalarT> const& a, vec<4, ScalarT> const& b)
{
    pos<4, ScalarT> r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    r.w = a.w - b.w;
    return r;
}

template <int D, class ScalarT>
constexpr pos<D, ScalarT> operator*(pos<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, b, detail::mul<ScalarT>);
}

template <class ScalarT>
constexpr pos<1, ScalarT> operator*(pos<1, ScalarT> const& a, size<1, ScalarT> const& b)
{
    return a.x * b.width;
}
template <class ScalarT>
constexpr pos<2, ScalarT> operator*(pos<2, ScalarT> const& a, size<2, ScalarT> const& b)
{
    pos<2, ScalarT> r;
    r.x = a.x * b.width;
    r.y = a.y * b.height;
    return r;
}
template <class ScalarT>
constexpr pos<3, ScalarT> operator*(pos<3, ScalarT> const& a, size<3, ScalarT> const& b)
{
    pos<3, ScalarT> r;
    r.x = a.x * b.width;
    r.y = a.y * b.height;
    r.z = a.z * b.depth;
    return r;
}

template <int D, class ScalarT>
constexpr pos<D, fractional_result<ScalarT>> operator/(pos<D, ScalarT> const& a, ScalarT b)
{
    return detail::compwise<ScalarT>(a, 1 / fractional_result<ScalarT>(b), detail::mul<ScalarT>);
}

template <int D, class ScalarT>
constexpr bool operator==(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return all(equals(a, b));
}
template <int D, class ScalarT>
constexpr bool operator!=(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return !(a == b);
}

template <int D>
constexpr pos<D, bool> operator&&(pos<D, bool> const& a, pos<D, bool> const& b)
{
    return detail::compwise<bool>(a, b, detail::logic_and);
}
template <int D>
constexpr pos<D, bool> operator||(pos<D, bool> const& a, pos<D, bool> const& b)
{
    return detail::compwise<bool>(a, b, detail::logic_or);
}
template <int D>
constexpr pos<D, bool> operator!(pos<D, bool> const& a)
{
    return detail::compwise<bool>(a, detail::logic_not);
}

// -- functions --

template <int D, class ScalarT>
constexpr pos<D, bool> equals(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return detail::compwise<bool>(a, b, detail::eq<ScalarT>);
}
template <int D>
constexpr bool any(pos<D, bool> const& v)
{
    return detail::reduce(v, detail::logic_or);
}
template <int D>
constexpr bool all(pos<D, bool> const& v)
{
    return detail::reduce(v, detail::logic_and);
}

template <class ScalarT>
constexpr ScalarT cross(pos<2, ScalarT> const& a, pos<2, ScalarT> const& b)
{
    return a.y * b.z - a.z * b.y;
}

template <int D, class ScalarT>
constexpr squared_result<ScalarT> length2(pos<D, ScalarT> const& v)
{
    auto a = pos<D, squared_result<ScalarT>>(v);
    pos<D, ScalarT> r;
    detail::apply(r, a, a, detail::mul<ScalarT>);
    return detail::reduce(r, detail::add<ScalarT>);
}
template <int D, class ScalarT>
constexpr fractional_result<ScalarT> length(pos<D, ScalarT> const& v)
{
    return sqrt(length2(pos<D, fractional_result<ScalarT>>(v)));
}

template <int D, class ScalarT>
constexpr pos<D, fractional_result<ScalarT>> normalize(pos<D, ScalarT> const& v)
{
    return v / length(v);
}
template <int D, class ScalarT>
constexpr pos<D, fractional_result<ScalarT>> normalize_safe(pos<D, ScalarT> const& v, ScalarT eps = ScalarT(0))
{
    auto l = length(v);
    return l <= eps ? pos<D, ScalarT>::zero : v / l;
}

} // namespace tg
