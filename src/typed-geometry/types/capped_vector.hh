#pragma once

#include <typed-geometry/common/assert.hh>
#include <typed-geometry/detail/utility.hh>
#include <utility> // std::move

namespace tg
{
/// stack allocated vector type with compile time memory footprint but runtime size
template <class T, size_t N>
struct capped_vector
{
    using index_t = detail::size_t_for<N, alignof(T)>;

    constexpr capped_vector() = default;

    constexpr capped_vector(size_t size) : _size(index_t(size))
    {
        TG_CONTRACT(size <= N);
        new (&_u._data[0]) T[size]();
    }

    ~capped_vector()
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

    void push_back(T const& t)
    {
        TG_CONTRACT(_size < N);
        new (&_u._data[_size]) T(t);
        ++_size;
    }

    void push_back(T&& t)
    {
        TG_CONTRACT(_size < N);
        new (&_u._data[_size]) T(std::move(t));
        ++_size;
    }

    void pop_back()
    {
        TG_CONTRACT(_size > 0);
        --_size;
        _u._data[_size].~T();
    }

    template <typename... Args>
    T& emplace_back(Args&&... args)
    {
        TG_CONTRACT(_size < N);
        new (&_u._data[_size]) T(std::forward<Args>(args)...);
        ++_size;
        return _u._data[_size - 1];
    }

    void clear()
    {
        for (size_t i = 0; i < _size; ++i)
            _u._data[i].~T();
        _size = 0;
    }

    void resize(size_t new_size)
    {
        TG_CONTRACT(new_size <= N);
        for (size_t i = _size; i < new_size; ++i)
            new (&_u._data[i]) T();
        _size = index_t(new_size);
    }

    constexpr T& front()
    {
        TG_CONTRACT(_size > 0);
        return _u._data[0];
    }

    constexpr T const& front() const
    {
        TG_CONTRACT(_size > 0);
        return _u._data[0];
    }

    constexpr T& back()
    {
        TG_CONTRACT(_size > 0);
        return _u._data[_size - 1];
    }

    constexpr T const& back() const
    {
        TG_CONTRACT(_size > 0);
        return _u._data[_size - 1];
    }

    constexpr T* begin() { return &_u._data[0]; }
    constexpr T* end() { return &_u._data[0] + _size; }
    constexpr T const* begin() const { return &_u._data[0]; }
    constexpr T const* end() const { return &_u._data[0] + _size; }

    constexpr size_t size() const { return _size; }
    constexpr size_t capacity() const { return N; }
    constexpr bool empty() const { return _size == 0; }

    constexpr T* data() { return &_u._data[0]; }
    constexpr T const* data() const { return &_u._data[0]; }

    template <int M>
    constexpr bool operator==(capped_vector<T, M> const& rhs) const
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
    constexpr bool operator!=(capped_vector<T, M> const& rhs) const
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
    union DataHolder {
        DataHolder() {}
        ~DataHolder() {}
        T _data[N];
    };
    index_t _size = 0;
    DataHolder _u;
};
}
