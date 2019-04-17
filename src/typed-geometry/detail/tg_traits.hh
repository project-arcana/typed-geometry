#pragma once

#include "../types/types.hh"
#include "scalar_traits.hh"

namespace tg
{
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
TG_IMPL_ADD_OBJECT_TYPE(ball);
// TODO: more

} // namespace tg
