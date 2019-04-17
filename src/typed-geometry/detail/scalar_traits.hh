#pragma once

#include <typed-geometry/types/scalar.hh>
#include "errors.hh"
#include "macros.hh"
#include "utility.hh"

namespace tg
{
// scalar traits
template <class Base, int Bits>
struct scalar_t
{
    using type = type_error::unsupported_scalar_type<Base, Bits>;
};
template <class Base, int Bits>
using scalar = typename scalar_t<Base, Bits>::type;
TG_IMPL_DEFINE_TRAIT(scalar_bit_width, int, type_error::unknown_scalar_type<T>::value);
TG_IMPL_DEFINE_TYPE_TRAIT(scalar_base_type, type_error::unknown_scalar_type<T>);

// name traits
TG_IMPL_DEFINE_TRAIT(type_name, char const*, type_error::unknown_type<T>::value);
TG_IMPL_DEFINE_TRAIT(type_name_prefix, char const*, type_name<T>);

// field traits
TG_IMPL_DEFINE_TRAIT(is_integer, bool, false);
TG_IMPL_DEFINE_TRAIT(is_floating_point, bool, false);
TG_IMPL_DEFINE_TRAIT(is_signed_integer, bool, false);
TG_IMPL_DEFINE_TRAIT(is_unsigned_integer, bool, false);
TG_IMPL_DEFINE_TRAIT(has_multiplication, bool, true);

// shape traits
TG_IMPL_DEFINE_TRAIT(is_scalar, bool, false);
TG_IMPL_DEFINE_TRAIT(is_vector, bool, false);
TG_IMPL_DEFINE_TRAIT(is_object, bool, false);
TG_IMPL_DEFINE_TRAIT(is_matrix, bool, false);
TG_IMPL_DEFINE_TRAIT(is_quaternion, bool, false);

// scalars
TG_IMPL_ADD_SCALAR_TYPE(int, 8, i8);
TG_IMPL_ADD_SCALAR_TYPE(int, 16, i16);
TG_IMPL_ADD_SCALAR_TYPE(int, 32, i32);
TG_IMPL_ADD_SCALAR_TYPE(int, 64, i64);

TG_IMPL_ADD_SCALAR_TYPE(unsigned, 8, u8);
TG_IMPL_ADD_SCALAR_TYPE(unsigned, 16, u16);
TG_IMPL_ADD_SCALAR_TYPE(unsigned, 32, u32);
TG_IMPL_ADD_SCALAR_TYPE(unsigned, 64, u64);

TG_IMPL_ADD_SCALAR_TYPE(float, 8, f8);
TG_IMPL_ADD_SCALAR_TYPE(float, 16, f16);
TG_IMPL_ADD_SCALAR_TYPE(float, 32, f32);
TG_IMPL_ADD_SCALAR_TYPE(float, 64, f64);

TG_IMPL_ADD_TRAIT(scalar_bit_width, int, i8, 8);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, i16, 16);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, i32, 32);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, i64, 64);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, long, 64);

TG_IMPL_ADD_TRAIT(scalar_bit_width, int, u8, 8);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, u16, 16);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, u32, 32);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, u64, 64);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, unsigned long, 64);

TG_IMPL_ADD_TRAIT(scalar_bit_width, int, f8, 8);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, f16, 16);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, f32, 32);
TG_IMPL_ADD_TRAIT(scalar_bit_width, int, f64, 64);

TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, i8, int);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, i16, int);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, i32, int);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, i64, int);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, long, int);

TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, u8, unsigned);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, u16, unsigned);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, u32, unsigned);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, u64, unsigned);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, unsigned long, unsigned);

TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, f8, float);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, f16, float);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, f32, float);
TG_IMPL_ADD_TYPE_TRAIT(scalar_base_type, f64, float);

TG_IMPL_ADD_TRAIT(type_name, char const*, bool, "b");

