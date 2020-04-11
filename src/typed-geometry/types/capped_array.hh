#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/feature/assert.hh>

namespace tg
{
/// Array type with compile time memory footprint but runtime size
template <class T, size_t N>
struct capped_array
{
    using index_t = detail::compact_size_t_typed<T, N>;

    constexpr capped_array(size_t size) : _size(index_t(size))
    {
        TG_CONTRACT(size <= N);
        new (&_u._data[0]) T[size]();
    }

    ~capped_array()
    {
        for (size_t i = 0; i < _size; ++i)
            _u._data[i].~T();
    }

    const T& operator[](size_t pos) const
    {
        TG_CONTRACT(pos < _size);
        return _u._data[pos];
    }

    T& operator[](size_t pos)
    {
        TG_CONTRACT(pos < _size);
        return _u._data[pos];
    }

    constexpr T* begin() { return &_u._data[0]; }
    constexpr T const* begin() const { return &_u._data[0]; }
    constexpr T* end() { return &_u._data[0] + _size; }
    constexpr T const* end() const { return &_u._data[0] + _size; }

    constexpr size_t size() const { return _size; }

    constexpr T* data() { return &_u._data[0]; }
    constexpr T const* data() const { return &_u._data[0]; }

    template <int M>
    constexpr bool operator==(capped_array<T, M> const& rhs) const
    {
        if (_size != rhs._size)
            return false;
        for (size_t i = 0; i < _size; ++i)
        {
            if ((*this)[i] != rhs[i])
                return false;
        }
        return true;
    }

    template <int M>
    constexpr bool operator!=(capped_array<T, M> const& rhs) const
    {
        if (_size != rhs._size)
            return true;
        for (size_t i = 0; i < _size; ++i)
        {
            if ((*this)[i] != rhs[i])
                return true;
        }
        return false;
    }

private:
    /// uninitialized data
    union DataHolder {
        DataHolder() {}
        ~DataHolder() {}
        T _data[N];
    };
    index_t _size = 0;
    DataHolder _u;
};
}
