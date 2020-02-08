#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../detail/comp_traits.hh"
#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "fwd.hh"

namespace tg
{
template <int D, class ScalarT>
struct vec;

// Common vector types

using vec1 = vec<1, f32>;
using vec2 = vec<2, f32>;
using vec3 = vec<3, f32>;
using vec4 = vec<4, f32>;

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


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct vec<1, ScalarT>
{
    ScalarT x = ScalarT(0);

    static const vec zero;
    static const vec one;
    static const vec unit_x;

    TG_DECLARE_COMP_TYPE_1(vec);
};

template <class ScalarT>
struct vec<2, ScalarT>
{
    ScalarT x = ScalarT(0);
    ScalarT y = ScalarT(0);

    static const vec zero;
    static const vec one;
    static const vec unit_x;
    static const vec unit_y;

    TG_DECLARE_COMP_TYPE_2(vec);
};

template <class ScalarT>
struct vec<3, ScalarT>
{
    ScalarT x = ScalarT(0);
    ScalarT y = ScalarT(0);
    ScalarT z = ScalarT(0);

    static const vec zero;
    static const vec one;
    static const vec unit_x;
    static const vec unit_y;
    static const vec unit_z;

    TG_DECLARE_COMP_TYPE_3(vec);
};

template <class ScalarT>
struct vec<4, ScalarT>
{
    ScalarT x = ScalarT(0);
    ScalarT y = ScalarT(0);
    ScalarT z = ScalarT(0);
    ScalarT w = ScalarT(0);

    static const vec zero;
    static const vec one;
    static const vec unit_x;
    static const vec unit_y;
    static const vec unit_z;
    static const vec unit_w;

    TG_DECLARE_COMP_TYPE_4(vec);
};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(vec, vec, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(vec, vec, bool, operator!=, ||, !=);

// deduction guides
TG_IMPL_COMP_DEDUCTION_GUIDES(vec);

// reflection
TG_IMPL_COMP_INTROSPECT(vec);

} // namespace tg
