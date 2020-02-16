#pragma once

#include <typed-geometry/types/fwd.hh>

namespace tg
{
/// the full object is meant (i.e. sphere with insides)
struct default_object_tag
{
    static constexpr char const* suffix = "";
};

/// only the object boundary is meant (i.e. only sphere surface)
struct boundary_tag
{
    static constexpr char const* suffix = "_boundary";
};

/// same as boundary_t but without caps (i.e. only cylinder mantle without caps)
struct boundary_no_caps_tag
{
    static constexpr char const* suffix = "_boundary_no_caps";
};

/// contains traits for a given object
template <class ObjT>
struct object_traits
{
    static constexpr bool is_object = false;
};

// implementation helper for objects
// objects traits should derive from finite_object_traits or infinite_object_traits
namespace detail
{
template <int ObjectD, class ScalarT, int DomainD, class TraitsTag>
struct object_base_traits
{
    using scalar_t = ScalarT;
    using pos_t = pos<DomainD, ScalarT>;
    using vec_t = vec<DomainD, ScalarT>;
    using dir_t = dir<DomainD, ScalarT>;
    using tag_t = TraitsTag;

    static constexpr bool is_object = true;

    static constexpr int domain_dimension = DomainD;
};

template <int ObjectD, class TraitsT>
struct object_tag_traits;
template <int ObjectD>
struct object_tag_traits<ObjectD, default_object_tag>
{
    static constexpr int object_dimension = ObjectD;
    static constexpr bool is_boundary = false;
    static constexpr bool no_caps = false;
};
template <int ObjectD>
struct object_tag_traits<ObjectD, boundary_tag>
{
    static constexpr int object_dimension = ObjectD - 1;
    static constexpr bool is_boundary = true;
    static constexpr bool no_caps = false;
};
template <int ObjectD>
struct object_tag_traits<ObjectD, boundary_no_caps_tag>
{
    static constexpr int object_dimension = ObjectD - 1;
    static constexpr bool is_boundary = true;
    static constexpr bool no_caps = true;
};

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct finite_object_traits : object_base_traits<ObjectD, ScalarT, DomainD, TraitsT>, object_tag_traits<ObjectD, TraitsT>
{
    static constexpr bool is_finite = true;
    static constexpr bool is_infinite = false;
};
template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct infinite_object_traits : object_base_traits<ObjectD, ScalarT, DomainD, TraitsT>, object_tag_traits<ObjectD, TraitsT>
{
    static constexpr bool is_finite = false;
    static constexpr bool is_infinite = true;
};
}
}
