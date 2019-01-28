#pragma once

#include "../types/types.hh"
#include "utility.hh"

namespace tg
{
namespace detail
{
using unused = int;
}

#define TG_DEFINE_TRAIT(trait, result_type, default_val)  \
    template <class T>                                    \
    struct trait##_t                                      \
    {                                                     \
        static constexpr result_type value = default_val; \
    };                                                    \
    template <class T>                                    \
    constexpr result_type trait = trait##_t<T>::value // enforce ;

#define TG_DEFINE_BINARY_TRAIT(trait, result_type, default_val) \
    template <class A, class B>                                 \
    struct trait##_t                                            \
    {                                                           \
        static constexpr result_type value = default_val;       \
    };                                                          \
    template <class A, class B>                                 \
    constexpr result_type trait = trait##_t<A, B>::value // enforce ;

#define TG_DEFINE_TYPE_TRAIT(trait, default_type) \
    template <class T>                            \
    struct trait##_t                              \
    {                                             \
        using type = default_type;                \
    };                                            \
    template <class T>                            \
    using trait = typename trait##_t<T>::type // enforce ;

#define TG_DEFINE_BINARY_TYPE_TRAIT(trait, default_type) \
    template <class A, class B>                          \
    struct trait##_t                                     \
    {                                                    \
        using type = default_type;                       \
    };                                                   \
    template <class A, class B>                          \
    using trait = typename trait##_t<A, B>::type // enforce ;

#define TG_ADD_TRAIT(trait, result_type, type, val) \
    template <>                                     \
    struct trait##_t<type>                          \
    {                                               \
        static constexpr result_type value = val;   \
    } // enforce ;
#define TG_ADD_BINARY_TRAIT(trait, result_type, typeA, typeB, val) \
    template <>                                                    \
    struct trait##_t<typeA, typeB>                                 \
    {                                                              \
        static constexpr result_type value = val;                  \
    } // enforce ;
#define TG_ADD_TYPE_TRAIT(trait, ttype, result_type) \
    template <>                                      \
    struct trait##_t<ttype>                          \
    {                                                \
        using type = result_type;                    \
    } // enforce ;
#define TG_ADD_BINARY_TYPE_TRAIT(trait, ttypeA, ttypeB, result_type) \
    template <>                                                      \
    struct trait##_t<ttypeA, ttypeB>                                 \
    {                                                                \
        using type = result_type;                                    \
    } // enforce ;
#define TG_ADD_SCALAR_TYPE(basetype, bits, rtype) \
    template <>                                   \
    struct scalar_t<basetype, bits>               \
    {                                             \
        using type = rtype;                       \
    } // enforce ;

#define TG_INHERIT_TRAITS_D(ttype)                                  \
    template <int D, class ScalarT>                                 \
    struct is_integer_t<ttype<D, ScalarT>>                          \
    {                                                               \
        static constexpr bool value = is_integer<ScalarT>;          \
    };                                                              \
    template <int D, class ScalarT>                                 \
    struct is_signed_integer_t<ttype<D, ScalarT>>                   \
    {                                                               \
        static constexpr bool value = is_signed_integer<ScalarT>;   \
    };                                                              \
    template <int D, class ScalarT>                                 \
    struct is_unsigned_integer_t<ttype<D, ScalarT>>                 \
    {                                                               \
        static constexpr bool value = is_unsigned_integer<ScalarT>; \
    };                                                              \
    template <int D, class ScalarT>                                 \
    struct is_floating_point_t<ttype<D, ScalarT>>                   \
    {                                                               \
        static constexpr bool value = is_floating_point<ScalarT>;   \
    };                                                              \
    template <int D, class ScalarT>                                 \
    struct fractional_result_t<ttype<D, ScalarT>>                   \
    {                                                               \
        using type = ttype<D, fractional_result<ScalarT>>;          \
    } // enforce ;


#define TG_INHERIT_TRAITS_FIELD_D(ttype)                           \
    TG_INHERIT_TRAITS_D(ttype);                                    \
    template <int D, class ScalarT>                                \
    struct has_multiplication_t<ttype<D, ScalarT>>                 \
    {                                                              \
        static constexpr bool value = has_multiplication<ScalarT>; \
    }; // enforce ;

// scalar traits
template <class Base, int Bits>
struct scalar_t
{
    using type = void;
};
template <class Base, int Bits>
using scalar = typename scalar_t<Base, Bits>::type;
TG_DEFINE_TRAIT(scalar_bit_width, int, 0);
TG_DEFINE_TYPE_TRAIT(scalar_base_type, void);

// name traits
TG_DEFINE_TRAIT(type_name_prefix, char const*, "");
TG_DEFINE_TRAIT(type_name, char const*, "");
TG_DEFINE_TRAIT(type_name_suffix, char const*, "");

// shape traits
TG_DEFINE_TRAIT(is_scalar, bool, false);
TG_DEFINE_TRAIT(is_vector, bool, false);
TG_DEFINE_TRAIT(is_matrix, bool, false);
TG_DEFINE_TRAIT(is_quaternion, bool, false);

// field traits
TG_DEFINE_TRAIT(is_integer, bool, false);
TG_DEFINE_TRAIT(is_floating_point, bool, false);
TG_DEFINE_TRAIT(is_signed_integer, bool, false);
TG_DEFINE_TRAIT(is_unsigned_integer, bool, false);
TG_DEFINE_TRAIT(has_multiplication, bool, true);

// numbers
template <int D>
struct type_name_number_t
{
    static constexpr char const* value = "";
};
template <>
struct type_name_number_t<1>
{
    static constexpr char const* value = "1";
};
template <>
struct type_name_number_t<2>
{
    static constexpr char const* value = "2";
};
template <>
struct type_name_number_t<3>
{
    static constexpr char const* value = "3";
};
template <>
struct type_name_number_t<4>
{
    static constexpr char const* value = "4";
};
template <int D>
constexpr char const* type_name_number = type_name_number_t<D>::value;

// scalars
TG_ADD_SCALAR_TYPE(int, 8, i8);
TG_ADD_SCALAR_TYPE(int, 16, i16);
TG_ADD_SCALAR_TYPE(int, 32, i32);
TG_ADD_SCALAR_TYPE(int, 64, i64);

TG_ADD_SCALAR_TYPE(unsigned, 8, u8);
TG_ADD_SCALAR_TYPE(unsigned, 16, u16);
TG_ADD_SCALAR_TYPE(unsigned, 32, u32);
TG_ADD_SCALAR_TYPE(unsigned, 64, u64);

TG_ADD_SCALAR_TYPE(float, 8, f8);
TG_ADD_SCALAR_TYPE(float, 16, f16);
TG_ADD_SCALAR_TYPE(float, 32, f32);
TG_ADD_SCALAR_TYPE(float, 64, f64);

TG_ADD_TRAIT(scalar_bit_width, int, i8, 8);
TG_ADD_TRAIT(scalar_bit_width, int, i16, 16);
TG_ADD_TRAIT(scalar_bit_width, int, i32, 32);
TG_ADD_TRAIT(scalar_bit_width, int, i64, 64);

TG_ADD_TRAIT(scalar_bit_width, int, u8, 8);
TG_ADD_TRAIT(scalar_bit_width, int, u16, 16);
TG_ADD_TRAIT(scalar_bit_width, int, u32, 32);
TG_ADD_TRAIT(scalar_bit_width, int, u64, 64);

TG_ADD_TRAIT(scalar_bit_width, int, f8, 8);
TG_ADD_TRAIT(scalar_bit_width, int, f16, 16);
TG_ADD_TRAIT(scalar_bit_width, int, f32, 32);
TG_ADD_TRAIT(scalar_bit_width, int, f64, 64);

TG_ADD_TYPE_TRAIT(scalar_base_type, i8, int);
TG_ADD_TYPE_TRAIT(scalar_base_type, i16, int);
TG_ADD_TYPE_TRAIT(scalar_base_type, i32, int);
TG_ADD_TYPE_TRAIT(scalar_base_type, i64, int);

TG_ADD_TYPE_TRAIT(scalar_base_type, u8, unsigned);
TG_ADD_TYPE_TRAIT(scalar_base_type, u16, unsigned);
TG_ADD_TYPE_TRAIT(scalar_base_type, u32, unsigned);
TG_ADD_TYPE_TRAIT(scalar_base_type, u64, unsigned);

TG_ADD_TYPE_TRAIT(scalar_base_type, f8, float);
TG_ADD_TYPE_TRAIT(scalar_base_type, f16, float);
TG_ADD_TYPE_TRAIT(scalar_base_type, f32, float);
TG_ADD_TYPE_TRAIT(scalar_base_type, f64, float);

TG_ADD_TRAIT(type_name, char const*, bool, "b");

TG_ADD_TRAIT(type_name, char const*, i8, "i8");
TG_ADD_TRAIT(type_name, char const*, i16, "i16");
TG_ADD_TRAIT(type_name, char const*, i32, "i32");
TG_ADD_TRAIT(type_name, char const*, i64, "i64");

TG_ADD_TRAIT(type_name, char const*, u8, "u8");
TG_ADD_TRAIT(type_name, char const*, u16, "u16");
TG_ADD_TRAIT(type_name, char const*, u32, "u32");
TG_ADD_TRAIT(type_name, char const*, u64, "u64");

TG_ADD_TRAIT(type_name, char const*, f8, "f8");
TG_ADD_TRAIT(type_name, char const*, f16, "f16");
TG_ADD_TRAIT(type_name, char const*, f32, "f32");
TG_ADD_TRAIT(type_name, char const*, f64, "f64");

TG_ADD_TRAIT(is_scalar, bool, i8, true);
TG_ADD_TRAIT(is_scalar, bool, i16, true);
TG_ADD_TRAIT(is_scalar, bool, i32, true);
TG_ADD_TRAIT(is_scalar, bool, i64, true);

TG_ADD_TRAIT(is_scalar, bool, u8, true);
TG_ADD_TRAIT(is_scalar, bool, u16, true);
TG_ADD_TRAIT(is_scalar, bool, u32, true);
TG_ADD_TRAIT(is_scalar, bool, u64, true);

TG_ADD_TRAIT(is_scalar, bool, f8, true);
TG_ADD_TRAIT(is_scalar, bool, f16, true);
TG_ADD_TRAIT(is_scalar, bool, f32, true);
TG_ADD_TRAIT(is_scalar, bool, f64, true);

TG_ADD_TRAIT(is_integer, bool, i8, true);
TG_ADD_TRAIT(is_integer, bool, i16, true);
TG_ADD_TRAIT(is_integer, bool, i32, true);
TG_ADD_TRAIT(is_integer, bool, i64, true);

TG_ADD_TRAIT(is_signed_integer, bool, i8, true);
TG_ADD_TRAIT(is_signed_integer, bool, i16, true);
TG_ADD_TRAIT(is_signed_integer, bool, i32, true);
TG_ADD_TRAIT(is_signed_integer, bool, i64, true);

TG_ADD_TRAIT(is_integer, bool, u8, true);
TG_ADD_TRAIT(is_integer, bool, u16, true);
TG_ADD_TRAIT(is_integer, bool, u32, true);
TG_ADD_TRAIT(is_integer, bool, u64, true);

TG_ADD_TRAIT(is_unsigned_integer, bool, u8, true);
TG_ADD_TRAIT(is_unsigned_integer, bool, u16, true);
TG_ADD_TRAIT(is_unsigned_integer, bool, u32, true);
TG_ADD_TRAIT(is_unsigned_integer, bool, u64, true);

TG_ADD_TRAIT(is_floating_point, bool, f8, true);
TG_ADD_TRAIT(is_floating_point, bool, f16, true);
TG_ADD_TRAIT(is_floating_point, bool, f32, true);
TG_ADD_TRAIT(is_floating_point, bool, f64, true);

TG_ADD_TRAIT(has_multiplication, bool, bool, false);

// type mapping
TG_DEFINE_TYPE_TRAIT(fractional_result, f32);
TG_ADD_TYPE_TRAIT(fractional_result, f64, f64);
TG_ADD_TYPE_TRAIT(fractional_result, i64, f64);
TG_ADD_TYPE_TRAIT(fractional_result, u64, f64);

TG_DEFINE_TYPE_TRAIT(integer_result, i32);
TG_ADD_TYPE_TRAIT(integer_result, f64, i64);
TG_ADD_TYPE_TRAIT(integer_result, i64, i64);
TG_ADD_TYPE_TRAIT(integer_result, u8, u8);
TG_ADD_TYPE_TRAIT(integer_result, u16, u16);
TG_ADD_TYPE_TRAIT(integer_result, u32, u32);
TG_ADD_TYPE_TRAIT(integer_result, u64, u64);

TG_DEFINE_TYPE_TRAIT(squared_result, void);

TG_ADD_TYPE_TRAIT(squared_result, i8, i16);
TG_ADD_TYPE_TRAIT(squared_result, i16, i32);
TG_ADD_TYPE_TRAIT(squared_result, i32, i64);
TG_ADD_TYPE_TRAIT(squared_result, i64, i64);

TG_ADD_TYPE_TRAIT(squared_result, u8, u16);
TG_ADD_TYPE_TRAIT(squared_result, u16, u32);
TG_ADD_TYPE_TRAIT(squared_result, u32, u64);
TG_ADD_TYPE_TRAIT(squared_result, u64, u64);

TG_ADD_TYPE_TRAIT(squared_result, f8, f32);
TG_ADD_TYPE_TRAIT(squared_result, f16, f32);
TG_ADD_TYPE_TRAIT(squared_result, f32, f32);
TG_ADD_TYPE_TRAIT(squared_result, f64, f64);

// promotions
TG_DEFINE_BINARY_TYPE_TRAIT(promoted_scalar_base, void);
TG_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, int, int, int);
TG_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, int, unsigned, int);
TG_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, int, float, float);
TG_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, unsigned, int, int);
TG_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, unsigned, unsigned, unsigned);
TG_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, unsigned, float, float);
TG_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, float, int, float);
TG_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, float, unsigned, float);
TG_ADD_BINARY_TYPE_TRAIT(promoted_scalar_base, float, float, float);

