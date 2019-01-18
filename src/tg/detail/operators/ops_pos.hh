#pragma once

#include "../../types/pos.hh"
#include "../../types/size.hh"
#include "../../types/vec.hh"
#include "../macros.hh"
#include "../traits.hh"

namespace tg
{
// pos +- vec = pos
TG_IMPL_DEFINE_BINARY_OP(pos, vec, pos, +);
TG_IMPL_DEFINE_BINARY_OP(pos, vec, pos, -);

// pos */ size = pos
TG_IMPL_DEFINE_BINARY_OP(pos, size, pos, *);
TG_IMPL_DEFINE_BINARY_OP(pos, size, pos, /);

// pos - pos = vec
TG_IMPL_DEFINE_BINARY_OP(pos, pos, vec, -);

// +pos, -pos
TG_IMPL_DEFINE_UNARY_OP(pos, +);
TG_IMPL_DEFINE_UNARY_OP(pos, -);

// scalar OP pos, pos OP scalar
TG_IMPL_DEFINE_BINARY_OP_SCALAR(pos, -);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(pos, +);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(pos, *);
TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(pos);


template <class ScalarT>
constexpr ScalarT cross(pos<2, ScalarT> const& a, pos<2, ScalarT> const& b)
{
    return a.x * b.y - a.y * b.x;
}

template <class ScalarT>
constexpr squared_result<ScalarT> length2(pos<1, ScalarT> const& v)
{
    return v.x * v.x;
}
template <class ScalarT>
constexpr squared_result<ScalarT> length2(pos<2, ScalarT> const& v)
{
    return v.x * v.x + v.y * v.y;
}
template <class ScalarT>
constexpr squared_result<ScalarT> length2(pos<3, ScalarT> const& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}
template <class ScalarT>
constexpr squared_result<ScalarT> length2(pos<4, ScalarT> const& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
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
