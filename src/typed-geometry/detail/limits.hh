#pragma once

#include "../types/scalar.hh"

namespace tg
{
namespace detail
{
template <class T>
struct limits
{
    static constexpr T min(); ///< smallest finite element (NOTE: different from numeric_limits<T>::min)
    static constexpr T max(); ///< biggest finite element
};

template <>
struct limits<i8>
{
    static constexpr i8 min() { return i8(-128); }
    static constexpr i8 max() { return i8(127); }
};
template <>
struct limits<i16>
{
    static constexpr i16 min() { return i16(-32768); }
    static constexpr i16 max() { return i16(32767); }
};
template <>
struct limits<i32>
{
    static constexpr i32 min() { return i32(-2147483647 - 1); }
    static constexpr i32 max() { return i32(2147483647); }
};
template <>
struct limits<i64>
{
    static constexpr i64 min() { return i64(-9223372036854775807LL - 1); }
    static constexpr i64 max() { return i64(9223372036854775807LL); }
};

template <>
struct limits<u8>
{
    static constexpr u8 min() { return u8(0); }
    static constexpr u8 max() { return u8(255); }
};
template <>
struct limits<u16>
{
    static constexpr u16 min() { return u16(0); }
    static constexpr u16 max() { return u16(65535); }
};
template <>
struct limits<u32>
{
    static constexpr u32 min() { return u32(0); }
    static constexpr u32 max() { return u32(4294967295); }
};
template <>
struct limits<u64>
{
    static constexpr u64 min() { return u64(0); }
    static constexpr u64 max() { return u64(18446744073709551615uLL); }
};

template <>
struct limits<f8>
{
    static constexpr f8 min() { return f8(0.0f); }
    static constexpr f8 max() { return f8(1.0f); }
};
template <>
struct limits<f16>
{
    // TODO
    // static constexpr f16 min() { return f16(-32768); }
    // static constexpr f16 max() { return f16(32767); }
};
template <>
struct limits<f32>
{
    static constexpr f32 min() { return f32(-3.402823466e+38F); }
    static constexpr f32 max() { return f32(+3.402823466e+38F); }
};
template <>
struct limits<f64>
{
    static constexpr f64 min() { return f64(-1.7976931348623158e+308); }
    static constexpr f64 max() { return f64(+1.7976931348623158e+308); }
};

} // namespace detail
} // namespace tg
