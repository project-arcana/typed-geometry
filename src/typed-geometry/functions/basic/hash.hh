#pragma once

#include <typed-geometry/tg-lean.hh>

#include <clean-core/hash.hh>

namespace tg
{
template <class T>
using hash = cc::hash<T>;

template <class T>
constexpr size_t make_hash(T const& v) noexcept
{
    return hash<T>{}(v);
}

namespace detail
{
struct hash_inspector
{
    size_t hash = 0x987654321;

    template <class T, class... Args>
    constexpr void operator()(T const& v, Args&&...)
    {
        add(v);
    }

private:
    constexpr void hash_combine(size_t v) { hash = hash * 6364136223846793005ULL + v + 0xda3e39cb94b95bdbULL; }

    constexpr void add(bool v) { hash_combine(size_t(v)); }

    constexpr void add(i8 v) { hash_combine(size_t(v)); }
    constexpr void add(i16 v) { hash_combine(size_t(v)); }
    constexpr void add(i32 v) { hash_combine(size_t(v)); }
    constexpr void add(i64 v) { hash_combine(size_t(v)); }

    constexpr void add(u8 v) { hash_combine(size_t(v)); }
    constexpr void add(u16 v) { hash_combine(size_t(v)); }
    constexpr void add(u32 v) { hash_combine(size_t(v)); }
    constexpr void add(u64 v) { hash_combine(size_t(v)); }

    constexpr void add(f32 v) { hash_combine(v == 0 ? 0 : bit_cast<u32>(v)); }
    constexpr void add(f64 v) { hash_combine(v == 0 ? 0 : bit_cast<u64>(v)); }

    template <class T>
    constexpr void add(T const& v)
    {
        introspect(*this, const_cast<T&>(v)); // v will not be changed
    }
};

// universal hasher for tg types
struct hash
{
    template <class T>
    constexpr size_t operator()(T const& v) const noexcept
    {
        detail::hash_inspector hi;
        hi(v);
        return hi.hash;
    }
};
}
}

// extend cc::hash for tg types
namespace cc
{

// ------------------------------------ objects ------------------------------------ //

template <int D, class ScalarT, class TraitsT>
struct hash<tg::aabb<D, ScalarT, TraitsT>> : tg::detail::hash
{
};

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct hash<tg::box<ObjectD, ScalarT, DomainD, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT, class TraitsT>
struct hash<tg::capsule<D, ScalarT, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT, class TraitsT>
struct hash<tg::cylinder<D, ScalarT, TraitsT>> : tg::detail::hash
{
};

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct hash<tg::ellipse<ObjectD, ScalarT, DomainD, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT, class TraitsT>
struct hash<tg::frustum<D, ScalarT, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::halfspace<D, ScalarT>> : tg::detail::hash
{
};

template <int D, class ScalarT, class TraitsT>
struct hash<tg::hemisphere<D, ScalarT, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT, class TraitsT>
struct hash<tg::inf_cone<D, ScalarT, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT, class TraitsT>
struct hash<tg::inf_cylinder<D, ScalarT, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT, class TraitsT>
struct hash<tg::inf_frustum<D, ScalarT, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::line<D, ScalarT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::plane<D, ScalarT>> : tg::detail::hash
{
};

template <class BaseT, class TraitsT>
struct hash<tg::pyramid<BaseT, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::quad<D, ScalarT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::ray<D, ScalarT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::segment<D, ScalarT>> : tg::detail::hash
{
};

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct hash<tg::sphere<ObjectD, ScalarT, DomainD, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT, class TraitsT>
struct hash<tg::tetrahedron<D, ScalarT, TraitsT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::triangle<D, ScalarT>> : tg::detail::hash
{
};

// ------------------------------------ scalars ------------------------------------ //

template <int words>
struct hash<tg::fixed_int<words>> : tg::detail::hash
{
};

template <int words>
struct hash<tg::fixed_uint<words>> : tg::detail::hash
{
};

template <class T>
struct hash<tg::fwd_diff<T>> : tg::detail::hash
{
};

// todo: half

template <class T>
struct hash<tg::interval<T>> : tg::detail::hash
{
};

// todo: quater

// ------------------------------------ comp-likes ------------------------------------ //

template <class T>
struct hash<tg::angle_t<T>> : tg::detail::hash
{
};

template <class T, uint64_t N>
struct hash<tg::array<T, N>> : tg::detail::hash
{
};

template <int Degree, class ControlPointT>
struct hash<tg::bezier<Degree, ControlPointT>> : tg::detail::hash
{
};

template <class T, size_t N>
struct hash<tg::capped_array<T, N>> : tg::detail::hash
{
};

template <class T, size_t N>
struct hash<tg::capped_vector<T, N>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::color<D, ScalarT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::comp<D, ScalarT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::dir<D, ScalarT>> : tg::detail::hash
{
};

template <class ScalarT>
struct hash<tg::hsl_t<ScalarT>> : tg::detail::hash
{
};

template <class ScalarT>
struct hash<tg::hsla_t<ScalarT>> : tg::detail::hash
{
};

template <int C, int R, class ScalarT>
struct hash<tg::mat<C, R, ScalarT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::pos<D, ScalarT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::quadric<D, ScalarT>> : tg::detail::hash
{
};

template <class ScalarT>
struct hash<tg::quaternion<ScalarT>> : tg::detail::hash
{
};

template <int D>
struct hash<tg::range<D>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::size<D, ScalarT>> : tg::detail::hash
{
};

template <class ScalarT>
struct hash<tg::srgb<ScalarT>> : tg::detail::hash
{
};

template <class ScalarT>
struct hash<tg::srgba<ScalarT>> : tg::detail::hash
{
};

template <int D, class ScalarT>
struct hash<tg::vec<D, ScalarT>> : tg::detail::hash
{
};

}
