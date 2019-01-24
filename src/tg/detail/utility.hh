#pragma once

namespace tg
{
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

template <class T, int SIZE>
struct array
{
    T values[SIZE];

    constexpr T& operator[](int i) { return values[i]; }
    constexpr T const& operator[](int i) const { return values[i]; }

    constexpr T* begin() { return &values[0]; }
    constexpr T* end() { return begin() + SIZE; }
    constexpr T const* begin() const { return &values[0]; }
    constexpr T const* end() const { return begin() + SIZE; }

    constexpr int size() const { return SIZE; }
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
} // namespace tg
