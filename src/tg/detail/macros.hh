#pragma once

// ================= OPERATORS =================



// ================= TRAITS =================

#define TG_DEFINE_BOOL_TRAIT(trait, default_val)   \
    template <class T>                             \
    struct trait##_t                               \
    {                                              \
        static constexpr bool value = default_val; \
    };                                             \
    template <class T>                             \
    constexpr bool trait = trait##_t<T>::value // enforce ;

#define TG_DEFINE_TYPE_TRAIT(trait, default_type) \
    template <class T>                            \
    struct trait##_t                              \
    {                                             \
        using type = default_type;                \
    };                                            \
    template <class T>                            \
    using trait = typename trait##_t<T>::type // enforce ;

#define TG_ADD_BOOL_TRAIT(trait, type, val) \
    template <>                             \
    struct trait##_t<type>                  \
    {                                       \
        static constexpr bool value = val;  \
    } // enforce ;
#define TG_ADD_TYPE_TRAIT(trait, ttype, result_type) \
    template <>                                      \
    struct trait##_t<ttype>                          \
    {                                                \
        using type = result_type;                    \
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
