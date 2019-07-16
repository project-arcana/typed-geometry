#pragma once

// =========
// configurations

#ifndef NDEBUG
#define TG_DEBUG
#endif

#if defined(_MSC_VER)
#define TG_COMPILER_MSVC
#elif defined(__clang__)
#define TG_COMPILER_CLANG
#elif defined(__GNUC__)
#define TG_COMPILER_GCC
#else
#error "Unknown compiler"
#endif


// =========
// compiler specific builtins

#if defined(TG_COMPILER_MSVC)

#define TG_PRETTY_FUNC __FUNCTION__

#define TG_FORCE_INLINE __forceinline
#define TG_DONT_INLINE __declspec(noinline)

#define TG_LIKELY(x) x
#define TG_UNLIKELY(x) x
#define TG_COLD_FUNC


#elif defined(TG_COMPILER_CLANG) || defined(TG_COMPILER_GCC)

#define TG_PRETTY_FUNC __PRETTY_FUNCTION__

#define TG_FORCE_INLINE __attribute__((always_inline))
#define TG_DONT_INLINE __attribute__((noinline))

#define TG_LIKELY(x) __builtin_expect(bool(x), 1)
#define TG_UNLIKELY(x) __builtin_expect(bool(x), 0)
#define TG_COLD_FUNC __attribute__((cold))

#else
#error "Unknown compiler"
#endif


// =========
// common helper

#define TG_DETAIL_MACRO_JOIN(arg1, arg2) arg1##arg2
#define TG_MACRO_JOIN(arg1, arg2) TG_DETAIL_MACRO_JOIN(arg1, arg2)

#define TG_UNUSED(expr) void(sizeof((expr))) // force ;

#define TG_FORCE_SEMICOLON static_assert(true, "")

// currently a macro because clang-format interacts badly with it
#define TG_NODISCARD [[nodiscard]]

// =========
// operations and classes

#define TG_IMPL_MEMBER_pos_0 x
#define TG_IMPL_MEMBER_pos_1 y
#define TG_IMPL_MEMBER_pos_2 z
#define TG_IMPL_MEMBER_pos_3 w

#define TG_IMPL_MEMBER_vec_0 x
#define TG_IMPL_MEMBER_vec_1 y
#define TG_IMPL_MEMBER_vec_2 z
#define TG_IMPL_MEMBER_vec_3 w

#define TG_IMPL_MEMBER_dir_0 x
#define TG_IMPL_MEMBER_dir_1 y
#define TG_IMPL_MEMBER_dir_2 z
#define TG_IMPL_MEMBER_dir_3 w

#define TG_IMPL_MEMBER_color_0 r
#define TG_IMPL_MEMBER_color_1 g
#define TG_IMPL_MEMBER_color_2 b
#define TG_IMPL_MEMBER_color_3 a

#define TG_IMPL_MEMBER_size_0 width
#define TG_IMPL_MEMBER_size_1 height
#define TG_IMPL_MEMBER_size_2 depth
#define TG_IMPL_MEMBER_size_3 w

#define TG_IMPL_MEMBER_mat_0 m[0]
#define TG_IMPL_MEMBER_mat_1 m[1]
#define TG_IMPL_MEMBER_mat_2 m[2]
#define TG_IMPL_MEMBER_mat_3 m[3]

#define TG_IMPL_MEMBER_comp_0 comp0
#define TG_IMPL_MEMBER_comp_1 comp1
#define TG_IMPL_MEMBER_comp_2 comp2
#define TG_IMPL_MEMBER_comp_3 comp3

#define TG_IMPL_MEMBER(TYPE, COMP) TG_IMPL_MEMBER_##TYPE##_##COMP

