#pragma once

// =========
// configurations

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

#define TG_FORCE_SEMICOLON static_assert(true)

#define TG_IMPL_STRINGIFY(expr) #expr
#define TG_STRINGIFY(expr) TG_IMPL_STRINGIFY(expr)

// =========
// operations and classes

#define TG_IMPL_MEMBER_pos_0 x
#define TG_IMPL_MEMBER_pos_1 y
#define TG_IMPL_MEMBER_pos_2 z
#define TG_IMPL_MEMBER_pos_3 w
#define TG_IMPL_MEMBER_pos_s v
#define TG_IMPL_MEMBER_pos_verify

#define TG_IMPL_MEMBER_vec_0 x
#define TG_IMPL_MEMBER_vec_1 y
#define TG_IMPL_MEMBER_vec_2 z
#define TG_IMPL_MEMBER_vec_3 w
#define TG_IMPL_MEMBER_vec_s v
#define TG_IMPL_MEMBER_vec_verify

#define TG_IMPL_MEMBER_dir_0 x
#define TG_IMPL_MEMBER_dir_1 y
#define TG_IMPL_MEMBER_dir_2 z
#define TG_IMPL_MEMBER_dir_3 w
#define TG_IMPL_MEMBER_dir_s v
#define TG_IMPL_MEMBER_dir_verify TG_CONTRACT(tg::detail::is_dir_valid(*this) && "dirs must be normalized");

#define TG_IMPL_MEMBER_color_0 r
#define TG_IMPL_MEMBER_color_1 g
#define TG_IMPL_MEMBER_color_2 b
#define TG_IMPL_MEMBER_color_3 a
#define TG_IMPL_MEMBER_color_s grey
#define TG_IMPL_MEMBER_color_verify

#define TG_IMPL_MEMBER_size_0 width
#define TG_IMPL_MEMBER_size_1 height
#define TG_IMPL_MEMBER_size_2 depth
#define TG_IMPL_MEMBER_size_3 w
#define TG_IMPL_MEMBER_size_s v
#define TG_IMPL_MEMBER_size_verify

#define TG_IMPL_MEMBER_comp_0 comp0
#define TG_IMPL_MEMBER_comp_1 comp1
#define TG_IMPL_MEMBER_comp_2 comp2
#define TG_IMPL_MEMBER_comp_3 comp3
#define TG_IMPL_MEMBER_comp_s v
#define TG_IMPL_MEMBER_comp_verify

#define TG_IMPL_MEMBER_quaternion_0 x
#define TG_IMPL_MEMBER_quaternion_1 y
#define TG_IMPL_MEMBER_quaternion_2 z
#define TG_IMPL_MEMBER_quaternion_3 w
#define TG_IMPL_MEMBER_quaternion_s v
#define TG_IMPL_MEMBER_quaternion_verify

#define TG_IMPL_MEMBER_mat_0 m[0]
#define TG_IMPL_MEMBER_mat_1 m[1]
#define TG_IMPL_MEMBER_mat_2 m[2]
#define TG_IMPL_MEMBER_mat_3 m[3]

#define TG_IMPL_MEMBER(TYPE, COMP) TG_IMPL_MEMBER_##TYPE##_##COMP
#define TG_IMPL_COMP_VERIFY(TYPE) TG_IMPL_MEMBER_##TYPE##_verify

#define TG_DECLARE_COMP_SUBSCRIPT_OP(TYPE)                                                     \
    constexpr ScalarT& operator[](int i) { return (&TG_IMPL_MEMBER(TYPE, 0))[i]; }             \
    constexpr ScalarT const& operator[](int i) const { return (&TG_IMPL_MEMBER(TYPE, 0))[i]; } \
    TG_FORCE_SEMICOLON

#define TG_DECLARE_SELF_SWIZZLE(TYPE)                                                                                     \
    template <int... Indices>                                                                                             \
    [[nodiscard]] constexpr auto operator[](swizzler<Indices...> const& swizzle)                                          \
    {                                                                                                                     \
        return swizzle[*this];                                                                                            \
    }                                                                                                                     \
    template <int... Indices>                                                                                             \
    [[nodiscard]] constexpr auto operator[](swizzler<Indices...> const& swizzle) const                                    \
    {                                                                                                                     \
        return swizzle[*this];                                                                                            \
    }                                                                                                                     \
    template <int... Indices>                                                                                             \
    [[nodiscard]] constexpr TYPE<sizeof...(Indices), ScalarT> const operator()(swizzler<Indices...> const& swizzle) const \
    {                                                                                                                     \
        return swizzle(*this);                                                                                            \
    }                                                                                                                     \
    TG_FORCE_SEMICOLON

