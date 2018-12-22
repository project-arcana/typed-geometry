#pragma once

namespace tg
{
namespace detail
{
template <class ScalarT>
constexpr ScalarT add(ScalarT a, ScalarT b)
{
    return a + b;
}
template <class ScalarT>
constexpr ScalarT sub(ScalarT a, ScalarT b)
{
    return a - b;
}
template <class ScalarT>
constexpr ScalarT neg(ScalarT a)
{
    return -a;
}
template <class ScalarT>
constexpr ScalarT mul(ScalarT a, ScalarT b)
{
    return a * b;
}
template <class ScalarT>
constexpr bool eq(ScalarT a, ScalarT b)
{
    return a == b;
}
constexpr bool logic_and(bool a, bool b) { return a && b; }
constexpr bool logic_or(bool a, bool b) { return a || b; }
constexpr bool logic_not(bool a) { return !a; }

template <class A, class B, class C, class F>
constexpr void apply(vec<1, A>& r, vec<1, B> const& a, vec<1, C> const& b, F&& f)
{
    r.x = f(a.x, b.x);
}
template <class A, class B, class C, class F>
constexpr void apply(vec<2, A>& r, vec<2, B> const& a, vec<2, C> const& b, F&& f)
{
    r.x = f(a.x, b.x);
    r.y = f(a.y, b.y);
}
template <class A, class B, class C, class F>
constexpr void apply(vec<3, A>& r, vec<3, B> const& a, vec<3, C> const& b, F&& f)
{
    r.x = f(a.x, b.x);
    r.y = f(a.y, b.y);
    r.z = f(a.z, b.z);
}
template <class A, class B, class C, class F>
constexpr void apply(vec<4, A>& r, vec<4, B> const& a, vec<4, C> const& b, F&& f)
{
    r.x = f(a.x, b.x);
    r.y = f(a.y, b.y);
    r.z = f(a.z, b.z);
    r.w = f(a.w, b.w);
}

template <class A, class B, class C, class F>
constexpr void apply(vec<1, A>& r, vec<1, B> const& a, F&& f)
{
    r.x = f(a.x);
}
template <class A, class B, class C, class F>
constexpr void apply(vec<2, A>& r, vec<2, B> const& a, F&& f)
{
    r.x = f(a.x);
    r.y = f(a.y);
}
template <class A, class B, class C, class F>
constexpr void apply(vec<3, A>& r, vec<3, B> const& a, F&& f)
{
    r.x = f(a.x);
    r.y = f(a.y);
    r.z = f(a.z);
}
template <class A, class B, class C, class F>
constexpr void apply(vec<4, A>& r, vec<4, B> const& a, F&& f)
{
    r.x = f(a.x);
    r.y = f(a.y);
    r.z = f(a.z);
    r.w = f(a.w);
}

template <class R, int D, class A, class B, class F>
constexpr vec<D, R> compwise(vec<D, A> const& a, vec<D, B> const& b, F&& f)
{
    vec<D, R> r;
    apply(r, a, b, f);
    return r;
}
template <class R, int D, class A, class B, class F>
constexpr vec<D, R> compwise(vec<D, A> const& a, B b, F&& f)
{
    vec<D, R> r;
    apply(r, a, vec<D, B>(b), f);
    return r;
}
template <class R, int D, class A, class F>
constexpr vec<D, R> compwise(vec<D, A> const& a, F&& f)
{
    vec<D, R> r;
    apply(r, a, f);
    return r;
}

template <class ScalarT, class F>
constexpr ScalarT reduce(vec<1, ScalarT> const& v, F&& f)
{
    return v.x;
}
template <class ScalarT, class F>
constexpr ScalarT reduce(vec<2, ScalarT> const& v, F&& f)
{
    return f(v.x, v.y);
}
template <class ScalarT, class F>
constexpr ScalarT reduce(vec<3, ScalarT> const& v, F&& f)
{
    return f(f(v.x, v.y), v.z);
}
template <class ScalarT, class F>
constexpr ScalarT reduce(vec<4, ScalarT> const& v, F&& f)
{
    return f(f(v.x, v.y), f(v.z, v.w));
}
} // namespace detail
} // namespace tg
