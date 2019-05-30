#pragma once

#include <typed-geometry/types/angle.hh>
#include <typed-geometry/detail/scalar_traits.hh>

namespace tg
{
template <class A>
[[nodiscard]] constexpr angle_t<A> operator-(angle_t<A> const& a)
{
    return radians(-a.radians());
}
template <class A>
[[nodiscard]] constexpr angle_t<A> operator+(angle_t<A> const& a)
{
    return a;
}

template <class A, class B, class R = promoted_scalar<A, B>>
[[nodiscard]] constexpr angle_t<R> operator+(angle_t<A> a, angle_t<B> b)
{
    return radians(a.radians() + b.radians());
}
template <class A, class B, class R = promoted_scalar<A, B>>
[[nodiscard]] constexpr angle_t<R> operator-(angle_t<A> a, angle_t<B> b)
{
    return radians(a.radians() - b.radians());
}

template <class A, class B, class R = promoted_scalar<A, B>>
[[nodiscard]] constexpr angle_t<R> operator*(angle_t<A> a, B b)
{
    return radians(a.radians() * b);
}
template <class A, class B, class R = promoted_scalar<A, B>>
[[nodiscard]] constexpr angle_t<R> operator/(angle_t<A> a, B b)
{
    return radians(a.radians() / b);
}

template <class A, class B, class R = promoted_scalar<A, B>>
[[nodiscard]] constexpr angle_t<R> operator*(A a, angle_t<B> b)
{
    return radians(a * b.radians());
}

template <class A, class B, class R = promoted_scalar<A, B>>
[[nodiscard]] constexpr R operator/(angle_t<A> a, angle_t<B> b)
{
    return a.radians() / b.radians();
}
} // namespace tg