#define TG_DECLARE_COMP_TYPE_1(TYPE)                                                                                                 \
    TG_DECLARE_COMP_SUBSCRIPT_OP(TYPE);                                                                                              \
    TG_DECLARE_SELF_SWIZZLE(TYPE);                                                                                                   \
    constexpr TYPE() = default;                                                                                                      \
    constexpr TYPE(ScalarT TG_IMPL_MEMBER(TYPE, s)) : TG_IMPL_MEMBER(TYPE, 0)(TG_IMPL_MEMBER(TYPE, s)) { TG_IMPL_COMP_VERIFY(TYPE) } \
    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>                                                       \
    explicit constexpr TYPE(Obj const& v, ScalarT fill = ScalarT(0))                                                                 \
    {                                                                                                                                \
        auto s = detail::get_dynamic_comp_size(v);                                                                                   \
        TG_IMPL_MEMBER(TYPE, 0) = detail::comp_get(v, 0, s, fill);                                                                   \
        TG_IMPL_COMP_VERIFY(TYPE)                                                                                                    \
    }                                                                                                                                \
    static constexpr TYPE from_data(ScalarT const* data) { return {data[0]}; }                                                       \
    TG_FORCE_SEMICOLON

#define TG_DECLARE_COMP_TYPE_2(TYPE)                                                                       \
    TG_DECLARE_COMP_SUBSCRIPT_OP(TYPE);                                                                    \
    TG_DECLARE_SELF_SWIZZLE(TYPE);                                                                         \
    constexpr TYPE() = default;                                                                            \
    constexpr explicit TYPE(ScalarT TG_IMPL_MEMBER(TYPE, s))                                               \
      : TG_IMPL_MEMBER(TYPE, 0)(TG_IMPL_MEMBER(TYPE, s)), TG_IMPL_MEMBER(TYPE, 1)(TG_IMPL_MEMBER(TYPE, s)) \
    {                                                                                                      \
        TG_IMPL_COMP_VERIFY(TYPE)                                                                          \
    }                                                                                                      \
    constexpr TYPE(ScalarT TG_IMPL_MEMBER(TYPE, 0), ScalarT TG_IMPL_MEMBER(TYPE, 1))                       \
      : TG_IMPL_MEMBER(TYPE, 0)(TG_IMPL_MEMBER(TYPE, 0)), TG_IMPL_MEMBER(TYPE, 1)(TG_IMPL_MEMBER(TYPE, 1)) \
    {                                                                                                      \
        TG_IMPL_COMP_VERIFY(TYPE)                                                                          \
    }                                                                                                      \
    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>                             \
    explicit constexpr TYPE(Obj const& v, ScalarT fill = ScalarT(0))                                       \
    {                                                                                                      \
        auto s = detail::get_dynamic_comp_size(v);                                                         \
        TG_IMPL_MEMBER(TYPE, 0) = detail::comp_get(v, 0, s, fill);                                         \
        TG_IMPL_MEMBER(TYPE, 1) = detail::comp_get(v, 1, s, fill);                                         \
        TG_IMPL_COMP_VERIFY(TYPE)                                                                          \
    }                                                                                                      \
    static constexpr TYPE from_data(ScalarT const* data) { return {data[0], data[1]}; }                    \
    TG_FORCE_SEMICOLON