TG_IMPL_ADD_TRAIT(type_name, char const*, i8, "i8");
TG_IMPL_ADD_TRAIT(type_name, char const*, i16, "i16");
TG_IMPL_ADD_TRAIT(type_name, char const*, i32, "i32");
TG_IMPL_ADD_TRAIT(type_name, char const*, i64, "i64");

TG_IMPL_ADD_TRAIT(type_name, char const*, u8, "u8");
TG_IMPL_ADD_TRAIT(type_name, char const*, u16, "u16");
TG_IMPL_ADD_TRAIT(type_name, char const*, u32, "u32");
TG_IMPL_ADD_TRAIT(type_name, char const*, u64, "u64");

TG_IMPL_ADD_TRAIT(type_name, char const*, f8, "f8");
TG_IMPL_ADD_TRAIT(type_name, char const*, f16, "f16");
TG_IMPL_ADD_TRAIT(type_name, char const*, f32, "f32");
TG_IMPL_ADD_TRAIT(type_name, char const*, f64, "f64");

TG_IMPL_ADD_TRAIT(is_scalar, bool, i8, true);
TG_IMPL_ADD_TRAIT(is_scalar, bool, i16, true);
TG_IMPL_ADD_TRAIT(is_scalar, bool, i32, true);
TG_IMPL_ADD_TRAIT(is_scalar, bool, i64, true);

TG_IMPL_ADD_TRAIT(is_scalar, bool, u8, true);
TG_IMPL_ADD_TRAIT(is_scalar, bool, u16, true);
TG_IMPL_ADD_TRAIT(is_scalar, bool, u32, true);
TG_IMPL_ADD_TRAIT(is_scalar, bool, u64, true);

TG_IMPL_ADD_TRAIT(is_scalar, bool, f8, true);
TG_IMPL_ADD_TRAIT(is_scalar, bool, f16, true);
TG_IMPL_ADD_TRAIT(is_scalar, bool, f32, true);
TG_IMPL_ADD_TRAIT(is_scalar, bool, f64, true);

TG_IMPL_ADD_TRAIT(is_scalar, bool, long, true);
TG_IMPL_ADD_TRAIT(is_scalar, bool, unsigned long, true);

TG_IMPL_ADD_TRAIT(is_integer, bool, i8, true);
TG_IMPL_ADD_TRAIT(is_integer, bool, i16, true);
TG_IMPL_ADD_TRAIT(is_integer, bool, i32, true);
TG_IMPL_ADD_TRAIT(is_integer, bool, i64, true);
TG_IMPL_ADD_TRAIT(is_integer, bool, long, true);

TG_IMPL_ADD_TRAIT(is_signed_integer, bool, i8, true);
TG_IMPL_ADD_TRAIT(is_signed_integer, bool, i16, true);
TG_IMPL_ADD_TRAIT(is_signed_integer, bool, i32, true);
TG_IMPL_ADD_TRAIT(is_signed_integer, bool, i64, true);
TG_IMPL_ADD_TRAIT(is_signed_integer, bool, long, true);

TG_IMPL_ADD_TRAIT(is_integer, bool, u8, true);
TG_IMPL_ADD_TRAIT(is_integer, bool, u16, true);
TG_IMPL_ADD_TRAIT(is_integer, bool, u32, true);
TG_IMPL_ADD_TRAIT(is_integer, bool, u64, true);
TG_IMPL_ADD_TRAIT(is_integer, bool, unsigned long, true);

TG_IMPL_ADD_TRAIT(is_unsigned_integer, bool, u8, true);
TG_IMPL_ADD_TRAIT(is_unsigned_integer, bool, u16, true);
TG_IMPL_ADD_TRAIT(is_unsigned_integer, bool, u32, true);
TG_IMPL_ADD_TRAIT(is_unsigned_integer, bool, u64, true);
TG_IMPL_ADD_TRAIT(is_unsigned_integer, bool, unsigned long, true);

