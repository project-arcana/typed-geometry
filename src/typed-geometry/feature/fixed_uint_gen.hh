#pragma once

#include <immintrin.h>

#include <typed-geometry/types/scalars/fixed_uint.hh>

namespace tg::detail
{
template <int w_res, class T0, class T1>
fixed_uint<w_res> mul(T0 const& lhs, T1 const& rhs);

template <>
fixed_uint<2> mul(u64 const& lhs, u64 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs, rhs, &h00);
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u128 const& lhs, u64 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs, &h00);
    l10 = lhs.d[1] * rhs;
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u192 const& lhs, u64 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs, &h00);
    l10 = lhs.d[1] * rhs;
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u256 const& lhs, u64 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs, &h00);
    l10 = lhs.d[1] * rhs;
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u64 const& lhs, u128 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs, rhs.d[0], &h00);
    l01 = lhs * rhs.d[1];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u128 const& lhs, u128 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = lhs.d[0] * rhs.d[1];
    l10 = lhs.d[1] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u192 const& lhs, u128 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = lhs.d[0] * rhs.d[1];
    l10 = lhs.d[1] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u256 const& lhs, u128 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = lhs.d[0] * rhs.d[1];
    l10 = lhs.d[1] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u64 const& lhs, u192 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs, rhs.d[0], &h00);
    l01 = lhs * rhs.d[1];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u128 const& lhs, u192 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = lhs.d[0] * rhs.d[1];
    l10 = lhs.d[1] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u192 const& lhs, u192 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = lhs.d[0] * rhs.d[1];
    l10 = lhs.d[1] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u256 const& lhs, u192 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = lhs.d[0] * rhs.d[1];
    l10 = lhs.d[1] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u64 const& lhs, u256 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs, rhs.d[0], &h00);
    l01 = lhs * rhs.d[1];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u128 const& lhs, u256 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = lhs.d[0] * rhs.d[1];
    l10 = lhs.d[1] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u192 const& lhs, u256 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = lhs.d[0] * rhs.d[1];
    l10 = lhs.d[1] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<2> mul(u256 const& lhs, u256 const& rhs)
{
    fixed_uint<2> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = lhs.d[0] * rhs.d[1];
    l10 = lhs.d[1] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    return res;
}

template <>
fixed_uint<3> mul(u64 const& lhs, u64 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs, rhs, &h00);
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    return res;
}

template <>
fixed_uint<3> mul(u128 const& lhs, u64 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs, &h00);
    l10 = _mulx_u64(lhs.d[1], rhs, &h10);
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u192 const& lhs, u64 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l10 = 0;
    u64 l20 = 0;
    u64 h00 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs, &h00);
    l10 = _mulx_u64(lhs.d[1], rhs, &h10);
    l20 = lhs.d[2] * rhs;
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u256 const& lhs, u64 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l10 = 0;
    u64 l20 = 0;
    u64 h00 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs, &h00);
    l10 = _mulx_u64(lhs.d[1], rhs, &h10);
    l20 = lhs.d[2] * rhs;
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u64 const& lhs, u128 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    l00 = _mulx_u64(lhs, rhs.d[0], &h00);
    l01 = _mulx_u64(lhs, rhs.d[1], &h01);
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u128 const& lhs, u128 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = lhs.d[1] * rhs.d[1];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u192 const& lhs, u128 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l20 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = lhs.d[1] * rhs.d[1];
    l20 = lhs.d[2] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u256 const& lhs, u128 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l20 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = lhs.d[1] * rhs.d[1];
    l20 = lhs.d[2] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u64 const& lhs, u192 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    l00 = _mulx_u64(lhs, rhs.d[0], &h00);
    l01 = _mulx_u64(lhs, rhs.d[1], &h01);
    l02 = lhs * rhs.d[2];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u128 const& lhs, u192 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l02 = lhs.d[0] * rhs.d[2];
    l11 = lhs.d[1] * rhs.d[1];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u192 const& lhs, u192 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l20 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l02 = lhs.d[0] * rhs.d[2];
    l11 = lhs.d[1] * rhs.d[1];
    l20 = lhs.d[2] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u256 const& lhs, u192 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l20 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l02 = lhs.d[0] * rhs.d[2];
    l11 = lhs.d[1] * rhs.d[1];
    l20 = lhs.d[2] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u64 const& lhs, u256 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    l00 = _mulx_u64(lhs, rhs.d[0], &h00);
    l01 = _mulx_u64(lhs, rhs.d[1], &h01);
    l02 = lhs * rhs.d[2];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u128 const& lhs, u256 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l02 = lhs.d[0] * rhs.d[2];
    l11 = lhs.d[1] * rhs.d[1];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u192 const& lhs, u256 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l20 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l02 = lhs.d[0] * rhs.d[2];
    l11 = lhs.d[1] * rhs.d[1];
    l20 = lhs.d[2] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    return res;
}

