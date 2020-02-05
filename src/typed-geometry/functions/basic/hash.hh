#pragma once

#include <typed-geometry/tg-lean.hh>

namespace tg
{
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
}

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

template <class T>
constexpr size_t make_hash(T const& v) noexcept
{
    return hash{}(v);
}
}
