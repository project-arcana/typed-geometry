#pragma once

#include "../../types/vec.hh"
#include "../traits.hh"

namespace tg
{
// -- operator@ --

// vec +- vec = vec
TG_IMPL_DEFINE_BINARY_OP(vec, vec, vec, +);
TG_IMPL_DEFINE_BINARY_OP(vec, vec, vec, -);

// vec */ size = vec
TG_IMPL_DEFINE_BINARY_OP(vec, vec, size, *);
TG_IMPL_DEFINE_BINARY_OP(vec, vec, size, /);

// +vec, -vec
TG_IMPL_DEFINE_UNARY_OP(vec, +);
TG_IMPL_DEFINE_UNARY_OP(vec, -);

// scalar OP vec, vec OP scalar
TG_IMPL_DEFINE_BINARY_OP_SCALAR(vec, -);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(vec, +);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(vec, *);
TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(vec);


// -- functions --

TG_IMPL_DEFINE_REDUCTION_OP_BINARY(vec, vec, ScalarT, dot, +, *);

template <class ScalarT>
constexpr vec<3, ScalarT> cross(vec<3, ScalarT> const& a, vec<3, ScalarT> const& b)
{
    vec<3, ScalarT> r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

template <class ScalarT>
constexpr ScalarT cross(vec<2, ScalarT> const& a, vec<2, ScalarT> const& b)
{
    return a.x * b.y - a.y * b.x;
}

template <int D, class ScalarT>
constexpr squared_result<ScalarT> length2(vec<D, ScalarT> const& v)
{
    auto a = vec<D, squared_result<ScalarT>>(v);
    return dot(a, a);
}
template <int D, class ScalarT>
constexpr fractional_result<ScalarT> length(vec<D, ScalarT> const& v)
{
    return sqrt(length2(vec<D, fractional_result<ScalarT>>(v)));
}

template <int D, class ScalarT>
constexpr vec<D, fractional_result<ScalarT>> normalize(vec<D, ScalarT> const& v)
{
    return v / length(v);
}
template <int D, class ScalarT>
constexpr vec<D, fractional_result<ScalarT>> normalize_safe(vec<D, ScalarT> const& v, ScalarT eps = ScalarT(0))
{
    auto l = length(v);
    return l <= eps ? vec<D, ScalarT>::zero : v / l;
}

} // namespace tg