#define TG_DECLARE_COMP_TYPE_3(TYPE)                                                                                                                         \
    TG_DECLARE_COMP_SUBSCRIPT_OP(TYPE);                                                                                                                      \
    TG_DECLARE_SELF_SWIZZLE(TYPE);                                                                                                                           \
    constexpr TYPE() = default;                                                                                                                              \
    constexpr explicit TYPE(ScalarT TG_IMPL_MEMBER(TYPE, s))                                                                                                 \
      : TG_IMPL_MEMBER(TYPE, 0)(TG_IMPL_MEMBER(TYPE, s)), TG_IMPL_MEMBER(TYPE, 1)(TG_IMPL_MEMBER(TYPE, s)), TG_IMPL_MEMBER(TYPE, 2)(TG_IMPL_MEMBER(TYPE, s)) \
    {                                                                                                                                                        \
        TG_IMPL_COMP_VERIFY(TYPE)                                                                                                                            \
    }                                                                                                                                                        \
    constexpr TYPE(ScalarT TG_IMPL_MEMBER(TYPE, 0), ScalarT TG_IMPL_MEMBER(TYPE, 1), ScalarT TG_IMPL_MEMBER(TYPE, 2))                                        \
      : TG_IMPL_MEMBER(TYPE, 0)(TG_IMPL_MEMBER(TYPE, 0)), TG_IMPL_MEMBER(TYPE, 1)(TG_IMPL_MEMBER(TYPE, 1)), TG_IMPL_MEMBER(TYPE, 2)(TG_IMPL_MEMBER(TYPE, 2)) \
    {                                                                                                                                                        \
        TG_IMPL_COMP_VERIFY(TYPE)                                                                                                                            \
    }                                                                                                                                                        \
    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>                                                                               \
    explicit constexpr TYPE(Obj const& v, ScalarT fill = ScalarT(0))                                                                                         \
    {                                                                                                                                                        \
        auto s = detail::get_dynamic_comp_size(v);                                                                                                           \
        TG_IMPL_MEMBER(TYPE, 0) = detail::comp_get(v, 0, s, fill);                                                                                           \
        TG_IMPL_MEMBER(TYPE, 1) = detail::comp_get(v, 1, s, fill);                                                                                           \
        TG_IMPL_MEMBER(TYPE, 2) = detail::comp_get(v, 2, s, fill);                                                                                           \
        TG_IMPL_COMP_VERIFY(TYPE)                                                                                                                            \
    }                                                                                                                                                        \
    static constexpr TYPE from_data(ScalarT const* data) { return {data[0], data[1], data[2]}; }                                                             \
    TG_FORCE_SEMICOLON

#define TG_DECLARE_COMP_TYPE_4_NO_SWIZZLE(TYPE)                                                                                                        \
    TG_DECLARE_COMP_SUBSCRIPT_OP(TYPE);                                                                                                                \
    constexpr TYPE() = default;                                                                                                                        \
    constexpr explicit TYPE(ScalarT TG_IMPL_MEMBER(TYPE, s))                                                                                           \
      : TG_IMPL_MEMBER(TYPE, 0)(TG_IMPL_MEMBER(TYPE, s)),                                                                                              \
        TG_IMPL_MEMBER(TYPE, 1)(TG_IMPL_MEMBER(TYPE, s)),                                                                                              \
        TG_IMPL_MEMBER(TYPE, 2)(TG_IMPL_MEMBER(TYPE, s)),                                                                                              \
        TG_IMPL_MEMBER(TYPE, 3)(TG_IMPL_MEMBER(TYPE, s))                                                                                               \
    {                                                                                                                                                  \
        TG_IMPL_COMP_VERIFY(TYPE)                                                                                                                      \
    }                                                                                                                                                  \
    constexpr TYPE(ScalarT TG_IMPL_MEMBER(TYPE, 0), ScalarT TG_IMPL_MEMBER(TYPE, 1), ScalarT TG_IMPL_MEMBER(TYPE, 2), ScalarT TG_IMPL_MEMBER(TYPE, 3)) \
      : TG_IMPL_MEMBER(TYPE, 0)(TG_IMPL_MEMBER(TYPE, 0)),                                                                                              \
        TG_IMPL_MEMBER(TYPE, 1)(TG_IMPL_MEMBER(TYPE, 1)),                                                                                              \
        TG_IMPL_MEMBER(TYPE, 2)(TG_IMPL_MEMBER(TYPE, 2)),                                                                                              \
        TG_IMPL_MEMBER(TYPE, 3)(TG_IMPL_MEMBER(TYPE, 3))                                                                                               \
    {                                                                                                                                                  \
        TG_IMPL_COMP_VERIFY(TYPE)                                                                                                                      \
    }                                                                                                                                                  \
    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>                                                                         \
    explicit constexpr TYPE(Obj const& v, ScalarT fill = ScalarT(0))                                                                                   \
    {                                                                                                                                                  \
        auto s = detail::get_dynamic_comp_size(v);                                                                                                     \
        TG_IMPL_MEMBER(TYPE, 0) = detail::comp_get(v, 0, s, fill);                                                                                     \
        TG_IMPL_MEMBER(TYPE, 1) = detail::comp_get(v, 1, s, fill);                                                                                     \
        TG_IMPL_MEMBER(TYPE, 2) = detail::comp_get(v, 2, s, fill);                                                                                     \
        TG_IMPL_MEMBER(TYPE, 3) = detail::comp_get(v, 3, s, fill);                                                                                     \
        TG_IMPL_COMP_VERIFY(TYPE)                                                                                                                      \
    }                                                                                                                                                  \
    static constexpr TYPE from_data(ScalarT const* data) { return {data[0], data[1], data[2], data[3]}; }                                              \
    TG_FORCE_SEMICOLON

