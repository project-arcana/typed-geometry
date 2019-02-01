#pragma once

#include "../types/types.hh"
#include "scalar_traits.hh"

namespace tg
{
// type names
template <int D, class ScalarT>
struct type_name_t<vec<D, ScalarT>>
{
    static constexpr char const* value = "vec";
};
template <int D, class ScalarT>
struct type_name_t<pos<D, ScalarT>>
{
    static constexpr char const* value = "pos";
};
template <int D, class ScalarT>
struct type_name_t<size<D, ScalarT>>
{
    static constexpr char const* value = "size";
};
template <int D, class ScalarT>
struct type_name_t<box<D, ScalarT>>
{
    static constexpr char const* value = "box";
};
template <int D, class ScalarT>
struct type_name_t<triangle<D, ScalarT>>
{
    static constexpr char const* value = "triangle";
};
template <int C, int R, class ScalarT>
struct type_name_t<mat<C, R, ScalarT>>
{
    static constexpr char const* value = "mat";
};

// type suffix
template <int D, class ScalarT, template <int, class> class Type>
struct type_name_suffix_t<Type<D, ScalarT>>
{
    static constexpr char const* value = type_name_number<D>;
};

// vectors
template <int D, class ScalarT>
struct is_vector_t<vec<D, ScalarT>>
{
    static constexpr bool value = true;
};
TG_IMPL_INHERIT_TRAITS_D(vec);
} // namespace tg
