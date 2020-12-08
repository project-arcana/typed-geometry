#pragma once

#include <type_traits>

namespace tg
{
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using size_t = decltype(sizeof(0));

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

constexpr int min(int a, int b) { return a < b ? a : b; }
constexpr int max(int a, int b) { return a < b ? b : a; }

template <int D>
struct priority_tag : priority_tag<D - 1>
{
};
template <>
struct priority_tag<0>
{
};

// see https://stackoverflow.com/questions/44395169/why-is-sfinae-on-if-constexpr-not-allowed
template <template <class...> class, class, class...>
struct can_apply : std::false_type
{
};
template <template <class...> class Z, class... Ts>
struct can_apply<Z, std::void_t<Z<Ts...>>, Ts...> : std::true_type
{
};
} // namespace detail
template <template <class...> class Z, class... Ts>
constexpr bool can_apply = detail::can_apply<Z, void, Ts...>::value;

template <class...>
constexpr bool always_false = false;

template <class...>
using void_t = void;

template <class T>
struct dont_deduce_t
{
    using type = T;
};
template <class T>
using dont_deduce = typename dont_deduce_t<T>::type;

template <class A, class B>
struct pair
{
    A first;
    B second;

    template <class C, class D>
    constexpr bool operator==(pair<C, D> const& rhs) const noexcept
    {
        return first == rhs.first && second == rhs.second;
    }
    template <class C, class D>
    constexpr bool operator!=(pair<C, D> const& rhs) const noexcept
    {
        return first != rhs.first || second != rhs.second;
    }
};
template <class A, class B>
pair(A const&, B const&)->pair<A, B>;
template <class I, class A, class B>
constexpr void introspect(I&& i, pair<A, B>& p)
{
    i(p.first, "first");
    i(p.second, "second");
}

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

template <class T>
struct remove_reference_t
{
    using type = T;
};
template <class T>
struct remove_reference_t<T&>
{
    using type = T;
};
template <class T>
struct remove_reference_t<T&&>
{
    using type = T;
};
template <class T>
using remove_reference = typename remove_reference_t<T>::type;

template <class T>
struct remove_const_t
{
    using type = T;
};
template <class T>
struct remove_const_t<const T>
{
    using type = T;
};
template <class T>
using remove_const = typename remove_const_t<T>::type;

template <class T>
using remove_const_ref = remove_const<remove_reference<T>>;

template <class T>
constexpr T&& forward(remove_reference<T>& t) noexcept
{
    return static_cast<T&&>(t);
}
template <class T>
constexpr T&& forward(remove_reference<T>&& t) noexcept
{
    return static_cast<T&&>(t);
}

template <typename T>
constexpr remove_reference<T>&& move(T&& t) noexcept
{
    return static_cast<remove_reference<T>&&>(t);
}

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

template <class A, class B>
struct same_or_t
{
    using type = A;
};
template <class B>
struct same_or_t<void, B>
{
    using type = B;
};

// returns A == void ? A : B
template <class A, class B>
using same_or = typename same_or_t<A, B>::type;

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

namespace detail
{
template <class Container, class ElementT>
auto container_test(Container* c) -> decltype(static_cast<ElementT*>(c->data()), static_cast<decltype(sizeof(0))>(c->size()), 0);
template <class Container, class ElementT>
char container_test(...);

template <class Container, class ElementT, class = void>
struct is_range_t : false_type
{
};
template <class ElementT, size_t N>
struct is_range_t<ElementT[N], ElementT> : true_type
{
};
template <class ElementT, size_t N>
struct is_range_t<ElementT[N], ElementT const> : true_type
{
};
template <class ElementT, size_t N>
struct is_range_t<ElementT (&)[N], ElementT> : true_type
{
};
template <class ElementT, size_t N>
struct is_range_t<ElementT (&)[N], ElementT const> : true_type
{
};
template <class Container, class ElementT>
struct is_range_t<Container,
                  ElementT,
                  std::void_t<                                                              //
                      decltype(static_cast<ElementT&>(*std::declval<Container>().begin())), //
                      decltype(std::declval<Container>().end())                             //
                      >> : std::true_type
{
};

template <class Container, class = void>
struct is_any_range_t : false_type
{
};
template <class ElementT, size_t N>
struct is_any_range_t<ElementT[N]> : true_type
{
};
template <class ElementT, size_t N>
struct is_any_range_t<ElementT (&)[N]> : true_type
{
};
template <class Container>
struct is_any_range_t<Container,
                      std::void_t<                                     //
                          decltype(std::declval<Container>().begin()), //
                          decltype(std::declval<Container>().end())    //
                          >> : std::true_type
{
};
}

