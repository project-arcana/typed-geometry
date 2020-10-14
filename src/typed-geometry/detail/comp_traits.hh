#pragma once

#include "utility.hh"

#include <typed-geometry/types/fwd.hh>

#include <type_traits>

namespace tg
{
namespace detail
{
template <class Obj>
struct comp_size
{
    static constexpr int value = -1;
};
template <int N, class T>
struct comp_size<T[N]>
{
    static constexpr int value = N;
};
template <int D, class T>
struct comp_size<vec<D, T>>
{
    static constexpr int value = D;
};
template <int D, class T>
struct comp_size<dir<D, T>>
{
    static constexpr int value = D;
};
template <int D, class T>
struct comp_size<pos<D, T>>
{
    static constexpr int value = D;
};
template <int D, class T>
struct comp_size<size<D, T>>
{
    static constexpr int value = D;
};
template <int D, class T>
struct comp_size<color<D, T>>
{
    static constexpr int value = D;
};
template <int D, class T>
struct comp_size<comp<D, T>>
{
    static constexpr int value = D;
};
template <class T>
struct comp_size<quaternion<T>>
{
    static constexpr int value = 4;
};
template <int C, int R, class T>
struct comp_size<mat<C, R, T>>
{
    static constexpr int value = C;
};

template <class Obj, class ScalarT>
auto test_comp_convertible(Obj* obj) -> decltype(ScalarT((*obj)[0]), true_type{});
template <class Obj, class ScalarT>
auto test_comp_convertible(...) -> false_type;

template <class Obj, class ScalarT>
auto test_mat_convertible(Obj* obj) -> decltype(ScalarT((*obj)[0][0]), true_type{});
template <class Obj, class ScalarT>
auto test_mat_convertible(...) -> false_type;

template <class Obj, class = enable_if<comp_size<Obj>::value != -1>>
constexpr int impl_get_dynamic_comp_size(Obj const&, priority_tag<2>)
{
    return comp_size<Obj>::value;
}
template <class Obj>
constexpr auto impl_get_dynamic_comp_size(Obj const& v, priority_tag<1>) -> decltype(int(v.size()))
{
    return int(v.size());
}
template <class Obj>
constexpr auto impl_get_dynamic_comp_size(Obj const& v, priority_tag<0>) -> decltype(int(v.length()))
{
    return int(v.length());
}

template <class Obj>
constexpr int get_dynamic_comp_size(Obj const& v)
{
    return impl_get_dynamic_comp_size(v, priority_tag<2>{});
}

template <class Obj, class ScalarT>
constexpr ScalarT comp_get(Obj const& v, unsigned char idx, int size, ScalarT fill)
{
    return idx < size ? ScalarT(v[idx]) : fill;
}
}

template <class Obj, class ScalarT>
constexpr bool is_comp_convertible = !std::is_pointer_v<std::remove_reference_t<Obj>> && decltype(detail::test_comp_convertible<Obj, ScalarT>(nullptr))::value;

template <class Obj, class ScalarT>
constexpr bool is_mat_convertible = !std::is_pointer_v<std::remove_reference_t<Obj>> && decltype(detail::test_mat_convertible<Obj, ScalarT>(nullptr))::value;

template <class Obj>
constexpr bool is_comp_dynamic_size = detail::comp_size<Obj>::value < 0;

template <class Obj, int D, class ScalarT>
constexpr bool is_comp_like
    = !std::is_pointer_v<std::remove_reference_t<Obj>> && is_comp_convertible<Obj, ScalarT> && (is_comp_dynamic_size<Obj> || detail::comp_size<Obj>::value >= D);
}
