#pragma once

#define TG_IMPL_MEMBER_pos_0 x
#define TG_IMPL_MEMBER_pos_1 y
#define TG_IMPL_MEMBER_pos_2 z
#define TG_IMPL_MEMBER_pos_3 w

#define TG_IMPL_MEMBER_vec_0 x
#define TG_IMPL_MEMBER_vec_1 y
#define TG_IMPL_MEMBER_vec_2 z
#define TG_IMPL_MEMBER_vec_3 w

#define TG_IMPL_MEMBER_size_0 width
#define TG_IMPL_MEMBER_size_1 height
#define TG_IMPL_MEMBER_size_2 depth
#define TG_IMPL_MEMBER_size_3 w

#define TG_IMPL_MEMBER(TYPE, COMP) TG_IMPL_MEMBER_##TYPE##_##COMP

#define TG_IMPL_DEFINE_UNARY_OP(TYPE, OP)                                                                                                \
    template <class ScalarT>                                                                                                             \
    constexpr TYPE<1, ScalarT> operator OP(TYPE<1, ScalarT> const& a)                                                                    \
    {                                                                                                                                    \
        return {OP a.TG_IMPL_MEMBER(TYPE, 0)};                                                                                           \
    }                                                                                                                                    \
    template <class ScalarT>                                                                                                             \
    constexpr TYPE<2, ScalarT> operator OP(TYPE<2, ScalarT> const& a)                                                                    \
    {                                                                                                                                    \
        return {OP a.TG_IMPL_MEMBER(TYPE, 0), OP a.TG_IMPL_MEMBER(TYPE, 1)};                                                             \
    }                                                                                                                                    \
    template <class ScalarT>                                                                                                             \
    constexpr TYPE<3, ScalarT> operator OP(TYPE<3, ScalarT> const& a)                                                                    \
    {                                                                                                                                    \
        return {OP a.TG_IMPL_MEMBER(TYPE, 0), OP a.TG_IMPL_MEMBER(TYPE, 1), OP a.TG_IMPL_MEMBER(TYPE, 2)};                               \
    }                                                                                                                                    \
    template <class ScalarT>                                                                                                             \
    constexpr TYPE<4, ScalarT> operator OP(TYPE<4, ScalarT> const& a)                                                                    \
    {                                                                                                                                    \
        return {OP a.TG_IMPL_MEMBER(TYPE, 0), OP a.TG_IMPL_MEMBER(TYPE, 1), OP a.TG_IMPL_MEMBER(TYPE, 2), OP a.TG_IMPL_MEMBER(TYPE, 3)}; \
    }