template <class Container, class ElementT>
static constexpr bool is_container = sizeof(detail::container_test<Container, ElementT>(nullptr)) == sizeof(int);

template <class Container, class ElementT>
static constexpr bool is_range = detail::is_range_t<Container, ElementT>::value;

template <class Container>
static constexpr bool is_any_range = detail::is_any_range_t<Container>::value;

template <class C>
constexpr auto begin(C& c) -> decltype(c.begin())
{
    return c.begin();
}
template <class C>
constexpr auto end(C& c) -> decltype(c.end())
{
    return c.end();
}

template <class C>
constexpr auto begin(C const& c) -> decltype(c.begin())
{
    return c.begin();
}
template <class C>
constexpr auto end(C const& c) -> decltype(c.end())
{
    return c.end();
}

template <class T, size_t N>
constexpr T* begin(T (&array)[N])
{
    return array;
}
template <class T, size_t N>
constexpr T* end(T (&array)[N])
{
    return array + N;
}

template <class ContainerT>
constexpr auto container_size(ContainerT&& c) -> decltype(c.size())
{
    return c.size();
}
template <class T, size_t N>
constexpr size_t container_size(T (&)[N])
{
    return N;
}

template <class Range>
struct element_type_t
{
    static Range make_range();

    using type = remove_reference<decltype(*tg::begin(make_range()))>;
};
template <class T, size_t N>
struct element_type_t<T[N]>
{
    using type = T;
};
template <class Range>
using element_type = typename element_type_t<Range>::type;

struct identity_fun
{
    template <class T>
    constexpr T&& operator()(T&& v) const noexcept
    {
        return forward<T>(v);
    }
};

namespace detail
{
// in detail ns to avoid ambiguity clash with std::swap
template <class T>
void swap(T& a, T& b)
{
    T tmp = static_cast<T&&>(a);
    a = static_cast<T&&>(b);
    b = static_cast<T&&>(tmp);
}

template <u64 N, u64 Alignment>
auto helper_size_t()
{
    if constexpr (N < (1 << 8) && Alignment <= 1)
        return u8{};
    else if constexpr (N < (1 << 16) && Alignment <= 2)
        return u16{};
    else if constexpr (N < (1uLL << 32) && Alignment <= 4)
        return u32{};
    else
        return u64{};
}

template <u64 N, u64 Alignment>
using size_t_for = decltype(helper_size_t<N, Alignment>());

/// Indirection workaround for a current MSVC compiler bug (19.23)
/// without indirection: https://godbolt.org/z/iQ19yj
/// with indirection: https://godbolt.org/z/6MoWE4
/// Bug report: https://developercommunity.visualstudio.com/content/problem/800899/false-positive-for-c2975-on-alias-template-fixed-w.html
template <class T, size_t N>
using compact_size_t_typed = size_t_for<N, alignof(T)>;

// adapted from https://stackoverflow.com/questions/23999573/convert-a-number-to-a-string-literal-with-constexpr
template <unsigned... digits>
struct digits_to_string_literal
{
    static const char value[];
};

template <unsigned... digits>
constexpr char digits_to_string_literal<digits...>::value[] = {('0' + digits)..., 0};

template <unsigned rem, unsigned... digits>
struct number_to_string_literal_t : number_to_string_literal_t<rem / 10, rem % 10, digits...>
{
};

template <unsigned... digits>
struct number_to_string_literal_t<0, digits...> : digits_to_string_literal<digits...>
{
};

template <unsigned num>
inline constexpr char const* number_to_string_literal = number_to_string_literal_t<num>::value;
}
} // namespace tg
