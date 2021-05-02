#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/types/scalars/default.hh>
#include <typed-geometry/types/span.hh>

namespace tg
{
// TODO: variable sized array
template <class T, u64 N>
struct array
{
    // must be public for ctor
    T _values[N];

    constexpr T* begin() { return _values; }
    constexpr T* end() { return _values + N; }
    constexpr T const* begin() const { return _values; }
    constexpr T const* end() const { return _values + N; }

    constexpr u64 size() const { return N; }

    constexpr T* data() { return _values; }
    constexpr T const* data() const { return _values; }

    constexpr T& operator[](u64 i)
    {
        TG_CONTRACT(i < N);
        return _values[i];
    }
    constexpr T const& operator[](u64 i) const
    {
        TG_CONTRACT(i < N);
        return _values[i];
    }

    constexpr bool operator==(span<T const> rhs) const noexcept
    {
        if (N != rhs.size())
            return false;
        for (size_t i = 0; i < N; ++i)
            if (!(_values[i] == rhs[i]))
                return false;
        return true;
    }

    constexpr bool operator!=(span<T const> rhs) const noexcept
    {
        if (N != rhs.size())
            return true;
        for (size_t i = 0; i < N; ++i)
            if (_values[i] != rhs[i])
                return true;
        return false;
    }
};
}
