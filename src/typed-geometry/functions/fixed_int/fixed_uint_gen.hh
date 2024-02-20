#pragma once

// This file was generated by generate_fixed_uint_multiplications.cc in TGSamples.

#include <typed-geometry/detail/intrinsics.hh>
#include <typed-geometry/feature/fixed_int.hh>

namespace tg::detail
{
using u64_word = fixed_uint<1>::word_t;
template <>
inline u128 mul(u64 lhs, u64 rhs)
{
    u128 res;
    u64_word l00 = 0;
    u64_word h00 = 0;
    l00 = detail::mul128(lhs, rhs, &h00);
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    res.d[1] = c + h00;
    return res;
}

template <>
inline u128 mul(u128 lhs, u64 rhs)
{
    u128 res;
    u64_word l00 = 0;
    u64_word l10 = 0;
    u64_word h00 = 0;
    l00 = detail::mul128(lhs.d[0], rhs, &h00);
    l10 = lhs.d[1] * rhs;
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    res.d[1] = c + h00 + l10;
    return res;
}

template <>
inline u128 mul(u64 lhs, u128 rhs)
{
    u128 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word h00 = 0;
    l00 = detail::mul128(lhs, rhs.d[0], &h00);
    l01 = lhs * rhs.d[1];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    res.d[1] = c + h00 + l01;
    return res;
}

template <>
inline u128 mul(u128 lhs, u128 rhs)
{
    u128 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l10 = 0;
    u64_word h00 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = lhs.d[0] * rhs.d[1];
    l10 = lhs.d[1] * rhs.d[0];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    res.d[1] = c + h00 + l01 + l10;
    return res;
}

template <>
inline u192 mul(u128 lhs, u64 rhs)
{
    u192 res;
    u64_word l00 = 0;
    u64_word l10 = 0;
    u64_word h00 = 0;
    u64_word h10 = 0;
    l00 = detail::mul128(lhs.d[0], rhs, &h00);
    l10 = detail::mul128(lhs.d[1], rhs, &h10);
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    res.d[2] = c + h10;
    return res;
}

template <>
inline u192 mul(u192 lhs, u64 rhs)
{
    u192 res;
    u64_word l00 = 0;
    u64_word l10 = 0;
    u64_word l20 = 0;
    u64_word h00 = 0;
    u64_word h10 = 0;
    l00 = detail::mul128(lhs.d[0], rhs, &h00);
    l10 = detail::mul128(lhs.d[1], rhs, &h10);
    l20 = lhs.d[2] * rhs;
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    res.d[2] = c + h10 + l20;
    return res;
}

template <>
inline u192 mul(u64 lhs, u128 rhs)
{
    u192 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    l00 = detail::mul128(lhs, rhs.d[0], &h00);
    l01 = detail::mul128(lhs, rhs.d[1], &h01);
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    res.d[2] = c + h01;
    return res;
}

template <>
inline u192 mul(u128 lhs, u128 rhs)
{
    u192 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h10 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = lhs.d[1] * rhs.d[1];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    res.d[2] = c + h01 + h10 + l11;
    return res;
}

template <>
inline u192 mul(u192 lhs, u128 rhs)
{
    u192 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word l20 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h10 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = lhs.d[1] * rhs.d[1];
    l20 = lhs.d[2] * rhs.d[0];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    res.d[2] = c + h01 + h10 + l11 + l20;
    return res;
}

template <>
inline u192 mul(u64 lhs, u192 rhs)
{
    u192 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    l00 = detail::mul128(lhs, rhs.d[0], &h00);
    l01 = detail::mul128(lhs, rhs.d[1], &h01);
    l02 = lhs * rhs.d[2];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    res.d[2] = c + h01 + l02;
    return res;
}

template <>
inline u192 mul(u128 lhs, u192 rhs)
{
    u192 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h10 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l02 = lhs.d[0] * rhs.d[2];
    l11 = lhs.d[1] * rhs.d[1];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    res.d[2] = c + h01 + l02 + h10 + l11;
    return res;
}

template <>
inline u192 mul(u192 lhs, u192 rhs)
{
    u192 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word l20 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h10 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l02 = lhs.d[0] * rhs.d[2];
    l11 = lhs.d[1] * rhs.d[1];
    l20 = lhs.d[2] * rhs.d[0];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    res.d[2] = c + h01 + l02 + h10 + l11 + l20;
    return res;
}

template <>
inline u256 mul(u192 lhs, u64 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l10 = 0;
    u64_word l20 = 0;
    u64_word h00 = 0;
    u64_word h10 = 0;
    u64_word h20 = 0;
    l00 = detail::mul128(lhs.d[0], rhs, &h00);
    l10 = detail::mul128(lhs.d[1], rhs, &h10);
    l20 = detail::mul128(lhs.d[2], rhs, &h20);
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l20, &res.d[2]);
    res.d[3] = c + h20;
    return res;
}