TG_IMPL_ADD_TRAIT(is_floating_point, bool, f8, true);
TG_IMPL_ADD_TRAIT(is_floating_point, bool, f16, true);
TG_IMPL_ADD_TRAIT(is_floating_point, bool, f32, true);
TG_IMPL_ADD_TRAIT(is_floating_point, bool, f64, true);

TG_IMPL_ADD_TRAIT(has_multiplication, bool, bool, false);

// type mapping
TG_IMPL_DEFINE_TYPE_TRAIT(fractional_result, f32);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, f64, f64);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, i64, f64);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, u64, f64);

TG_IMPL_DEFINE_TYPE_TRAIT(integer_result, i32);
TG_IMPL_ADD_TYPE_TRAIT(integer_result, f64, i64);
TG_IMPL_ADD_TYPE_TRAIT(integer_result, i64, i64);
TG_IMPL_ADD_TYPE_TRAIT(integer_result, u8, u8);
TG_IMPL_ADD_TYPE_TRAIT(integer_result, u16, u16);
TG_IMPL_ADD_TYPE_TRAIT(integer_result, u32, u32);
TG_IMPL_ADD_TYPE_TRAIT(integer_result, u64, u64);

TG_IMPL_DEFINE_TYPE_TRAIT(squared_result, type_error::unknown_scalar_type<T>);

TG_IMPL_ADD_TYPE_TRAIT(squared_result, i8, i16);
TG_IMPL_ADD_TYPE_TRAIT(squared_result, i16, i32);
TG_IMPL_ADD_TYPE_TRAIT(squared_result, i32, i64);
TG_IMPL_ADD_TYPE_TRAIT(squared_result, i64, i64);

TG_IMPL_ADD_TYPE_TRAIT(squared_result, u8, u16);
TG_IMPL_ADD_TYPE_TRAIT(squared_result, u16, u32);
TG_IMPL_ADD_TYPE_TRAIT(squared_result, u32, u64);
TG_IMPL_ADD_TYPE_TRAIT(squared_result, u64, u64);

TG_IMPL_ADD_TYPE_TRAIT(squared_result, f8, f32);
TG_IMPL_ADD_TYPE_TRAIT(squared_result, f16, f32);
TG_IMPL_ADD_TYPE_TRAIT(squared_result, f32, f32);
TG_IMPL_ADD_TYPE_TRAIT(squared_result, f64, f64);

// promotions
TG_IMPL_DEFINE_BINARY_TYPE_TRAIT(promoted_scalar_base, type_error::cannot_promote_types<A, B>);
TG_IMPL_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, int, int, int);
TG_IMPL_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, int, unsigned, int);
TG_IMPL_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, int, float, float);
TG_IMPL_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, unsigned, int, int);
TG_IMPL_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, unsigned, unsigned, unsigned);
TG_IMPL_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, unsigned, float, float);
TG_IMPL_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, float, int, float);
TG_IMPL_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, float, unsigned, float);
TG_IMPL_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, float, float, float);

template <bool, class A, class B>
struct promoted_scalar_t
{
    using type = type_error::cannot_promote_types<A, B>;
};
template <class A, class B>
struct promoted_scalar_t<true, A, B>
{
    static_assert(is_scalar<A> && is_scalar<B>, "only valid for scalars");

    static constexpr int bitmax(int a, int b) { return a > b ? a : b; }

    using type = scalar<promoted_scalar_base<scalar_base_type<A>, scalar_base_type<B>>, bitmax(scalar_bit_width<A>, scalar_bit_width<B>)>;
};
template <class A, class B>
using promoted_scalar = typename promoted_scalar_t<is_scalar<A> && is_scalar<B>, A, B>::type;

// type names
template <>
struct type_name_prefix_t<f32>
{
    static constexpr char const* value = "";
};
template <>
struct type_name_prefix_t<i32>
{
    static constexpr char const* value = "i";
};
template <>
struct type_name_prefix_t<u32>
{
    static constexpr char const* value = "u";
};
template <>
struct type_name_prefix_t<f64>
{
    static constexpr char const* value = "d";
};
} // namespace tg