#define TG_IMPL_DEFINE_UNARY_OP(TYPE, OP)                                                                                                    \
    template <int D, class ScalarT>                                                                                                          \
    TG_NODISCARD constexpr TYPE<D, ScalarT> operator OP(TYPE<D, ScalarT> const& a)                                                           \
    {                                                                                                                                        \
        if constexpr (D == 1)                                                                                                                \
            return {OP a.TG_IMPL_MEMBER(TYPE, 0)};                                                                                           \
        else if constexpr (D == 2)                                                                                                           \
            return {OP a.TG_IMPL_MEMBER(TYPE, 0), OP a.TG_IMPL_MEMBER(TYPE, 1)};                                                             \
        else if constexpr (D == 3)                                                                                                           \
            return {OP a.TG_IMPL_MEMBER(TYPE, 0), OP a.TG_IMPL_MEMBER(TYPE, 1), OP a.TG_IMPL_MEMBER(TYPE, 2)};                               \
        else if constexpr (D == 4)                                                                                                           \
            return {OP a.TG_IMPL_MEMBER(TYPE, 0), OP a.TG_IMPL_MEMBER(TYPE, 1), OP a.TG_IMPL_MEMBER(TYPE, 2), OP a.TG_IMPL_MEMBER(TYPE, 3)}; \
        else                                                                                                                                 \
            static_assert(always_false<ScalarT>, "only supported up to 4 dimensions");                                                       \
    }                                                                                                                                        \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_CONV_CTOR_IMPL(TYPE_FROM, TYPE_TO)                                                                               \
    template <class ScalarT>                                                                                                            \
    constexpr TYPE_TO<1, ScalarT>::TYPE_TO(TYPE_FROM<1, ScalarT> const& a) : TG_IMPL_MEMBER(TYPE_TO, 0)(a.TG_IMPL_MEMBER(TYPE_FROM, 0)) \
    {                                                                                                                                   \
    }                                                                                                                                   \
    template <class ScalarT>                                                                                                            \
    constexpr TYPE_TO<2, ScalarT>::TYPE_TO(TYPE_FROM<2, ScalarT> const& a)                                                              \
      : TG_IMPL_MEMBER(TYPE_TO, 0)(a.TG_IMPL_MEMBER(TYPE_FROM, 0)), TG_IMPL_MEMBER(TYPE_TO, 1)(a.TG_IMPL_MEMBER(TYPE_FROM, 1))          \
    {                                                                                                                                   \
    }                                                                                                                                   \
    template <class ScalarT>                                                                                                            \
    constexpr TYPE_TO<3, ScalarT>::TYPE_TO(TYPE_FROM<3, ScalarT> const& a)                                                              \
      : TG_IMPL_MEMBER(TYPE_TO, 0)(a.TG_IMPL_MEMBER(TYPE_FROM, 0)),                                                                     \
        TG_IMPL_MEMBER(TYPE_TO, 1)(a.TG_IMPL_MEMBER(TYPE_FROM, 1)),                                                                     \
        TG_IMPL_MEMBER(TYPE_TO, 2)(a.TG_IMPL_MEMBER(TYPE_FROM, 2))                                                                      \
    {                                                                                                                                   \
    }                                                                                                                                   \
    template <class ScalarT>                                                                                                            \
    constexpr TYPE_TO<4, ScalarT>::TYPE_TO(TYPE_FROM<4, ScalarT> const& a)                                                              \
      : TG_IMPL_MEMBER(TYPE_TO, 0)(a.TG_IMPL_MEMBER(TYPE_FROM, 0)),                                                                     \
        TG_IMPL_MEMBER(TYPE_TO, 1)(a.TG_IMPL_MEMBER(TYPE_FROM, 1)),                                                                     \
        TG_IMPL_MEMBER(TYPE_TO, 2)(a.TG_IMPL_MEMBER(TYPE_FROM, 2)),                                                                     \
        TG_IMPL_MEMBER(TYPE_TO, 3)(a.TG_IMPL_MEMBER(TYPE_FROM, 3))                                                                      \
    {                                                                                                                                   \
    }                                                                                                                                   \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_BINARY_OP(TYPE_A, TYPE_B, TYPE_R, OP)                                                        \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                      \
    TG_NODISCARD constexpr TYPE_R<1, ScalarR> operator OP(TYPE_A<1, ScalarA> const& a, TYPE_B<1, ScalarB> const& b) \
    {                                                                                                               \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 0)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 0))};                      \
    }                                                                                                               \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                      \
    TG_NODISCARD constexpr TYPE_R<2, ScalarR> operator OP(TYPE_A<2, ScalarA> const& a, TYPE_B<2, ScalarB> const& b) \
    {                                                                                                               \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 0)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 0)),                       \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 1)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 1))};                      \
    }                                                                                                               \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                      \
    TG_NODISCARD constexpr TYPE_R<3, ScalarR> operator OP(TYPE_A<3, ScalarA> const& a, TYPE_B<3, ScalarB> const& b) \
    {                                                                                                               \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 0)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 0)),                       \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 1)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 1)),                       \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 2)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 2))};                      \
    }                                                                                                               \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                      \
    TG_NODISCARD constexpr TYPE_R<4, ScalarR> operator OP(TYPE_A<4, ScalarA> const& a, TYPE_B<4, ScalarB> const& b) \
    {                                                                                                               \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 0)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 0)),                       \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 1)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 1)),                       \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 2)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 2)),                       \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 3)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 3))};                      \
    }                                                                                                               \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_RIGHT(TYPE, OP)                                                                               \
    template <class ScalarA, class ScalarB, class = enable_if<is_scalar<ScalarB>>, class ScalarR = promoted_scalar<ScalarA, ScalarB>> \
    TG_NODISCARD constexpr TYPE<1, ScalarR> operator OP(TYPE<1, ScalarA> const& a, ScalarB const& b)                                  \
    {                                                                                                                                 \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) OP ScalarR(b)};                                                                    \
    }                                                                                                                                 \
    template <class ScalarA, class ScalarB, class = enable_if<is_scalar<ScalarB>>, class ScalarR = promoted_scalar<ScalarA, ScalarB>> \
    TG_NODISCARD constexpr TYPE<2, ScalarR> operator OP(TYPE<2, ScalarA> const& a, ScalarB const& b)                                  \
    {                                                                                                                                 \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) OP ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) OP ScalarR(b)};                  \
    }                                                                                                                                 \
    template <class ScalarA, class ScalarB, class = enable_if<is_scalar<ScalarB>>, class ScalarR = promoted_scalar<ScalarA, ScalarB>> \
    TG_NODISCARD constexpr TYPE<3, ScalarR> operator OP(TYPE<3, ScalarA> const& a, ScalarB const& b)                                  \
    {                                                                                                                                 \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) OP ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) OP ScalarR(b),                   \
                ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) OP ScalarR(b)};                                                                    \
    }                                                                                                                                 \
    template <class ScalarA, class ScalarB, class = enable_if<is_scalar<ScalarB>>, class ScalarR = promoted_scalar<ScalarA, ScalarB>> \
    TG_NODISCARD constexpr TYPE<4, ScalarR> operator OP(TYPE<4, ScalarA> const& a, ScalarB const& b)                                  \
    {                                                                                                                                 \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) OP ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) OP ScalarR(b),                   \
                ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) OP ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 3)) OP ScalarR(b)};                  \
    }                                                                                                                                 \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, OP)                                                                                \
    template <class ScalarA, class ScalarB, class = enable_if<is_scalar<ScalarA>>, class ScalarR = promoted_scalar<ScalarA, ScalarB>> \
    TG_NODISCARD constexpr TYPE<1, ScalarR> operator OP(ScalarA const& a, TYPE<1, ScalarB> const& b)                                  \
    {                                                                                                                                 \
        return {ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 0))};                                                                    \
    }                                                                                                                                 \
    template <class ScalarA, class ScalarB, class = enable_if<is_scalar<ScalarA>>, class ScalarR = promoted_scalar<ScalarA, ScalarB>> \
    TG_NODISCARD constexpr TYPE<2, ScalarR> operator OP(ScalarA const& a, TYPE<2, ScalarB> const& b)                                  \
    {                                                                                                                                 \
        return {ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 0)), ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 1))};                  \
    }                                                                                                                                 \
    template <class ScalarA, class ScalarB, class = enable_if<is_scalar<ScalarA>>, class ScalarR = promoted_scalar<ScalarA, ScalarB>> \
    TG_NODISCARD constexpr TYPE<3, ScalarR> operator OP(ScalarA const& a, TYPE<3, ScalarB> const& b)                                  \
    {                                                                                                                                 \
        return {ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 0)), ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 1)),                   \
                ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 2))};                                                                    \
    }                                                                                                                                 \
    template <class ScalarA, class ScalarB, class = enable_if<is_scalar<ScalarA>>, class ScalarR = promoted_scalar<ScalarA, ScalarB>> \
    TG_NODISCARD constexpr TYPE<4, ScalarR> operator OP(ScalarA const& a, TYPE<4, ScalarB> const& b)                                  \
    {                                                                                                                                 \
        return {ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 0)), ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 1)),                   \
                ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 2)), ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 3))};                  \
    }                                                                                                                                 \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(TYPE)                                                                                                                                            \
    /* scalar / type */                                                                                                                                                                      \
    TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, /);                                                                                                                                           \
    /* type / scalar, optimized if result is floating point */                                                                                                                               \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<is_floating_point<ScalarR> && is_scalar<ScalarB>>>                          \
    TG_NODISCARD constexpr TYPE<1, ScalarR> operator/(TYPE<1, ScalarA> const& a, ScalarB const& b)                                                                                           \
    {                                                                                                                                                                                        \
        auto inv_b = ScalarR(1) / ScalarR(b);                                                                                                                                                \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) * inv_b};                                                                                                                                 \
    }                                                                                                                                                                                        \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<is_floating_point<ScalarR> && is_scalar<ScalarB>>>                          \
    TG_NODISCARD constexpr TYPE<2, ScalarR> operator/(TYPE<2, ScalarA> const& a, ScalarB const& b)                                                                                           \
    {                                                                                                                                                                                        \
        auto inv_b = ScalarR(1) / ScalarR(b);                                                                                                                                                \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) * inv_b, ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) * inv_b};                                                                                     \
    }                                                                                                                                                                                        \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<is_floating_point<ScalarR> && is_scalar<ScalarB>>>                          \
    TG_NODISCARD constexpr TYPE<3, ScalarR> operator/(TYPE<3, ScalarA> const& a, ScalarB const& b)                                                                                           \
    {                                                                                                                                                                                        \
        auto inv_b = ScalarR(1) / ScalarR(b);                                                                                                                                                \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) * inv_b, ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) * inv_b, ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) * inv_b};                                         \
    }                                                                                                                                                                                        \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<is_floating_point<ScalarR> && is_scalar<ScalarB>>>                          \
    TG_NODISCARD constexpr TYPE<4, ScalarR> operator/(TYPE<4, ScalarA> const& a, ScalarB const& b)                                                                                           \
    {                                                                                                                                                                                        \
        auto inv_b = ScalarR(1) / ScalarR(b);                                                                                                                                                \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) * inv_b, ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) * inv_b, ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) * inv_b,                                          \
                ScalarR(a.TG_IMPL_MEMBER(TYPE, 3)) * inv_b};                                                                                                                                 \
    }                                                                                                                                                                                        \
    /* type / scalar, for non-float result */                                                                                                                                                \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<!is_floating_point<ScalarR> && is_scalar<ScalarB>>, class = detail::unused> \
    TG_NODISCARD constexpr TYPE<1, ScalarR> operator/(TYPE<1, ScalarA> const& a, ScalarB const& b)                                                                                           \
    {                                                                                                                                                                                        \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) / ScalarR(b)};                                                                                                                            \
    }                                                                                                                                                                                        \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<!is_floating_point<ScalarR> && is_scalar<ScalarB>>, class = detail::unused> \
    TG_NODISCARD constexpr TYPE<2, ScalarR> operator/(TYPE<2, ScalarA> const& a, ScalarB const& b)                                                                                           \
    {                                                                                                                                                                                        \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) / ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) / ScalarR(b)};                                                                           \
    }                                                                                                                                                                                        \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<!is_floating_point<ScalarR> && is_scalar<ScalarB>>, class = detail::unused> \
    TG_NODISCARD constexpr TYPE<3, ScalarR> operator/(TYPE<3, ScalarA> const& a, ScalarB const& b)                                                                                           \
    {                                                                                                                                                                                        \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) / ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) / ScalarR(b),                                                                            \
                ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) / ScalarR(b)};                                                                                                                            \
    }                                                                                                                                                                                        \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<!is_floating_point<ScalarR> && is_scalar<ScalarB>>, class = detail::unused> \
    TG_NODISCARD constexpr TYPE<4, ScalarR> operator/(TYPE<4, ScalarA> const& a, ScalarB const& b)                                                                                           \
    {                                                                                                                                                                                        \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) / ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) / ScalarR(b),                                                                            \
                ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) / ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 3)) / ScalarR(b)};                                                                           \
    }                                                                                                                                                                                        \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR(TYPE, OP)   \
    TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, OP); \
    TG_IMPL_DEFINE_BINARY_OP_SCALAR_RIGHT(TYPE, OP)

