#pragma once

#include <typed-geometry/detail/macros.hh>

namespace tg
{
namespace detail
{
template <class StoreCompT, template <int, class> class CompTemplate, int D, class ScalarT, int... Indices>
struct base_swizzle_proxy
{
    static_assert((... && (0 <= Indices && Indices < D)), "swizzle index out of range");

    using comp_t = CompTemplate<D, ScalarT>;
    using other_comp_t = CompTemplate<sizeof...(Indices), ScalarT>;

    TG_NODISCARD constexpr operator other_comp_t() const { return other_comp_t(ref[Indices]...); }
    template <class OtherT>
    TG_NODISCARD explicit constexpr operator OtherT() const
    {
        return OtherT(ref[Indices]...);
    }

    explicit constexpr base_swizzle_proxy(StoreCompT ref) : ref(ref) {}

protected:
    StoreCompT ref;
};
template <template <int, class> class CompTemplate, int D, class ScalarT, int... Indices>
struct mutable_swizzle_proxy : base_swizzle_proxy<CompTemplate<D, ScalarT>&, CompTemplate, D, ScalarT, Indices...>
{
    using comp_t = CompTemplate<D, ScalarT>;
    using other_comp_t = CompTemplate<sizeof...(Indices), ScalarT>;

    constexpr void operator=(other_comp_t rhs) // per value for self-assign!
    {
        int i = 0;
        ((this->ref[Indices] = rhs[i++]), ...);
    }
    constexpr void operator=(ScalarT rhs) // per value for self-assign!
    {
        ((this->ref[Indices] = rhs), ...);
    }
    constexpr void operator=(mutable_swizzle_proxy const& rhs) // doesn't need per-value due to comp-cast
    {
        operator=((other_comp_t)rhs);
    }
    template <class OtherT>
    constexpr auto operator=(OtherT const& rhs) -> decltype((other_comp_t)rhs, void()) // only enable if implicit conversion available
    {
        operator=((other_comp_t)rhs);
    }

    using base_swizzle_proxy<CompTemplate<D, ScalarT>&, CompTemplate, D, ScalarT, Indices...>::base_swizzle_proxy;
};

template <template <int, class> class CompTemplate, int D, class ScalarT, int... Indices>
using const_swizzle_proxy = base_swizzle_proxy<CompTemplate<D, ScalarT> const&, CompTemplate, D, ScalarT, Indices...>;
template <template <int, class> class CompTemplate, int D, class ScalarT, int... Indices>
using owning_swizzle_proxy = base_swizzle_proxy<CompTemplate<D, ScalarT>, CompTemplate, D, ScalarT, Indices...>;
}

template <int... Indices>
struct swizzler
{
    // directly apply swizzling
    template <template <int, class> class CompT, int D, class ScalarT>
    TG_NODISCARD constexpr CompT<sizeof...(Indices), ScalarT> operator()(CompT<D, ScalarT> const& v) const
    {
        static_assert((... && (0 <= Indices && Indices < D)), "swizzle index out of range");
        return {v[Indices]...};
    }

    // return proxy object to allow assignment
    template <template <int, class> class CompT, int D, class ScalarT>
    TG_NODISCARD constexpr detail::mutable_swizzle_proxy<CompT, D, ScalarT, Indices...> operator[](CompT<D, ScalarT>& v) const
    {
        return detail::mutable_swizzle_proxy<CompT, D, ScalarT, Indices...>(v);
    }
    template <template <int, class> class CompT, int D, class ScalarT>
    TG_NODISCARD constexpr detail::const_swizzle_proxy<CompT, D, ScalarT, Indices...> operator[](CompT<D, ScalarT> const& v) const
    {
        return detail::const_swizzle_proxy<CompT, D, ScalarT, Indices...>(v);
    }
    template <template <int, class> class CompT, int D, class ScalarT>
    TG_NODISCARD constexpr detail::owning_swizzle_proxy<CompT, D, ScalarT, Indices...> operator[](CompT<D, ScalarT>&& v) const
    {
        return detail::owning_swizzle_proxy<CompT, D, ScalarT, Indices...>(static_cast<CompT<D, ScalarT>&&>(v));
    }
    template <template <int, class> class CompT, int D, class ScalarT>
    TG_NODISCARD constexpr detail::owning_swizzle_proxy<CompT, D, ScalarT, Indices...> operator[](CompT<D, ScalarT> const&& v) const
    {
        return detail::owning_swizzle_proxy<CompT, D, ScalarT, Indices...>(v);
    }
};

inline constexpr swizzler<0, 1> xy;
inline constexpr swizzler<0, 2> xz;
inline constexpr swizzler<0, 3> xw;
inline constexpr swizzler<1, 2> yz;
inline constexpr swizzler<1, 3> yw;
inline constexpr swizzler<2, 3> zw;

inline constexpr swizzler<0, 1, 2> xyz;
inline constexpr swizzler<0, 1, 3> xyw;
inline constexpr swizzler<0, 2, 3> xzw;
inline constexpr swizzler<1, 2, 3> yzw;

inline constexpr swizzler<0, 1, 2, 3> xyzw;
}