#define TG_DECLARE_COMP_TYPE_4(TYPE)         \
    TG_DECLARE_COMP_TYPE_4_NO_SWIZZLE(TYPE); \
    TG_DECLARE_SELF_SWIZZLE(TYPE)

#define TG_IMPL_DEFINE_UNARY_OP(TYPE, OP)                                                                                                    \
    template <int D, class ScalarT>                                                                                                          \
    [[nodiscard]] constexpr TYPE<D, ScalarT> operator OP(TYPE<D, ScalarT> const& a)                                                          \
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

#define TG_IMPL_DEFINE_BINARY_OP(TYPE_A, TYPE_B, TYPE_R, OP)                                                                                 \
    template <int D, class ScalarT>                                                                                                          \
    [[nodiscard]] constexpr TYPE_R<D, ScalarT> operator OP(TYPE_A<D, ScalarT> const& a, TYPE_B<D, ScalarT> const& b)                         \
    {                                                                                                                                        \
        if constexpr (D == 1)                                                                                                                \
            return {a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0)};                                                             \
                                                                                                                                             \
        else if constexpr (D == 2)                                                                                                           \
            return {a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0), a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1)}; \
                                                                                                                                             \
        else if constexpr (D == 3)                                                                                                           \
            return {a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0), a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1),  \
                    a.TG_IMPL_MEMBER(TYPE_A, 2) OP b.TG_IMPL_MEMBER(TYPE_B, 2)};                                                             \
                                                                                                                                             \
        else if constexpr (D == 4)                                                                                                           \
            return {a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0), a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1),  \
                    a.TG_IMPL_MEMBER(TYPE_A, 2) OP b.TG_IMPL_MEMBER(TYPE_B, 2), a.TG_IMPL_MEMBER(TYPE_A, 3) OP b.TG_IMPL_MEMBER(TYPE_B, 3)}; \
                                                                                                                                             \
        else                                                                                                                                 \
            static_assert(always_false<ScalarT>, "only supported up to 4 dimensions");                                                       \
    }                                                                                                                                        \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_RIGHT(TYPE, OP)                                                                                              \
    template <int D, class ScalarT>                                                                                                                  \
    [[nodiscard]] constexpr TYPE<D, ScalarT> operator OP(TYPE<D, ScalarT> const& a, dont_deduce<ScalarT> const& b)                                   \
    {                                                                                                                                                \
        if constexpr (D == 1)                                                                                                                        \
            return {a.TG_IMPL_MEMBER(TYPE, 0) OP b};                                                                                                 \
        else if constexpr (D == 2)                                                                                                                   \
            return {a.TG_IMPL_MEMBER(TYPE, 0) OP b, a.TG_IMPL_MEMBER(TYPE, 1) OP b};                                                                 \
        else if constexpr (D == 3)                                                                                                                   \
            return {a.TG_IMPL_MEMBER(TYPE, 0) OP b, a.TG_IMPL_MEMBER(TYPE, 1) OP b, a.TG_IMPL_MEMBER(TYPE, 2) OP b};                                 \
        else if constexpr (D == 4)                                                                                                                   \
            return {a.TG_IMPL_MEMBER(TYPE, 0) OP b, a.TG_IMPL_MEMBER(TYPE, 1) OP b, a.TG_IMPL_MEMBER(TYPE, 2) OP b, a.TG_IMPL_MEMBER(TYPE, 3) OP b}; \
        else                                                                                                                                         \
            static_assert(always_false<ScalarT>, "only supported up to 4 dimensions");                                                               \
    }                                                                                                                                                \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, OP)                                                                                               \
    template <int D, class ScalarT>                                                                                                                  \
    [[nodiscard]] constexpr TYPE<D, ScalarT> operator OP(dont_deduce<ScalarT> const& a, TYPE<D, ScalarT> const& b)                                   \
    {                                                                                                                                                \
        if constexpr (D == 1)                                                                                                                        \
            return {a OP b.TG_IMPL_MEMBER(TYPE, 0)};                                                                                                 \
        else if constexpr (D == 2)                                                                                                                   \
            return {a OP b.TG_IMPL_MEMBER(TYPE, 0), a OP b.TG_IMPL_MEMBER(TYPE, 1)};                                                                 \
        else if constexpr (D == 3)                                                                                                                   \
            return {a OP b.TG_IMPL_MEMBER(TYPE, 0), a OP b.TG_IMPL_MEMBER(TYPE, 1), a OP b.TG_IMPL_MEMBER(TYPE, 2)};                                 \
        else if constexpr (D == 4)                                                                                                                   \
            return {a OP b.TG_IMPL_MEMBER(TYPE, 0), a OP b.TG_IMPL_MEMBER(TYPE, 1), a OP b.TG_IMPL_MEMBER(TYPE, 2), a OP b.TG_IMPL_MEMBER(TYPE, 3)}; \
        else                                                                                                                                         \
            static_assert(always_false<ScalarT>, "only supported up to 4 dimensions");                                                               \
    }                                                                                                                                                \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(TYPE)                                                                                                \
    /* scalar / type */                                                                                                                          \
    TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, /);                                                                                               \
    template <int D, class ScalarT>                                                                                                              \
    [[nodiscard]] constexpr TYPE<D, ScalarT> operator/(TYPE<D, ScalarT> const& a, dont_deduce<ScalarT> const& b)                                 \
    {                                                                                                                                            \
        if constexpr (is_floating_point<ScalarT>)                                                                                                \
            return a * ScalarT(1 / b);                                                                                                           \
        else if constexpr (D == 1)                                                                                                               \
            return {a.TG_IMPL_MEMBER(TYPE, 0) / b};                                                                                              \
        else if constexpr (D == 2)                                                                                                               \
            return {a.TG_IMPL_MEMBER(TYPE, 0) / b, a.TG_IMPL_MEMBER(TYPE, 1) / b};                                                               \
        else if constexpr (D == 3)                                                                                                               \
            return {a.TG_IMPL_MEMBER(TYPE, 0) / b, a.TG_IMPL_MEMBER(TYPE, 1) / b, a.TG_IMPL_MEMBER(TYPE, 2) / b};                                \
        else if constexpr (D == 4)                                                                                                               \
            return {a.TG_IMPL_MEMBER(TYPE, 0) / b, a.TG_IMPL_MEMBER(TYPE, 1) / b, a.TG_IMPL_MEMBER(TYPE, 2) / b, a.TG_IMPL_MEMBER(TYPE, 3) / b}; \
        else                                                                                                                                     \
            static_assert(always_false<ScalarT>, "only supported up to 4 dimensions");                                                           \
    }                                                                                                                                            \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_BINARY_OP_SCALAR(TYPE, OP)   \
    TG_IMPL_DEFINE_BINARY_OP_SCALAR_LEFT(TYPE, OP); \
    TG_IMPL_DEFINE_BINARY_OP_SCALAR_RIGHT(TYPE, OP)

