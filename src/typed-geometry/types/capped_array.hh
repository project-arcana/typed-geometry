#pragma once

#include <type_traits>
#include <typed-geometry/common/assert.hh>

namespace tg
{
/// Array type with compile time memory footprint but runtime size
template <class T, std::size_t N>
struct capped_array
{
    constexpr capped_array(std::size_t size) : _size(size)
    {
        TG_CONTRACT(size <= N);
        new (&_data[0]) T[size]();
    }

    ~capped_array()
    {
        for (std::size_t i = 0; i < _size; ++i)
            reinterpret_cast<T*>(&_data[i])->~T();
    }

    const T& operator[](std::size_t pos) const
    {
        TG_CONTRACT(pos < _size);
        return reinterpret_cast<T const&>(_data[pos]);
    }

    T& operator[](std::size_t pos)
    {
        TG_CONTRACT(pos < _size);
        return reinterpret_cast<T&>(_data[pos]);
    }

    constexpr T* begin() { return reinterpret_cast<T*>(&_data); }
    constexpr T* end() { return reinterpret_cast<T*>(&_data + _size); }
    constexpr T const* begin() const { return reinterpret_cast<T const*>(&_data); }
    constexpr T const* end() const { return reinterpret_cast<T const*>(&_data + _size); }

    constexpr std::size_t size() const { return _size; }

    constexpr T* data() { return *reinterpret_cast<T*>(&_data); }
    constexpr T const* data() const { return reinterpret_cast<T const*>(&_data); }

private:
    std::size_t _size;
    std::aligned_storage_t<sizeof(T), alignof(T)> _data[N];
};
}
