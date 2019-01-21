#pragma once

// TODO

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
        return TYPE<1, ScalarT>{OP a.TG_IMPL_MEMBER(TYPE, 0)};                                                                                           \
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

#define TG_IMPL_DEFINE_BINARY_OP(TYPE_A, TYPE_B, TYPE_R, OP)                                                                             \
    template <class ScalarT>                                                                                                             \
    constexpr TYPE_R<1, ScalarT> operator OP(TYPE_A<1, ScalarT> const& a, TYPE_B<1, ScalarT> const& b)                                   \
    {                                                                                                                                    \
        return TYPE_R<1, ScalarT>{a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0)};                                                             \
    }                                                                                                                                    \
    template <class ScalarT>                                                                                                             \
    constexpr TYPE_R<2, ScalarT> operator OP(TYPE_A<2, ScalarT> const& a, TYPE_B<2, ScalarT> const& b)                                   \
    {                                                                                                                                    \
        return {a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0), a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1)}; \
    }                                                                                                                                    \
    template <class ScalarT>                                                                                                             \
    constexpr TYPE_R<3, ScalarT> operator OP(TYPE_A<3, ScalarT> const& a, TYPE_B<3, ScalarT> const& b)                                   \
    {                                                                                                                                    \
        return {a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0), a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1),  \
                a.TG_IMPL_MEMBER(TYPE_A, 2) OP b.TG_IMPL_MEMBER(TYPE_B, 2)};                                                             \
    }                                                                                                                                    \
    template <class ScalarT>                                                                                                             \
    constexpr TYPE_R<4, ScalarT> operator OP(TYPE_A<4, ScalarT> const& a, TYPE_B<4, ScalarT> const& b)                                   \
    {                                                                                                                                    \
        return {a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0), a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1),  \
                a.TG_IMPL_MEMBER(TYPE_A, 2) OP b.TG_IMPL_MEMBER(TYPE_B, 2), a.TG_IMPL_MEMBER(TYPE_A, 3) OP b.TG_IMPL_MEMBER(TYPE_B, 3)}; \
    }

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_RIGHT(TYPE, OP)                                                                                          \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<1, ScalarT> operator OP(TYPE<1, ScalarT> const& a, ScalarT const& b)                                                          \
    {                                                                                                                                            \
        return TYPE<1, ScalarT>{a.TG_IMPL_MEMBER(TYPE, 0) OP b};                                                                                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<2, ScalarT> operator OP(TYPE<2, ScalarT> const& a, ScalarT const& b)                                                          \
    {                                                                                                                                            \
        return {a.TG_IMPL_MEMBER(TYPE, 0) OP b, a.TG_IMPL_MEMBER(TYPE, 1) OP b};                                                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<3, ScalarT> operator OP(TYPE<3, ScalarT> const& a, ScalarT const& b)                                                          \
    {                                                                                                                                            \
        return {a.TG_IMPL_MEMBER(TYPE, 0) OP b, a.TG_IMPL_MEMBER(TYPE, 1) OP b, a.TG_IMPL_MEMBER(TYPE, 2) OP b};                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<4, ScalarT> operator OP(TYPE<4, ScalarT> const& a, ScalarT const& b)                                                          \
    {                                                                                                                                            \
        return {a.TG_IMPL_MEMBER(TYPE, 0) OP b, a.TG_IMPL_MEMBER(TYPE, 1) OP b, a.TG_IMPL_MEMBER(TYPE, 2) OP b, a.TG_IMPL_MEMBER(TYPE, 3) OP b}; \
    }

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, OP)                                                                                           \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<1, ScalarT> operator OP(ScalarT const& a, TYPE<1, ScalarT> const& b)                                                          \
    {                                                                                                                                            \
        return TYPE<1, ScalarT>{a OP b.TG_IMPL_MEMBER(TYPE, 0)};                                                                                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<2, ScalarT> operator OP(ScalarT const& a, TYPE<2, ScalarT> const& b)                                                          \
    {                                                                                                                                            \
        return {a OP b.TG_IMPL_MEMBER(TYPE, 0), a OP b.TG_IMPL_MEMBER(TYPE, 1)};                                                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<3, ScalarT> operator OP(ScalarT const& a, TYPE<3, ScalarT> const& b)                                                          \
    {                                                                                                                                            \
        return {a OP b.TG_IMPL_MEMBER(TYPE, 0), a OP b.TG_IMPL_MEMBER(TYPE, 1), a OP b.TG_IMPL_MEMBER(TYPE, 2)};                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<4, ScalarT> operator OP(ScalarT const& a, TYPE<4, ScalarT> const& b)                                                          \
    {                                                                                                                                            \
        return {a OP b.TG_IMPL_MEMBER(TYPE, 0), a OP b.TG_IMPL_MEMBER(TYPE, 1), a OP b.TG_IMPL_MEMBER(TYPE, 2), a OP b.TG_IMPL_MEMBER(TYPE, 3)}; \
    }

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(TYPE)                                                                                                            \
    template <class ScalarT, class = std::enable_if_t<is_floating_point<ScalarT>>>                                                                           \
    constexpr TYPE<1, ScalarT> operator/(TYPE<1, ScalarT> const& a, ScalarT const& b)                                                                        \
    {                                                                                                                                                        \
        auto inv_b = ScalarT(1) / b;                                                                                                                         \
        return TYPE<1, ScalarT>{a.TG_IMPL_MEMBER(TYPE, 0) * inv_b};                                                                                                          \
    }                                                                                                                                                        \
    template <class ScalarT, class = std::enable_if_t<is_floating_point<ScalarT>>>                                                                           \
    constexpr TYPE<2, ScalarT> operator/(TYPE<2, ScalarT> const& a, ScalarT const& b)                                                                        \
    {                                                                                                                                                        \
        auto inv_b = ScalarT(1) / b;                                                                                                                         \
        return {a.TG_IMPL_MEMBER(TYPE, 0) * inv_b, a.TG_IMPL_MEMBER(TYPE, 1) * inv_b};                                                                       \
    }                                                                                                                                                        \
    template <class ScalarT, class = std::enable_if_t<is_floating_point<ScalarT>>>                                                                           \
    constexpr TYPE<3, ScalarT> operator/(TYPE<3, ScalarT> const& a, ScalarT const& b)                                                                        \
    {                                                                                                                                                        \
        auto inv_b = ScalarT(1) / b;                                                                                                                         \
        return {a.TG_IMPL_MEMBER(TYPE, 0) * inv_b, a.TG_IMPL_MEMBER(TYPE, 1) * inv_b, a.TG_IMPL_MEMBER(TYPE, 2) * inv_b};                                    \
    }                                                                                                                                                        \
    template <class ScalarT, class = std::enable_if_t<is_floating_point<ScalarT>>>                                                                           \
    constexpr TYPE<4, ScalarT> operator/(TYPE<4, ScalarT> const& a, ScalarT const& b)                                                                        \
    {                                                                                                                                                        \
        auto inv_b = ScalarT(1) / b;                                                                                                                         \
        return {a.TG_IMPL_MEMBER(TYPE, 0) * inv_b, a.TG_IMPL_MEMBER(TYPE, 1) * inv_b, a.TG_IMPL_MEMBER(TYPE, 2) * inv_b, a.TG_IMPL_MEMBER(TYPE, 3) * inv_b}; \
    }

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR(TYPE, OP) TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, OP) TG_IMPL_DEFINE_BINARY_OP_SCALAR_RIGHT(TYPE, OP)

#define TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(TYPE, FUN)                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<1, ScalarT> FUN(TYPE<1, ScalarT> const& a)                                                                                    \
    {                                                                                                                                            \
        return TYPE<1, ScalarT>{FUN(a.TG_IMPL_MEMBER(TYPE, 0))};                                                                                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<2, ScalarT> FUN(TYPE<2, ScalarT> const& a)                                                                                    \
    {                                                                                                                                            \
        return {FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1))};                                                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<3, ScalarT> FUN(TYPE<3, ScalarT> const& a)                                                                                    \
    {                                                                                                                                            \
        return {FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1)), FUN(a.TG_IMPL_MEMBER(TYPE, 2))};                                 \
    }                                                                                                                                            \
    template <class ScalarT>                                                                                                                     \
    constexpr TYPE<4, ScalarT> FUN(TYPE<4, ScalarT> const& a)                                                                                    \
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