template <>
fixed_uint<3> mul(u256 const& lhs, u256 const& rhs)
{
    fixed_uint<3> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l20 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l02 = lhs.d[0] * rhs.d[2];
    l11 = lhs.d[1] * rhs.d[1];
    l20 = lhs.d[2] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    return res;
}

template <>
fixed_uint<4> mul(u64 const& lhs, u64 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 h00 = 0;
    l00 = _mulx_u64(lhs, rhs, &h00);
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    return res;
}

template <>
fixed_uint<4> mul(u128 const& lhs, u64 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l10 = 0;
    u64 h00 = 0;
    u64 h10 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs, &h00);
    l10 = _mulx_u64(lhs.d[1], rhs, &h10);
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    return res;
}

template <>
fixed_uint<4> mul(u192 const& lhs, u64 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l10 = 0;
    u64 l20 = 0;
    u64 h00 = 0;
    u64 h10 = 0;
    u64 h20 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs, &h00);
    l10 = _mulx_u64(lhs.d[1], rhs, &h10);
    l20 = _mulx_u64(lhs.d[2], rhs, &h20);
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h20, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u256 const& lhs, u64 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l10 = 0;
    u64 l20 = 0;
    u64 l30 = 0;
    u64 h00 = 0;
    u64 h10 = 0;
    u64 h20 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs, &h00);
    l10 = _mulx_u64(lhs.d[1], rhs, &h10);
    l20 = _mulx_u64(lhs.d[2], rhs, &h20);
    l30 = lhs.d[3] * rhs;
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h20, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l30, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u64 const& lhs, u128 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    l00 = _mulx_u64(lhs, rhs.d[0], &h00);
    l01 = _mulx_u64(lhs, rhs.d[1], &h01);
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    return res;
}