template <>
inline u256 mul(u256 lhs, u64 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l10 = 0;
    u64_word l20 = 0;
    u64_word l30 = 0;
    u64_word h00 = 0;
    u64_word h10 = 0;
    u64_word h20 = 0;
    l00 = detail::mul128(lhs.d[0], rhs, &h00);
    l10 = detail::mul128(lhs.d[1], rhs, &h10);
    l20 = detail::mul128(lhs.d[2], rhs, &h20);
    l30 = lhs.d[3] * rhs;
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l20, &res.d[2]);
    res.d[3] = c + h20 + l30;
    return res;
}

template <>
inline u256 mul(u128 lhs, u128 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h10 = 0;
    u64_word h11 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = detail::mul128(lhs.d[1], rhs.d[1], &h11);
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l11, &res.d[2]);
    res.d[3] = c + h11;
    return res;
}

template <>
inline u256 mul(u192 lhs, u128 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word l20 = 0;
    u64_word l21 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h10 = 0;
    u64_word h11 = 0;
    u64_word h20 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = detail::mul128(lhs.d[1], rhs.d[1], &h11);
    l20 = detail::mul128(lhs.d[2], rhs.d[0], &h20);
    l21 = lhs.d[2] * rhs.d[1];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l11, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l20, &res.d[2]);
    res.d[3] = c + h11 + h20 + l21;
    return res;
}

template <>
inline u256 mul(u256 lhs, u128 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word l20 = 0;
    u64_word l21 = 0;
    u64_word l30 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h10 = 0;
    u64_word h11 = 0;
    u64_word h20 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = detail::mul128(lhs.d[1], rhs.d[1], &h11);
    l20 = detail::mul128(lhs.d[2], rhs.d[0], &h20);
    l21 = lhs.d[2] * rhs.d[1];
    l30 = lhs.d[3] * rhs.d[0];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l11, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l20, &res.d[2]);
    res.d[3] = c + h11 + h20 + l21 + l30;
    return res;
}

template <>
inline u256 mul(u64 lhs, u192 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h02 = 0;
    l00 = detail::mul128(lhs, rhs.d[0], &h00);
    l01 = detail::mul128(lhs, rhs.d[1], &h01);
    l02 = detail::mul128(lhs, rhs.d[2], &h02);
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l02, &res.d[2]);
    res.d[3] = c + h02;
    return res;
}

template <>
inline u256 mul(u128 lhs, u192 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word l12 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h02 = 0;
    u64_word h10 = 0;
    u64_word h11 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l02 = detail::mul128(lhs.d[0], rhs.d[2], &h02);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = detail::mul128(lhs.d[1], rhs.d[1], &h11);
    l12 = lhs.d[1] * rhs.d[2];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l02, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l11, &res.d[2]);
    res.d[3] = c + h02 + h11 + l12;
    return res;
}

template <>
inline u256 mul(u192 lhs, u192 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word l12 = 0;
    u64_word l20 = 0;
    u64_word l21 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h02 = 0;
    u64_word h10 = 0;
    u64_word h11 = 0;
    u64_word h20 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l02 = detail::mul128(lhs.d[0], rhs.d[2], &h02);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = detail::mul128(lhs.d[1], rhs.d[1], &h11);
    l20 = detail::mul128(lhs.d[2], rhs.d[0], &h20);
    l12 = lhs.d[1] * rhs.d[2];
    l21 = lhs.d[2] * rhs.d[1];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l02, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l11, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l20, &res.d[2]);
    res.d[3] = c + h02 + h11 + l12 + h20 + l21;
    return res;
}

