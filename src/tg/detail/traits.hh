#pragma once

#include "types.hh"

namespace tg
{
#define TG_DEFINE_BOOL_TRAIT(trait, default_val)   \
    template <class T>                             \
    struct trait##_t                               \
    {                                              \
        static constexpr bool value = default_val; \
    };                                             \
    template <class T>                             \
    constexpr bool trait = trait##_t<T>::value

#define TG_ADD_BOOL_TRAIT(trait, type, val) \
    template <>                             \
    struct trait##_t<type>                  \
    {                                       \
        static constexpr bool value = val;  \
    }

// shape traits
TG_DEFINE_BOOL_TRAIT(is_scalar, false);
TG_DEFINE_BOOL_TRAIT(is_vector, false);
TG_DEFINE_BOOL_TRAIT(is_matrix, false);
TG_DEFINE_BOOL_TRAIT(is_quaternion, false);

// field traits
TG_DEFINE_BOOL_TRAIT(is_integer, false);
TG_DEFINE_BOOL_TRAIT(is_floating_point, false);
TG_DEFINE_BOOL_TRAIT(is_signed_integer, false);
TG_DEFINE_BOOL_TRAIT(is_unsigned_integer, false);

// scalars
TG_ADD_BOOL_TRAIT(is_scalar, i8, true);
TG_ADD_BOOL_TRAIT(is_scalar, i16, true);
TG_ADD_BOOL_TRAIT(is_scalar, i32, true);
TG_ADD_BOOL_TRAIT(is_scalar, i64, true);

TG_ADD_BOOL_TRAIT(is_scalar, u8, true);
TG_ADD_BOOL_TRAIT(is_scalar, u16, true);
TG_ADD_BOOL_TRAIT(is_scalar, u32, true);
TG_ADD_BOOL_TRAIT(is_scalar, u64, true);

TG_ADD_BOOL_TRAIT(is_scalar, f8, true);
TG_ADD_BOOL_TRAIT(is_scalar, f16, true);
TG_ADD_BOOL_TRAIT(is_scalar, f32, true);
TG_ADD_BOOL_TRAIT(is_scalar, f64, true);

TG_ADD_BOOL_TRAIT(is_integer, i8, true);
TG_ADD_BOOL_TRAIT(is_integer, i16, true);
TG_ADD_BOOL_TRAIT(is_integer, i32, true);
TG_ADD_BOOL_TRAIT(is_integer, i64, true);

TG_ADD_BOOL_TRAIT(is_signed_integer, i8, true);
TG_ADD_BOOL_TRAIT(is_signed_integer, i16, true);
TG_ADD_BOOL_TRAIT(is_signed_integer, i32, true);
TG_ADD_BOOL_TRAIT(is_signed_integer, i64, true);

TG_ADD_BOOL_TRAIT(is_integer, u8, true);
TG_ADD_BOOL_TRAIT(is_integer, u16, true);
TG_ADD_BOOL_TRAIT(is_integer, u32, true);
TG_ADD_BOOL_TRAIT(is_integer, u64, true);

TG_ADD_BOOL_TRAIT(is_unsigned_integer, u8, true);
TG_ADD_BOOL_TRAIT(is_unsigned_integer, u16, true);
TG_ADD_BOOL_TRAIT(is_unsigned_integer, u32, true);
TG_ADD_BOOL_TRAIT(is_unsigned_integer, u64, true);

TG_ADD_BOOL_TRAIT(is_floating_point, f8, true);
TG_ADD_BOOL_TRAIT(is_floating_point, f16, true);
TG_ADD_BOOL_TRAIT(is_floating_point, f32, true);
TG_ADD_BOOL_TRAIT(is_floating_point, f64, true);

// vectors
template <int D, class ScalarT>
struct is_vector_t<vec<D, ScalarT>>
{
    static constexpr bool value = true;
};
template <int D, class ScalarT>
struct is_integer_t<vec<D, ScalarT>>
{
    static constexpr bool value = is_integer<ScalarT>;
};
template <int D, class ScalarT>
struct is_signed_integer_t<vec<D, ScalarT>>
{
    static constexpr bool value = is_signed_integer<ScalarT>;
};
template <int D, class ScalarT>
struct is_unsigned_integer_t<vec<D, ScalarT>>
{
    static constexpr bool value = is_unsigned_integer<ScalarT>;
};
template <int D, class ScalarT>
struct is_floating_point_t<vec<D, ScalarT>>
{
    static constexpr bool value = is_floating_point<ScalarT>;
};

#undef TG_DEFINE_BOOL_TRAIT
#undef TG_ADD_BOOL_TRAIT
} // namespace tg