#define TG_IMPL_DEFINE_COMPWISE_UNARY_TO_COMP(TYPE, FUN)                                                                                                       \
    template <int D, class ScalarT>                                                                                                                            \
    [[nodiscard]] constexpr auto FUN(TYPE<D, ScalarT> const& a)                                                                                                \
    {                                                                                                                                                          \
        using R = remove_const_ref<decltype(FUN(a.TG_IMPL_MEMBER(TYPE, 0)))>;                                                                                  \
                                                                                                                                                               \
        if constexpr (D == 1)                                                                                                                                  \
            return comp<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0))};                                                                                                 \
                                                                                                                                                               \
        else if constexpr (D == 2)                                                                                                                             \
            return comp<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1))};                                                                 \
                                                                                                                                                               \
        else if constexpr (D == 3)                                                                                                                             \
            return comp<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1)), FUN(a.TG_IMPL_MEMBER(TYPE, 2))};                                 \
                                                                                                                                                               \
        else if constexpr (D == 4)                                                                                                                             \
            return comp<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0)), FUN(a.TG_IMPL_MEMBER(TYPE, 1)), FUN(a.TG_IMPL_MEMBER(TYPE, 2)), FUN(a.TG_IMPL_MEMBER(TYPE, 3))}; \
                                                                                                                                                               \
        else                                                                                                                                                   \
            static_assert(always_false<ScalarT>, "only supported up to 4 dimensions");                                                                         \
    }                                                                                                                                                          \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(TYPE, FUN)                                                                                                          \
    template <int D, class ScalarT>                                                                                                                            \
    [[nodiscard]] constexpr auto FUN(TYPE<D, ScalarT> const& a)                                                                                                \
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
    [[nodiscard]] constexpr auto FUN(TYPE<D, ScalarT> const& a, TYPE<D, ScalarT> const& b)                                                           \
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