#define TG_IMPL_DEFINE_BINARY_OP(TYPE_A, TYPE_B, TYPE_R, OP)                                           \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>         \
    constexpr TYPE_R<1, ScalarR> operator OP(TYPE_A<1, ScalarA> const& a, TYPE_B<1, ScalarB> const& b) \
    {                                                                                                  \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 0)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 0))};         \
    }                                                                                                  \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>         \
    constexpr TYPE_R<2, ScalarR> operator OP(TYPE_A<2, ScalarA> const& a, TYPE_B<2, ScalarB> const& b) \
    {                                                                                                  \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 0)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 0)),          \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 1)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 1))};         \
    }                                                                                                  \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>         \
    constexpr TYPE_R<3, ScalarR> operator OP(TYPE_A<3, ScalarA> const& a, TYPE_B<3, ScalarB> const& b) \
    {                                                                                                  \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 0)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 0)),          \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 1)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 1)),          \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 2)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 2))};         \
    }                                                                                                  \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>         \
    constexpr TYPE_R<4, ScalarR> operator OP(TYPE_A<4, ScalarA> const& a, TYPE_B<4, ScalarB> const& b) \
    {                                                                                                  \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 0)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 0)),          \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 1)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 1)),          \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 2)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 2)),          \
                ScalarR(a.TG_IMPL_MEMBER(TYPE_A, 3)) OP ScalarR(b.TG_IMPL_MEMBER(TYPE_B, 3))};         \
    }

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_RIGHT(TYPE, OP)                                                              \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                       \
    constexpr TYPE<1, ScalarR> operator OP(TYPE<1, ScalarA> const& a, ScalarB const& b)                              \
    {                                                                                                                \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) OP ScalarR(b)};                                                   \
    }                                                                                                                \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                       \
    constexpr TYPE<2, ScalarR> operator OP(TYPE<2, ScalarA> const& a, ScalarB const& b)                              \
    {                                                                                                                \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) OP ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) OP ScalarR(b)}; \
    }                                                                                                                \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                       \
    constexpr TYPE<3, ScalarR> operator OP(TYPE<3, ScalarA> const& a, ScalarB const& b)                              \
    {                                                                                                                \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) OP ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) OP ScalarR(b),  \
                ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) OP ScalarR(b)};                                                   \
    }                                                                                                                \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                       \
    constexpr TYPE<4, ScalarR> operator OP(TYPE<4, ScalarA> const& a, ScalarB const& b)                              \
    {                                                                                                                \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) OP ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) OP ScalarR(b),  \
                ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) OP ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 3)) OP ScalarR(b)}; \
    }

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, OP)                                                               \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                       \
    constexpr TYPE<1, ScalarR> operator OP(ScalarA const& a, TYPE<1, ScalarB> const& b)                              \
    {                                                                                                                \
        return {ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 0))};                                                   \
    }                                                                                                                \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                       \
    constexpr TYPE<2, ScalarR> operator OP(ScalarA const& a, TYPE<2, ScalarB> const& b)                              \
    {                                                                                                                \
        return {ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 0)), ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 1))}; \
    }                                                                                                                \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                       \
    constexpr TYPE<3, ScalarR> operator OP(ScalarA const& a, TYPE<3, ScalarB> const& b)                              \
    {                                                                                                                \
        return {ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 0)), ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 1)),  \
                ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 2))};                                                   \
    }                                                                                                                \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>>                       \
    constexpr TYPE<4, ScalarR> operator OP(ScalarA const& a, TYPE<4, ScalarB> const& b)                              \
    {                                                                                                                \
        return {ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 0)), ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 1)),  \
                ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 2)), ScalarR(a) OP ScalarR(b.TG_IMPL_MEMBER(TYPE, 3))}; \
    }

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(TYPE)                                                                                                                      \
    /* scalar / type */                                                                                                                                                \
    TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, /)                                                                                                                      \
    /* type / scalar, optimized if result is floating point */                                                                                                         \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<is_floating_point<ScalarR>>>                          \
    constexpr TYPE<1, ScalarR> operator/(TYPE<1, ScalarA> const& a, ScalarB const& b)                                                                                  \
    {                                                                                                                                                                  \
        auto inv_b = ScalarR(1) / ScalarR(b);                                                                                                                          \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) * inv_b};                                                                                                           \
    }                                                                                                                                                                  \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<is_floating_point<ScalarR>>>                          \
    constexpr TYPE<2, ScalarR> operator/(TYPE<2, ScalarA> const& a, ScalarB const& b)                                                                                  \
    {                                                                                                                                                                  \
        auto inv_b = ScalarR(1) / ScalarR(b);                                                                                                                          \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) * inv_b, ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) * inv_b};                                                               \
    }                                                                                                                                                                  \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<is_floating_point<ScalarR>>>                          \
    constexpr TYPE<3, ScalarR> operator/(TYPE<3, ScalarA> const& a, ScalarB const& b)                                                                                  \
    {                                                                                                                                                                  \
        auto inv_b = ScalarR(1) / ScalarR(b);                                                                                                                          \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) * inv_b, ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) * inv_b, ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) * inv_b};                   \
    }                                                                                                                                                                  \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<is_floating_point<ScalarR>>>                          \
    constexpr TYPE<4, ScalarR> operator/(TYPE<4, ScalarA> const& a, ScalarB const& b)                                                                                  \
    {                                                                                                                                                                  \
        auto inv_b = ScalarR(1) / ScalarR(b);                                                                                                                          \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) * inv_b, ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) * inv_b, ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) * inv_b,                    \
                ScalarR(a.TG_IMPL_MEMBER(TYPE, 3)) * inv_b};                                                                                                           \
    }                                                                                                                                                                  \
    /* type / scalar, for non-float result */                                                                                                                          \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<!is_floating_point<ScalarR>>, class = detail::unused> \
    constexpr TYPE<1, ScalarR> operator/(TYPE<1, ScalarA> const& a, ScalarB const& b)                                                                                  \
    {                                                                                                                                                                  \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) / ScalarR(b)};                                                                                                      \
    }                                                                                                                                                                  \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<!is_floating_point<ScalarR>>, class = detail::unused> \
    constexpr TYPE<2, ScalarR> operator/(TYPE<2, ScalarA> const& a, ScalarB const& b)                                                                                  \
    {                                                                                                                                                                  \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) / ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) / ScalarR(b)};                                                     \
    }                                                                                                                                                                  \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<!is_floating_point<ScalarR>>, class = detail::unused> \
    constexpr TYPE<3, ScalarR> operator/(TYPE<3, ScalarA> const& a, ScalarB const& b)                                                                                  \
    {                                                                                                                                                                  \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) / ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) / ScalarR(b),                                                      \
                ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) / ScalarR(b)};                                                                                                      \
    }                                                                                                                                                                  \
    template <class ScalarA, class ScalarB, class ScalarR = promoted_scalar<ScalarA, ScalarB>, class = enable_if<!is_floating_point<ScalarR>>, class = detail::unused> \
    constexpr TYPE<4, ScalarR> operator/(TYPE<4, ScalarA> const& a, ScalarB const& b)                                                                                  \
    {                                                                                                                                                                  \
        return {ScalarR(a.TG_IMPL_MEMBER(TYPE, 0)) / ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 1)) / ScalarR(b),                                                      \
                ScalarR(a.TG_IMPL_MEMBER(TYPE, 2)) / ScalarR(b), ScalarR(a.TG_IMPL_MEMBER(TYPE, 3)) / ScalarR(b)};                                                     \
    }

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR(TYPE, OP) TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, OP) TG_IMPL_DEFINE_BINARY_OP_SCALAR_RIGHT(TYPE, OP)