template <>
inline u256 mul(u256 lhs, u192 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word l12 = 0;
    u64_word l20 = 0;
    u64_word l21 = 0;
    u64_word l30 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h02 = 0;
    u64_word h10 = 0;
    u64_word h11 = 0;
    u64_word h20 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l02 = detail::mul128(lhs.d[0], rhs.d[2], &h02);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = detail::mul128(lhs.d[1], rhs.d[1], &h11);
    l20 = detail::mul128(lhs.d[2], rhs.d[0], &h20);
    l12 = lhs.d[1] * rhs.d[2];
    l21 = lhs.d[2] * rhs.d[1];
    l30 = lhs.d[3] * rhs.d[0];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l02, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l11, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l20, &res.d[2]);
    res.d[3] = c + h02 + h11 + l12 + h20 + l21 + l30;
    return res;
}

template <>
inline u256 mul(u64 lhs, u256 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word l03 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h02 = 0;
    l00 = detail::mul128(lhs, rhs.d[0], &h00);
    l01 = detail::mul128(lhs, rhs.d[1], &h01);
    l02 = detail::mul128(lhs, rhs.d[2], &h02);
    l03 = lhs * rhs.d[3];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l02, &res.d[2]);
    res.d[3] = c + h02 + l03;
    return res;
}

template <>
inline u256 mul(u128 lhs, u256 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word l03 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word l12 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h02 = 0;
    u64_word h10 = 0;
    u64_word h11 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l02 = detail::mul128(lhs.d[0], rhs.d[2], &h02);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = detail::mul128(lhs.d[1], rhs.d[1], &h11);
    l03 = lhs.d[0] * rhs.d[3];
    l12 = lhs.d[1] * rhs.d[2];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l02, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l11, &res.d[2]);
    res.d[3] = c + h02 + l03 + h11 + l12;
    return res;
}

template <>
inline u256 mul(u192 lhs, u256 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word l03 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word l12 = 0;
    u64_word l20 = 0;
    u64_word l21 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h02 = 0;
    u64_word h10 = 0;
    u64_word h11 = 0;
    u64_word h20 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l02 = detail::mul128(lhs.d[0], rhs.d[2], &h02);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = detail::mul128(lhs.d[1], rhs.d[1], &h11);
    l20 = detail::mul128(lhs.d[2], rhs.d[0], &h20);
    l03 = lhs.d[0] * rhs.d[3];
    l12 = lhs.d[1] * rhs.d[2];
    l21 = lhs.d[2] * rhs.d[1];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l02, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l11, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l20, &res.d[2]);
    res.d[3] = c + h02 + l03 + h11 + l12 + h20 + l21;
    return res;
}

template <>
inline u256 mul(u256 lhs, u256 rhs)
{
    u256 res;
    u64_word l00 = 0;
    u64_word l01 = 0;
    u64_word l02 = 0;
    u64_word l03 = 0;
    u64_word l10 = 0;
    u64_word l11 = 0;
    u64_word l12 = 0;
    u64_word l20 = 0;
    u64_word l21 = 0;
    u64_word l30 = 0;
    u64_word h00 = 0;
    u64_word h01 = 0;
    u64_word h02 = 0;
    u64_word h10 = 0;
    u64_word h11 = 0;
    u64_word h20 = 0;
    l00 = detail::mul128(lhs.d[0], rhs.d[0], &h00);
    l01 = detail::mul128(lhs.d[0], rhs.d[1], &h01);
    l02 = detail::mul128(lhs.d[0], rhs.d[2], &h02);
    l10 = detail::mul128(lhs.d[1], rhs.d[0], &h10);
    l11 = detail::mul128(lhs.d[1], rhs.d[1], &h11);
    l20 = detail::mul128(lhs.d[2], rhs.d[0], &h20);
    l03 = lhs.d[0] * rhs.d[3];
    l12 = lhs.d[1] * rhs.d[2];
    l21 = lhs.d[2] * rhs.d[1];
    l30 = lhs.d[3] * rhs.d[0];
    unsigned char c = 0;
    c += detail::add_with_carry(0, res.d[0], l00, &res.d[0]);
    c += detail::add_with_carry(0, res.d[1], c, &res.d[1]);
    c = 0;
    c += detail::add_with_carry(0, res.d[1], h00, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l01, &res.d[1]);
    c += detail::add_with_carry(0, res.d[1], l10, &res.d[1]);
    c += detail::add_with_carry(0, res.d[2], c, &res.d[2]);
    c = 0;
    c += detail::add_with_carry(0, res.d[2], h01, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l02, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], h10, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l11, &res.d[2]);
    c += detail::add_with_carry(0, res.d[2], l20, &res.d[2]);
    res.d[3] = c + h02 + l03 + h11 + l12 + h20 + l21 + l30;
    return res;
}

} // namespace tg::detail