#define TG_IMPL_DEFINE_COMPWISE_FUNC_TERNARY(TYPE, FUN)                                                                             \
    template <int D, class ScalarT>                                                                                                 \
    [[nodiscard]] constexpr auto FUN(TYPE<D, ScalarT> const& a, TYPE<D, ScalarT> const& b, TYPE<D, ScalarT> const& c)               \
    {                                                                                                                               \
        using R = remove_const_ref<decltype(FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0), c.TG_IMPL_MEMBER(TYPE, 0)))>; \
                                                                                                                                    \
        if constexpr (D == 1)                                                                                                       \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0), c.TG_IMPL_MEMBER(TYPE, 0))};                \
                                                                                                                                    \
        else if constexpr (D == 2)                                                                                                  \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0), c.TG_IMPL_MEMBER(TYPE, 0)),                 \
                              FUN(a.TG_IMPL_MEMBER(TYPE, 1), b.TG_IMPL_MEMBER(TYPE, 1), c.TG_IMPL_MEMBER(TYPE, 1))};                \
                                                                                                                                    \
        else if constexpr (D == 3)                                                                                                  \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0), c.TG_IMPL_MEMBER(TYPE, 0)),                 \
                              FUN(a.TG_IMPL_MEMBER(TYPE, 1), b.TG_IMPL_MEMBER(TYPE, 1), c.TG_IMPL_MEMBER(TYPE, 1)),                 \
                              FUN(a.TG_IMPL_MEMBER(TYPE, 2), b.TG_IMPL_MEMBER(TYPE, 2), c.TG_IMPL_MEMBER(TYPE, 2))};                \
                                                                                                                                    \
        else if constexpr (D == 4)                                                                                                  \
            return TYPE<D, R>{FUN(a.TG_IMPL_MEMBER(TYPE, 0), b.TG_IMPL_MEMBER(TYPE, 0), c.TG_IMPL_MEMBER(TYPE, 0)),                 \
                              FUN(a.TG_IMPL_MEMBER(TYPE, 1), b.TG_IMPL_MEMBER(TYPE, 1), c.TG_IMPL_MEMBER(TYPE, 1)),                 \
                              FUN(a.TG_IMPL_MEMBER(TYPE, 2), b.TG_IMPL_MEMBER(TYPE, 2), c.TG_IMPL_MEMBER(TYPE, 2)),                 \
                              FUN(a.TG_IMPL_MEMBER(TYPE, 3), b.TG_IMPL_MEMBER(TYPE, 3), c.TG_IMPL_MEMBER(TYPE, 3))};                \
                                                                                                                                    \
        else                                                                                                                        \
            static_assert(always_false<ScalarT>, "only supported up to 4 dimensions");                                              \
    }                                                                                                                               \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_REDUCTION_OP_BINARY(TYPE_A, TYPE_B, RESULT_T, NAME, REDUCE, OP)                                                                     \
    template <int D, class ScalarT>                                                                                                                        \
    [[nodiscard]] constexpr RESULT_T NAME(TYPE_A<D, ScalarT> const& a, TYPE_B<D, ScalarT> const& b)                                                        \
    {                                                                                                                                                      \
        if constexpr (D == 1)                                                                                                                              \
            return a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0);                                                                             \
                                                                                                                                                           \
        else if constexpr (D == 2)                                                                                                                         \
            return (a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1));         \
                                                                                                                                                           \
        else if constexpr (D == 3)                                                                                                                         \
            return (a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1))          \
                REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 2) OP b.TG_IMPL_MEMBER(TYPE_B, 2));                                                                        \
                                                                                                                                                           \
        else if constexpr (D == 4)                                                                                                                         \
            return ((a.TG_IMPL_MEMBER(TYPE_A, 0) OP b.TG_IMPL_MEMBER(TYPE_B, 0))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 1) OP b.TG_IMPL_MEMBER(TYPE_B, 1)))REDUCE( \
                (a.TG_IMPL_MEMBER(TYPE_A, 2) OP b.TG_IMPL_MEMBER(TYPE_B, 2))REDUCE(a.TG_IMPL_MEMBER(TYPE_A, 3) OP b.TG_IMPL_MEMBER(TYPE_B, 3)));           \
                                                                                                                                                           \
        else                                                                                                                                               \
            static_assert(always_false<ScalarT>, "only supported up to 4 dimensions");                                                                     \
    }                                                                                                                                                      \
    TG_FORCE_SEMICOLON