#define TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(TYPE, FUN)                                                                                                          \
    template <int D, class ScalarT>                                                                                                                            \
    TG_NODISCARD constexpr auto FUN(TYPE<D, ScalarT> const& a)                                                                                                 \
    {                                                                                                                                                          \
        using R = remove_const_ref<decltype(FUN(a.TG_IMPL_MEMBER(TYPE, 0)))>;                                                                                  \
                                                                                                                                                               \
        if constexpr (D == 1)                                                                                                                                  \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0))};                                                                                                 \
                                                                                                                                                               \
        else if constexpr (D == 2)                                                                                                                             \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1))};                                                                 \
                                                                                                                                                               \
        else if constexpr (D == 3)                                                                                                                             \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1)), FUN(a.TG_IMPL_MEMBER(TYPE, 2))};                                 \
                                                                                                                                                               \
        else if constexpr (D == 4)                                                                                                                             \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1)), FUN(a.TG_IMPL_MEMBER(TYPE, 2)), FUN(a.TG_IMPL_MEMBER(TYPE, 3))}; \
                                                                                                                                                               \
        else                                                                                                                                                   \
            static_assert(always_false<ScalarT>, "only supported up to 4 dimensions");                                                                         \
    }                                                                                                                                                          \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_COMPWISE_FUNC_BINARY(TYPE, FUN)                                                                                               \
    template <int D, class ScalarT>                                                                                                                  \
    TG_NODISCARD constexpr auto FUN(TYPE<D, ScalarT> const& a, TYPE<D, ScalarT> const& b)                                                            \
    {                                                                                                                                                \
        using R = remove_const_ref<decltype(FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0)))>;                                             \
                                                                                                                                                     \
        if constexpr (D == 1)                                                                                                                        \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0))};                                                            \
                                                                                                                                                     \
        else if constexpr (D == 2)                                                                                                                   \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1), b.TG_IMPL_MEMBER(TYPE, 1))}; \
                                                                                                                                                     \
        else if constexpr (D == 3)                                                                                                                   \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1), b.TG_IMPL_MEMBER(TYPE, 1)),  \
                              FUN(a.TG_IMPL_MEMBER(TYPE, 2), b.TG_IMPL_MEMBER(TYPE, 2))};                                                            \
                                                                                                                                                     \
        else if constexpr (D == 4)                                                                                                                   \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1), b.TG_IMPL_MEMBER(TYPE, 1)),  \
                              FUN(a.TG_IMPL_MEMBER(TYPE, 2), b.TG_IMPL_MEMBER(TYPE, 2)), FUN(a.TG_IMPL_MEMBER(TYPE, 3), b.TG_IMPL_MEMBER(TYPE, 3))}; \
                                                                                                                                                     \
        else                                                                                                                                         \
            static_assert(always_false<ScalarT>, "only supported up to 4 dimensions");                                                               \
    }                                                                                                                                                \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_COMPWISE_FUNC_TERNARY(TYPE, FUN)                                                                                      \
    template <class ScalarA, class ScalarB, class ScalarC>                                                                                   \
    TG_NODISCARD constexpr TYPE<1, decltype(FUN(ScalarA(), ScalarB(), ScalarC()))> FUN(TYPE<1, ScalarA> const& a, TYPE<1, ScalarB> const& b, \
                                                                                       TYPE<1, ScalarC> const& c)                            \
    {                                                                                                                                        \
        return {FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0), c.TG_IMPL_MEMBER(TYPE, 0))};                                       \
    }                                                                                                                                        \
    template <class ScalarA, class ScalarB, class ScalarC>                                                                                   \
    TG_NODISCARD constexpr TYPE<2, decltype(FUN(ScalarA(), ScalarB(), ScalarC()))> FUN(TYPE<2, ScalarA> const& a, TYPE<2, ScalarB> const& b, \
                                                                                       TYPE<2, ScalarC> const& c)                            \
    {                                                                                                                                        \
        return {FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0), c.TG_IMPL_MEMBER(TYPE, 0)),                                        \
                FUN(a.TG_IMPL_MEMBER(TYPE, 1), b.TG_IMPL_MEMBER(TYPE, 1), c.TG_IMPL_MEMBER(TYPE, 1))};                                       \
    }                                                                                                                                        \
    template <class ScalarA, class ScalarB, class ScalarC>                                                                                   \
    TG_NODISCARD constexpr TYPE<3, decltype(FUN(ScalarA(), ScalarB(), ScalarC()))> FUN(TYPE<3, ScalarA> const& a, TYPE<3, ScalarB> const& b, \
                                                                                       TYPE<3, ScalarC> const& c)                            \
    {                                                                                                                                        \
        return {FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0), c.TG_IMPL_MEMBER(TYPE, 0)),                                        \
                FUN(a.TG_IMPL_MEMBER(TYPE, 1), b.TG_IMPL_MEMBER(TYPE, 1), c.TG_IMPL_MEMBER(TYPE, 1)),                                        \
                FUN(a.TG_IMPL_MEMBER(TYPE, 2), b.TG_IMPL_MEMBER(TYPE, 2), c.TG_IMPL_MEMBER(TYPE, 2))};                                       \
    }                                                                                                                                        \
    template <class ScalarA, class ScalarB, class ScalarC>                                                                                   \
    TG_NODISCARD constexpr TYPE<4, decltype(FUN(ScalarA(), ScalarB(), ScalarC()))> FUN(TYPE<4, ScalarA> const& a, TYPE<4, ScalarB> const& b, \
                                                                                       TYPE<4, ScalarC> const& c)                            \
    {                                                                                                                                        \
        return {FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0), c.TG_IMPL_MEMBER(TYPE, 0)),                                        \
                FUN(a.TG_IMPL_MEMBER(TYPE, 1), b.TG_IMPL_MEMBER(TYPE, 1), c.TG_IMPL_MEMBER(TYPE, 1)),                                        \
                FUN(a.TG_IMPL_MEMBER(TYPE, 2), b.TG_IMPL_MEMBER(TYPE, 2), c.TG_IMPL_MEMBER(TYPE, 2)),                                        \
                FUN(a.TG_IMPL_MEMBER(TYPE, 3), b.TG_IMPL_MEMBER(TYPE, 3), c.TG_IMPL_MEMBER(TYPE, 3))};                                       \
    }                                                                                                                                        \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_REDUCTION_OP_BINARY(TYPE_A, TYPE_B, RESULT_T, NAME, REDUCE, OP)                                                                 \
    template <class ScalarT>                                                                                                                           \
    TG_NODISCARD constexpr RESULT_T NAME(TYPE_A<1, ScalarT> const& a, TYPE_B<1, ScalarT> const& b)                                                     \
    {                                                                                                                                                  \
        return a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0);                                                                             \
    }                                                                                                                                                  \
    template <class ScalarT>                                                                                                                           \
    TG_NODISCARD constexpr RESULT_T NAME(TYPE_A<2, ScalarT> const& a, TYPE_B<2, ScalarT> const& b)                                                     \
    {                                                                                                                                                  \
        return (a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1));         \
    }                                                                                                                                                  \
    template <class ScalarT>                                                                                                                           \
    TG_NODISCARD constexpr RESULT_T NAME(TYPE_A<3, ScalarT> const& a, TYPE_B<3, ScalarT> const& b)                                                     \
    {                                                                                                                                                  \
        return (a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1))          \
            REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 2) OP b.TG_IMPL_MEMBER(TYPE_B, 2));                                                                        \
    }                                                                                                                                                  \
    template <class ScalarT>                                                                                                                           \
    TG_NODISCARD constexpr RESULT_T NAME(TYPE_A<4, ScalarT> const& a, TYPE_B<4, ScalarT> const& b)                                                     \
    {                                                                                                                                                  \
        return ((a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1)))REDUCE( \
            (a.TG_IMPL_MEMBER(TYPE_A, 2) OP b.TG_IMPL_MEMBER(TYPE_B, 2))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 3) OP b.TG_IMPL_MEMBER(TYPE_B, 3)));           \
    }                                                                                                                                                  \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_ASSIGNMENT_OP(TYPE_THIS, OP)                                                                           \
    template <int D, class ScalarA, class T>                                                                                  \
    constexpr auto operator OP##=(TYPE_THIS<D, ScalarA>& lhs, T const& rhs)->decltype(TYPE_THIS<D, ScalarA>(lhs OP rhs), lhs) \
    {                                                                                                                         \
        lhs = TYPE_THIS<D, ScalarA>(lhs OP rhs);                                                                              \
        return lhs;                                                                                                           \
    }                                                                                                                         \
    TG_FORCE_SEMICOLON


