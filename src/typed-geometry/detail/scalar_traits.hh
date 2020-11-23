#pragma once

#include <typed-geometry/types/scalars/default.hh>

#include "errors.hh"
#include "macros.hh"
#include "utility.hh"

namespace tg
{
// name traits
TG_IMPL_DEFINE_TRAIT(type_name_prefix, char const*, type_error::unknown_type<T>::value);

// field traits
TG_IMPL_DEFINE_TRAIT(is_integer, bool, false);
TG_IMPL_DEFINE_TRAIT(is_floating_point, bool, false);
TG_IMPL_DEFINE_TRAIT(is_signed_integer, bool, false);
TG_IMPL_DEFINE_TRAIT(is_unsigned_integer, bool, false);

// shape traits
TG_IMPL_DEFINE_TRAIT(is_scalar, bool, false);
TG_IMPL_DEFINE_TRAIT(is_vector, bool, false);
TG_IMPL_DEFINE_TRAIT(is_object, bool, false);
TG_IMPL_DEFINE_TRAIT(is_matrix, bool, false);
TG_IMPL_DEFINE_TRAIT(is_quaternion, bool, false);

// scalars
TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, bool, "b");

TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, i8, "i8");
TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, i16, "i16");
TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, i32, "i");
TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, i64, "i64");

TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, u8, "u8");
TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, u16, "u16");
TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, u32, "u");
TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, u64, "u64");

TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, f8, "f8");
TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, f16, "f16");
TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, f32, ""); // vec3, not fvec3
TG_IMPL_ADD_TRAIT(type_name_prefix, char const*, f64, "d");

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

// type mapping
TG_IMPL_DEFINE_TYPE_TRAIT(fractional_result, T);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, f64, f64);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, i64, f64);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, u64, f64);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, i32, f32);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, u32, f32);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, i16, f32);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, u16, f32);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, i8, f32);
TG_IMPL_ADD_TYPE_TRAIT(fractional_result, u8, f32);

// special scalars
template <class T>
struct angle_t;
template <class T>
struct is_scalar_t<angle_t<T>>
{
    static constexpr bool value = true;
};

// abstract scalars are scalars that don't hold a concrete values (e.g. traced types)
TG_IMPL_DEFINE_TRAIT(is_abstract_scalar, bool, false);

template <class T>
struct make_unsigned_t
{
    using type = std::make_unsigned_t<T>;
};
template <class T>
using make_unsigned = typename make_unsigned_t<T>::type;

template <class T>
struct make_signed_t
{
    using type = std::make_signed_t<T>;
};
template <class T>
using make_signed = typename make_signed_t<T>::type;

} // namespace tg