#define TG_IMPL_DEFINE_ASSIGNMENT_OP(TYPE_THIS, OP)                                                                           \
    template <int D, class ScalarA, class T>                                                                                  \
    constexpr auto operator OP##=(TYPE_THIS<D, ScalarA>& lhs, T const& rhs)->decltype(TYPE_THIS<D, ScalarA>(lhs OP rhs), lhs) \
    {                                                                                                                         \
        lhs = TYPE_THIS<D, ScalarA>(lhs OP rhs);                                                                              \
        return lhs;                                                                                                           \
    }                                                                                                                         \
    TG_FORCE_SEMICOLON

#define TG_IMPL_COMP_DEDUCTION_GUIDES(type)                                                 \
    template <class A, class = enable_if<is_scalar<A>>>                                     \
    type(A const& x)->type<1, A>;                                                           \
    template <class A, class B>                                                             \
    type(A const& x, B const& y)->type<2, decltype(x + y)>;                                 \
    template <class A, class B, class C>                                                    \
    type(A const& x, B const& y, C const& z)->type<3, decltype(x + y + z)>;                 \
    template <class A, class B, class C, class D>                                           \
    type(A const& x, B const& y, C const& z, D const& w)->type<4, decltype(x + y + z + w)>; \
                                                                                            \
    template <u64 D, class T>                                                               \
    type(array<T, D> const&)->type<int(D), T>;                                              \
    template <int D, class T>                                                               \
    type(vec<D, T> const&)->type<D, T>;                                                     \
    template <int D, class T>                                                               \
    type(color<D, T> const&)->type<D, T>;                                                   \
    template <int D, class T>                                                               \
    type(comp<D, T> const&)->type<D, T>;                                                    \
    template <int D, class T>                                                               \
    type(pos<D, T> const&)->type<D, T>;                                                     \
    template <int D, class T>                                                               \
    type(size<D, T> const&)->type<D, T>;                                                    \
    template <int D, class T>                                                               \
    type(dir<D, T> const&)->type<D, T> // enforce ;

#define TG_IMPL_COMP_INTROSPECT(type)                                            \
    template <class I, int D, class ScalarT>                                     \
    constexpr void introspect(I&& i, type<D, ScalarT>& v)                        \
    {                                                                            \
        i(v.TG_IMPL_MEMBER(type, 0), TG_STRINGIFY(TG_IMPL_MEMBER(type, 0)));     \
        if constexpr (D >= 2)                                                    \
            i(v.TG_IMPL_MEMBER(type, 1), TG_STRINGIFY(TG_IMPL_MEMBER(type, 1))); \
        if constexpr (D >= 3)                                                    \
            i(v.TG_IMPL_MEMBER(type, 2), TG_STRINGIFY(TG_IMPL_MEMBER(type, 2))); \
        if constexpr (D >= 4)                                                    \
            i(v.TG_IMPL_MEMBER(type, 3), TG_STRINGIFY(TG_IMPL_MEMBER(type, 3))); \
    }                                                                            \
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
