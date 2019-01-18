#pragma once

#include "../../types/types.hh"

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

template <int D, class ScalarT>
constexpr ScalarT const* data_ptr(vec<D, ScalarT> const& v)
{
    return &v.x;
}
template <int D, class ScalarT>
constexpr ScalarT* data_ptr(vec<D, ScalarT>& v)
{
    return &v.x;
}

template <int D, class ScalarT>
constexpr ScalarT const* data_ptr(size<D, ScalarT> const& v)
{
    return &v.width;
}
template <int D, class ScalarT>
constexpr ScalarT* data_ptr(size<D, ScalarT>& v)
{
    return &v.width;
}
} // namespace tg
