#pragma once

namespace tg
{
namespace detail
{
struct unused;

struct true_type
{
    static constexpr bool value = true;
};
struct false_type
{
    static constexpr bool value = false;
};

template <class T>
auto try_add_lvalue_reference(int) -> T&;
template <class T>
auto try_add_lvalue_reference(...) -> T;

template <class T>
auto try_add_rvalue_reference(int) -> T&&;
template <class T>
auto try_add_rvalue_reference(...) -> T;

inline constexpr int min(int a, int b) { return a < b ? a : b; }
inline constexpr int max(int a, int b) { return a < b ? b : a; }

template <int D>
struct priority_tag : priority_tag<D - 1>
{
};
template <>
struct priority_tag<0>
{
};
} // namespace detail

template <class A, class B>
struct pair
{
    A first;
    B second;
};

template <class A, class B, class C>
struct triple
{
    A first;
    B second;
    C third;
};

template <bool, class T = void>
struct enable_if_t
{
};
template <class T>
struct enable_if_t<true, T>
{
    using type = T;
};
template <bool cond, class T = void>
using enable_if = typename enable_if_t<cond, T>::type;

template <class A, class B>
struct is_same_t
{
    static constexpr bool value = false;
};
template <class A>
struct is_same_t<A, A>
{
    static constexpr bool value = true;
};
template <class A, class B>
static constexpr bool is_same = is_same_t<A, B>::value;

template <class To, class From>
To bit_cast(From f)
{
    static_assert(sizeof(From) == sizeof(To), "can only bitcast between same-size types");

    // NOTE: std::memcpy includes an std header which we want to avoid
    union {
        From from;
        To to;
    } u;
    u.from = f;
    return u.to;
}

template <class T>
using add_lvalue_reference = decltype(detail::try_add_lvalue_reference<T>(0));

template <class T>
using add_rvalue_reference = decltype(detail::try_add_rvalue_reference<T>(0));

// must be a function because it's an lvalue otherwise
template <class T>
static constexpr add_rvalue_reference<T> declval() noexcept
{
    return *reinterpret_cast<T*>(nullptr);
}

template <bool B, class T, class F>
struct conditional_type_t
{
    using type = T;
};

template <class T, class F>
struct conditional_type_t<false, T, F>
{
    using type = F;
};

template <bool B, class T, class F>
using conditional_type = typename conditional_type_t<B, T, F>::type;

} // namespace tg