template <>
fixed_uint<4> mul(u128 const& lhs, u128 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    u64 h11 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = _mulx_u64(lhs.d[1], rhs.d[1], &h11);
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h11, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u192 const& lhs, u128 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l20 = 0;
    u64 l21 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    u64 h11 = 0;
    u64 h20 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = _mulx_u64(lhs.d[1], rhs.d[1], &h11);
    l20 = _mulx_u64(lhs.d[2], rhs.d[0], &h20);
    l21 = lhs.d[2] * rhs.d[1];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h11, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h20, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l21, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u256 const& lhs, u128 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l20 = 0;
    u64 l21 = 0;
    u64 l30 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h10 = 0;
    u64 h11 = 0;
    u64 h20 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = _mulx_u64(lhs.d[1], rhs.d[1], &h11);
    l20 = _mulx_u64(lhs.d[2], rhs.d[0], &h20);
    l21 = lhs.d[2] * rhs.d[1];
    l30 = lhs.d[3] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h11, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h20, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l21, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l30, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u64 const& lhs, u192 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h02 = 0;
    l00 = _mulx_u64(lhs, rhs.d[0], &h00);
    l01 = _mulx_u64(lhs, rhs.d[1], &h01);
    l02 = _mulx_u64(lhs, rhs.d[2], &h02);
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h02, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u128 const& lhs, u192 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l12 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h02 = 0;
    u64 h10 = 0;
    u64 h11 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l02 = _mulx_u64(lhs.d[0], rhs.d[2], &h02);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = _mulx_u64(lhs.d[1], rhs.d[1], &h11);
    l12 = lhs.d[1] * rhs.d[2];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h02, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h11, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l12, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u192 const& lhs, u192 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l12 = 0;
    u64 l20 = 0;
    u64 l21 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h02 = 0;
    u64 h10 = 0;
    u64 h11 = 0;
    u64 h20 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l02 = _mulx_u64(lhs.d[0], rhs.d[2], &h02);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = _mulx_u64(lhs.d[1], rhs.d[1], &h11);
    l20 = _mulx_u64(lhs.d[2], rhs.d[0], &h20);
    l12 = lhs.d[1] * rhs.d[2];
    l21 = lhs.d[2] * rhs.d[1];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h02, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h11, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l12, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h20, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l21, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u256 const& lhs, u192 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l12 = 0;
    u64 l20 = 0;
    u64 l21 = 0;
    u64 l30 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h02 = 0;
    u64 h10 = 0;
    u64 h11 = 0;
    u64 h20 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l02 = _mulx_u64(lhs.d[0], rhs.d[2], &h02);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = _mulx_u64(lhs.d[1], rhs.d[1], &h11);
    l20 = _mulx_u64(lhs.d[2], rhs.d[0], &h20);
    l12 = lhs.d[1] * rhs.d[2];
    l21 = lhs.d[2] * rhs.d[1];
    l30 = lhs.d[3] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h02, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h11, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l12, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h20, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l21, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l30, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u64 const& lhs, u256 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l03 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h02 = 0;
    l00 = _mulx_u64(lhs, rhs.d[0], &h00);
    l01 = _mulx_u64(lhs, rhs.d[1], &h01);
    l02 = _mulx_u64(lhs, rhs.d[2], &h02);
    l03 = lhs * rhs.d[3];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h02, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l03, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u128 const& lhs, u256 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l03 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l12 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h02 = 0;
    u64 h10 = 0;
    u64 h11 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l02 = _mulx_u64(lhs.d[0], rhs.d[2], &h02);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = _mulx_u64(lhs.d[1], rhs.d[1], &h11);
    l03 = lhs.d[0] * rhs.d[3];
    l12 = lhs.d[1] * rhs.d[2];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h02, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l03, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h11, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l12, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u192 const& lhs, u256 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l03 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l12 = 0;
    u64 l20 = 0;
    u64 l21 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h02 = 0;
    u64 h10 = 0;
    u64 h11 = 0;
    u64 h20 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l02 = _mulx_u64(lhs.d[0], rhs.d[2], &h02);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = _mulx_u64(lhs.d[1], rhs.d[1], &h11);
    l20 = _mulx_u64(lhs.d[2], rhs.d[0], &h20);
    l03 = lhs.d[0] * rhs.d[3];
    l12 = lhs.d[1] * rhs.d[2];
    l21 = lhs.d[2] * rhs.d[1];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h02, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l03, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h11, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l12, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h20, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l21, &res.d[3]);
    return res;
}

template <>
fixed_uint<4> mul(u256 const& lhs, u256 const& rhs)
{
    fixed_uint<4> res;
    u64 l00 = 0;
    u64 l01 = 0;
    u64 l02 = 0;
    u64 l03 = 0;
    u64 l10 = 0;
    u64 l11 = 0;
    u64 l12 = 0;
    u64 l20 = 0;
    u64 l21 = 0;
    u64 l30 = 0;
    u64 h00 = 0;
    u64 h01 = 0;
    u64 h02 = 0;
    u64 h10 = 0;
    u64 h11 = 0;
    u64 h20 = 0;
    l00 = _mulx_u64(lhs.d[0], rhs.d[0], &h00);
    l01 = _mulx_u64(lhs.d[0], rhs.d[1], &h01);
    l02 = _mulx_u64(lhs.d[0], rhs.d[2], &h02);
    l10 = _mulx_u64(lhs.d[1], rhs.d[0], &h10);
    l11 = _mulx_u64(lhs.d[1], rhs.d[1], &h11);
    l20 = _mulx_u64(lhs.d[2], rhs.d[0], &h20);
    l03 = lhs.d[0] * rhs.d[3];
    l12 = lhs.d[1] * rhs.d[2];
    l21 = lhs.d[2] * rhs.d[1];
    l30 = lhs.d[3] * rhs.d[0];
    unsigned char c = 0;
    c = _addcarry_u64(c, res.d[0], l00, &res.d[0]);
    c = _addcarry_u64(c, res.d[1], h00, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l01, &res.d[1]);
    c = _addcarry_u64(c, res.d[1], l10, &res.d[1]);
    c = _addcarry_u64(c, res.d[2], h01, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l02, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], h10, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l11, &res.d[2]);
    c = _addcarry_u64(c, res.d[2], l20, &res.d[2]);
    c = _addcarry_u64(c, res.d[3], h02, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l03, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h11, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l12, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], h20, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l21, &res.d[3]);
    c = _addcarry_u64(c, res.d[3], l30, &res.d[3]);
    return res;
}

} // namespace tg::detail
