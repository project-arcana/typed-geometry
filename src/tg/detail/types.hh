#pragma once

#include <cstdint>

#include "half.hh"
#include "quarter.hh"
#include "vec.hh"

namespace tg
{
// Scalar types

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f8 = quarter;
using f16 = half;
using f32 = float;
using f64 = double;


// Common vector types

using vec1 = vec<1, f32>;
using vec2 = vec<2, f32>;
using vec3 = vec<3, f32>;
using vec4 = vec<4, f32>;

using bvec1 = vec<1, bool>;
using bvec2 = vec<2, bool>;
using bvec3 = vec<3, bool>;
using bvec4 = vec<4, bool>;

using fvec1 = vec<1, f32>;
using fvec2 = vec<2, f32>;
using fvec3 = vec<3, f32>;
using fvec4 = vec<4, f32>;

using dvec1 = vec<1, f64>;
using dvec2 = vec<2, f64>;
using dvec3 = vec<3, f64>;
using dvec4 = vec<4, f64>;

using ivec1 = vec<1, i32>;
using ivec2 = vec<2, i32>;
using ivec3 = vec<3, i32>;
using ivec4 = vec<4, i32>;

using uvec1 = vec<1, u32>;
using uvec2 = vec<2, u32>;
using uvec3 = vec<3, u32>;
using uvec4 = vec<4, u32>;


// Sized vector types

using i8vec1 = vec<1, i8>;
using i8vec2 = vec<2, i8>;
using i8vec3 = vec<3, i8>;
using i8vec4 = vec<4, i8>;

using i16vec1 = vec<1, i16>;
using i16vec2 = vec<2, i16>;
using i16vec3 = vec<3, i16>;
using i16vec4 = vec<4, i16>;

using i32vec1 = vec<1, i32>;
using i32vec2 = vec<2, i32>;
using i32vec3 = vec<3, i32>;
using i32vec4 = vec<4, i32>;

using i64vec1 = vec<1, i64>;
using i64vec2 = vec<2, i64>;
using i64vec3 = vec<3, i64>;
using i64vec4 = vec<4, i64>;

using u8vec1 = vec<1, u8>;
using u8vec2 = vec<2, u8>;
using u8vec3 = vec<3, u8>;
using u8vec4 = vec<4, u8>;

using u16vec1 = vec<1, u16>;
using u16vec2 = vec<2, u16>;
using u16vec3 = vec<3, u16>;
using u16vec4 = vec<4, u16>;

using u32vec1 = vec<1, u32>;
using u32vec2 = vec<2, u32>;
using u32vec3 = vec<3, u32>;
using u32vec4 = vec<4, u32>;

using u64vec1 = vec<1, u64>;
using u64vec2 = vec<2, u64>;
using u64vec3 = vec<3, u64>;
using u64vec4 = vec<4, u64>;

using f8vec1 = vec<1, f8>;
using f8vec2 = vec<2, f8>;
using f8vec3 = vec<3, f8>;
using f8vec4 = vec<4, f8>;

using f16vec1 = vec<1, f16>;
using f16vec2 = vec<2, f16>;
using f16vec3 = vec<3, f16>;
using f16vec4 = vec<4, f16>;

using f32vec1 = vec<1, f32>;
using f32vec2 = vec<2, f32>;
using f32vec3 = vec<3, f32>;
using f32vec4 = vec<4, f32>;

using f64vec1 = vec<1, f64>;
using f64vec2 = vec<2, f64>;
using f64vec3 = vec<3, f64>;
using f64vec4 = vec<4, f64>;


// Quaternion types

// TODO


// Matrix types

// TODO

} // namespace tg
