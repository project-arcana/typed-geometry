#pragma once

#include "../../types/angle.hh"
#include "../scalar_traits.hh"

namespace tg
{
template <class A>
TG_NODISCARD constexpr angle<A> operator-(angle<A> const& a)
{
    return radians(-a.radians());
}
template <class A>
TG_NODISCARD constexpr angle<A> operator+(angle<A> const& a)
{
    return a;
}

template <class A, class B, class R = promoted_scalar<A, B>>
TG_NODISCARD constexpr angle<R> operator+(angle<A> a, angle<B> b)
{
    return radians(a.radians() + b.radians());
}
template <class A, class B, class R = promoted_scalar<A, B>>
TG_NODISCARD constexpr angle<R> operator-(angle<A> a, angle<B> b)
{
    return radians(a.radians() - b.radians());
}

template <class A, class B, class R = promoted_scalar<A, B>>
TG_NODISCARD constexpr angle<R> operator*(angle<A> a, B b)
{
    return radians(a.radians() * b);
}
template <class A, class B, class R = promoted_scalar<A, B>>
TG_NODISCARD constexpr angle<R> operator/(angle<A> a, B b)
{
    return radians(a.radians() / b);
}

template <class A, class B, class R = promoted_scalar<A, B>>
TG_NODISCARD constexpr angle<R> operator*(A a, angle<B> b)
{
    return radians(a * b.radians());
}

template <class A, class B, class R = promoted_scalar<A, B>>
TG_NODISCARD constexpr R operator/(angle<A> a, angle<B> b)
{
    return a.radians() / b.radians();
}
} // namespace tg