template <class A, class B>
struct promoted_scalar_t
{
    static constexpr int bitmax(int a, int b) { return a > b ? a : b; }
    using type = scalar<promoted_scalar_base<A, B>, bitmax(scalar_bit_width<A>, scalar_bit_width<B>)>;
};
template <class A, class B>
using promoted_scalar = typename promoted_scalar_t<A, B>::type;


// vectors
template <int D, class ScalarT>
struct is_vector_t<vec<D, ScalarT>>
{
    static constexpr bool value = true;
};
template <int D, class ScalarT, template <int, class> class Type>
struct type_name_prefix_t<Type<D, ScalarT>>
{
    static constexpr char const* value = type_name<ScalarT>;
};
template <int D, template <int, class> class Type>
struct type_name_prefix_t<Type<D, f32>>
{
    static constexpr char const* value = "";
};
template <int D, template <int, class> class Type>
struct type_name_prefix_t<Type<D, f64>>
{
    static constexpr char const* value = "d";
};
template <int D, template <int, class> class Type>
struct type_name_prefix_t<Type<D, i32>>
{
    static constexpr char const* value = "i";
};
template <int D, template <int, class> class Type>
struct type_name_prefix_t<Type<D, u32>>
{
    static constexpr char const* value = "u";
};
template <int D, class ScalarT>
struct type_name_t<vec<D, ScalarT>>
{
    static constexpr char const* value = "vec";
};
template <int D, class ScalarT>
struct type_name_t<pos<D, ScalarT>>
{
    static constexpr char const* value = "pos";
};
template <int D, class ScalarT>
struct type_name_t<size<D, ScalarT>>
{
    static constexpr char const* value = "size";
};
template <int D, class ScalarT>
struct type_name_t<box<D, ScalarT>>
{
    static constexpr char const* value = "box";
};
template <int D, class ScalarT>
struct type_name_t<triangle<D, ScalarT>>
{
    static constexpr char const* value = "triangle";
};
template <int D, class ScalarT, template <int, class> class Type>
struct type_name_suffix_t<Type<D, ScalarT>>
{
    static constexpr char const* value = type_name_number<D>;
};
TG_INHERIT_TRAITS_D(vec);

#undef TG_DEFINE_TRAIT
#undef TG_DEFINE_TYPE_TRAIT
#undef TG_ADD_TRAIT
#undef TG_ADD_TYPE_TRAIT
#undef TG_INHERIT_TRAITS_D
#undef TG_INHERIT_TRAITS_FIELDS_D
} // namespace tg