#define TG_IMPL_DEFINE_TRAIT(trait, result_type, default_val) \
    template <class T>                                        \
    struct trait##_t                                          \
    {                                                         \
        static constexpr result_type value = default_val;     \
    };                                                        \
    template <class T>                                        \
    constexpr result_type trait = trait##_t<T>::value // enforce ;

#define TG_IMPL_DEFINE_BINARY_TRAIT(trait, result_type, default_val) \
    template <class A, class B>                                      \
    struct trait##_t                                                 \
    {                                                                \
        static constexpr result_type value = default_val;            \
    };                                                               \
    template <class A, class B>                                      \
    constexpr result_type trait = trait##_t<A, B>::value // enforce ;

#define TG_IMPL_DEFINE_TYPE_TRAIT(trait, default_type) \
    template <class T>                                 \
    struct trait##_t                                   \
    {                                                  \
        using type = default_type;                     \
    };                                                 \
    template <class T>                                 \
    using trait = typename trait##_t<T>::type // enforce ;

#define TG_IMPL_DEFINE_BINARY_TYPE_TRAIT(trait, ...) \
    template <class A, class B>                      \
    struct trait##_t                                 \
    {                                                \
        using type = __VA_ARGS__;                    \
    };                                               \
    template <class A, class B>                      \
    using trait = typename trait##_t<A, B>::type // enforce ;

