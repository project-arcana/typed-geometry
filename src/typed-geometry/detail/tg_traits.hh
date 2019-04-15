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

// pos/vec types
TG_IMPL_DEFINE_TYPE_TRAIT(pos_type_for, type_error::unknown_type<T>);
TG_IMPL_DEFINE_TYPE_TRAIT(vec_type_for, type_error::unknown_type<T>);
template <int D, class ScalarT, template <int, class> class Type>
struct pos_type_for_t<Type<D, ScalarT>>
{
    using type = pos<D, ScalarT>;
};
template <int D, class ScalarT, template <int, class> class Type>
struct vec_type_for_t<Type<D, ScalarT>>
{
    using type = vec<D, ScalarT>;
};

// vectors
template <int D, class ScalarT>
struct is_vector_t<vec<D, ScalarT>>
{
    static constexpr bool value = true;
};
TG_IMPL_INHERIT_TRAITS_D(vec);

// objects
TG_IMPL_ADD_OBJECT_TYPE(pos);
TG_IMPL_ADD_OBJECT_TYPE(box);
TG_IMPL_ADD_OBJECT_TYPE(line);
TG_IMPL_ADD_OBJECT_TYPE(segment);
TG_IMPL_ADD_OBJECT_TYPE(ray);
TG_IMPL_ADD_OBJECT_TYPE(sphere);
// TODO: more

} // namespace tg