#define TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(TYPE, FUN)                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<1, decltype(FUN(ScalarT(0)))> FUN(TYPE<1, ScalarT> const& a)                                                                  \
    {                                                                                                                                            \
        return {FUN(a.TG_IMPL_MEMBER(TYPE, 0))};                                                                                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<2, decltype(FUN(ScalarT(0)))> FUN(TYPE<2, ScalarT> const& a)                                                                  \
    {                                                                                                                                            \
        return {FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1))};                                                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<3, decltype(FUN(ScalarT(0)))> FUN(TYPE<3, ScalarT> const& a)                                                                  \
    {                                                                                                                                            \
        return {FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1)), FUN(a.TG_IMPL_MEMBER(TYPE, 2))};                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<4, decltype(FUN(ScalarT(0)))> FUN(TYPE<4, ScalarT> const& a)                                                                  \
    {                                                                                                                                            \
        return {FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1)), FUN(a.TG_IMPL_MEMBER(TYPE, 2)), FUN(a.TG_IMPL_MEMBER(TYPE, 3))}; \
    }

#define TG_IMPL_DEFINE_REDUCTION_OP_BINARY(TYPE_A, TYPE_B, RESULT_T, NAME, REDUCE, OP)                                                                 \
    template <class ScalarT>                                                                                                                           \
    constexpr RESULT_T NAME(TYPE_A<1, ScalarT> const& a, TYPE_B<1, ScalarT> const& b)                                                                  \
    {                                                                                                                                                  \
        return a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0);                                                                             \
    }                                                                                                                                                  \
    template <class ScalarT>                                                                                                                           \
    constexpr RESULT_T NAME(TYPE_A<2, ScalarT> const& a, TYPE_B<2, ScalarT> const& b)                                                                  \
    {                                                                                                                                                  \
        return (a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1));         \
    }                                                                                                                                                  \
    template <class ScalarT>                                                                                                                           \
    constexpr RESULT_T NAME(TYPE_A<3, ScalarT> const& a, TYPE_B<3, ScalarT> const& b)                                                                  \
    {                                                                                                                                                  \
        return (a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1))          \
            REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 2) OP b.TG_IMPL_MEMBER(TYPE_B, 2));                                                                        \
    }                                                                                                                                                  \
    template <class ScalarT>                                                                                                                           \
    constexpr RESULT_T NAME(TYPE_A<4, ScalarT> const& a, TYPE_B<4, ScalarT> const& b)                                                                  \
    {                                                                                                                                                  \
        return ((a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1)))REDUCE( \
            (a.TG_IMPL_MEMBER(TYPE_A, 2) OP b.TG_IMPL_MEMBER(TYPE_B, 2))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 3) OP b.TG_IMPL_MEMBER(TYPE_B, 3)));           \
    }

#define TG_IMPL_DEFINE_ASSIGNMENT_OP(TYPE_THIS, TYPE_THAT, OP)                                                                                    \
    template <int D, class ScalarA, class ScalarB>                                                                                                \
    constexpr auto operator OP##=(TYPE_THIS<D, ScalarA>& lhs, TYPE_THAT<D, ScalarB> const& rhs)->decltype(TYPE_THIS<D, ScalarA>(lhs OP rhs), lhs) \
    {                                                                                                                                             \
        lhs = TYPE_THIS<D, ScalarA>(lhs OP rhs);                                                                                                  \
        return lhs;                                                                                                                               \
    }

#define TG_IMPL_DEFINE_ASSIGNMENT_OP_SCALAR(TYPE_THIS, OP)                                                                          \
    template <int D, class ScalarA, class ScalarB>                                                                                  \
    constexpr auto operator OP##=(TYPE_THIS<D, ScalarA>& lhs, ScalarB const& rhs)->decltype(TYPE_THIS<D, ScalarA>(lhs OP rhs), lhs) \
    {                                                                                                                               \
        lhs = TYPE_THIS<D, ScalarA>(lhs OP rhs);                                                                                    \
        return lhs;                                                                                                                 \
    }