#define TG_IMPL_ADD_TRAIT(trait, result_type, type, val) \
    template <>                                          \
    struct trait##_t<type>                               \
    {                                                    \
        static constexpr result_type value = val;        \
    } // enforce ;
#define TG_IMPL_ADD_BINARY_TRAIT(trait, result_type, typeA, typeB, val) \
    template <>                                                         \
    struct trait##_t<typeA, typeB>                                      \
    {                                                                   \
        static constexpr result_type value = val;                       \
    } // enforce ;
#define TG_IMPL_ADD_TYPE_TRAIT(trait, ttype, result_type) \
    template <>                                           \
    struct trait##_t<ttype>                               \
    {                                                     \
        using type = result_type;                         \
    } // enforce ;
#define TG_IMPL_ADD_BINARY_TYPE_TRAIT(trait, ttypeA, ttypeB, result_type) \
    template <>                                                           \
    struct trait##_t<ttypeA, ttypeB>                                      \
    {                                                                     \
        using type = result_type;                                         \
    } // enforce ;
#define TG_IMPL_ADD_SCALAR_TYPE(basetype, bits, rtype) \
    template <>                                        \
    struct scalar_t<basetype, bits>                    \
    {                                                  \
        using type = rtype;                            \
    } // enforce ;

#define TG_IMPL_ADD_OBJECT_TYPE(ttype)      \
    template <int D, class ScalarT>         \
    struct is_object_t<ttype<D, ScalarT>>   \
    {                                       \
        static constexpr bool value = true; \
    } // enforce ;

#define TG_IMPL_INHERIT_TRAITS_D(ttype)                             \
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


#define TG_IMPL_INHERIT_TRAITS_FIELD_D(ttype)                      \
    TG_IMPL_INHERIT_TRAITS_D(ttype);                               \
    template <int D, class ScalarT>                                \
    struct has_multiplication_t<ttype<D, ScalarT>>                 \
    {                                                              \
        static constexpr bool value = has_multiplication<ScalarT>; \
    } // enforce ;
