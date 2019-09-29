#pragma once

namespace tg
{
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
} // namespace detail

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
};
template <class A, class B>
pair(A const&, B const&)->pair<A, B>;

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
}

template <class Container, class ElementT>
static constexpr bool is_container = sizeof(detail::container_test<Container, ElementT>(nullptr)) == sizeof(int);

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
}

} // namespace tg
