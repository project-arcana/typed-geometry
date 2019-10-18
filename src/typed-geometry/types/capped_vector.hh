#pragma once

#include <type_traits>
#include <typed-geometry/common/assert.hh>
#include <utility>

namespace tg
{
/// stack allocated vector type with compile time memory footprint but runtime size
template <class T, std::size_t N>
struct capped_vector
{
    constexpr capped_vector() = default;

    constexpr capped_vector(std::size_t size) : _size(size)
    {
        TG_CONTRACT(size <= N);
        new (&_data[0]) T[size]();
    }

    ~capped_vector()
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

    void push_back(T const& t)
    {
        TG_CONTRACT(_size < N);
        new (&_data[_size]) T(t);
        ++_size;
    }

    void push_back(T&& t)
    {
        TG_CONTRACT(_size < N);
        new (&_data[_size]) T(std::move(t));
        ++_size;
    }

    void pop_back()
    {
        TG_CONTRACT(_size > 0);
        --_size;
        reinterpret_cast<T*>(&_data[_size])->~T();
    }

    template <typename... Args>
    T& emplace_back(Args&&... args)
    {
        TG_CONTRACT(_size < N);
        new (&_data[_size]) T(std::forward<Args>(args)...);
        ++_size;
        return reinterpret_cast<T&>(_data[_size - 1]);
    }

    void clear()
    {
        for (std::size_t i = 0; i < _size; ++i)
            reinterpret_cast<T*>(&_data[i])->~T();
        _size = 0;
    }

    constexpr T& front()
    {
        TG_CONTRACT(_size > 0);
        return reinterpret_cast<T&>(_data[0]);
    }

    constexpr T const& front() const
    {
        TG_CONTRACT(_size > 0);
        return reinterpret_cast<T const&>(_data[0]);
    }

    constexpr T& back()
    {
        TG_CONTRACT(_size > 0);
        return reinterpret_cast<T&>(_data[_size - 1]);
    }

    constexpr T const& back() const
    {
        TG_CONTRACT(_size > 0);
        return reinterpret_cast<T const&>(_data[_size - 1]);
    }

    constexpr T* begin() { return reinterpret_cast<T*>(&_data); }
    constexpr T* end() { return reinterpret_cast<T*>(&_data + _size); }
    constexpr T const* begin() const { return reinterpret_cast<T const*>(&_data); }
    constexpr T const* end() const { return reinterpret_cast<T const*>(&_data + _size); }

    constexpr std::size_t size() const { return _size; }
    constexpr std::size_t capacity() const { return N; }
    constexpr bool empty() const { return _size == 0; }

    constexpr T* data() { return *reinterpret_cast<T*>(&_data); }
    constexpr T const* data() const { return reinterpret_cast<T const*>(&_data); }

    template <int M>
    constexpr bool operator==(capped_vector<T, M> const& rhs)
    {
        if (_size != rhs._size)
            return false;
        for (std::size_t i = 0; i < _size; ++i)
        {
            if ((*this)[i] != rhs[i])
                return false;
        }
        return true;
    }

    template <int M>
    constexpr bool operator!=(capped_vector<T, M> const& rhs)
    {
        if(_size != rhs._size)
            return true;
        for (std::size_t i = 0; i < _size; ++i)
        {
            if ((*this)[i] != rhs[i])
                return true;
        }
        return false;
    }

private:
    std::size_t _size = 0;
    std::aligned_storage_t<sizeof(T), alignof(T)> _data[N];
};
}
