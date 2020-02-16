#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/detail/utility.hh>

#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
// a non-owning view of a contiguous array of Ts
// can be read and write (span<const T> vs span<T>)
// is trivially copyable (and cheap)
// NOTE: is range-checked via TG_CONTRACT
template <class T>
struct span
{
    // ctors
public:
    constexpr span() = default;
    constexpr span(T* data, size_t size) : _data(data), _size(size) {}
    constexpr span(T* d_begin, T* d_end) : _data(d_begin), _size(d_end - d_begin) {}
    template <size_t N>
    constexpr span(T (&data)[N]) : _data(data), _size(N)
    {
    }
    template <class Container, class = enable_if<is_container<Container, T>>>
    constexpr span(Container& c) : _data(c.data()), _size(c.size())
    {
    }

    // container
public:
    constexpr T* begin() const { return _data; }
    constexpr T* end() const { return _data + _size; }

    constexpr T* data() const { return _data; }
    constexpr size_t size() const { return _size; }
    constexpr bool empty() const { return _size == 0; }

    constexpr T& operator[](size_t i) const
    {
        TG_CONTRACT(i < _size);
        return _data[i];
    }

    constexpr T& front() const
    {
        TG_CONTRACT(_size > 0);
        return _data[0];
    }
    constexpr T& back() const
    {
        TG_CONTRACT(_size > 0);
        return _data[_size - 1];
    }

    // subviews
public:
    constexpr span first(size_t n) const
    {
        TG_CONTRACT(n <= _size);
        return {_data, n};
    }
    constexpr span last(size_t n) const
    {
        TG_CONTRACT(n <= _size);
        return {_data + (_size - n), n};
    }
    constexpr span subspan(size_t offset, size_t count) const
    {
        TG_CONTRACT(offset + count <= _size);
        return {_data + offset, count};
    }

private:
    T* _data = nullptr;
    size_t _size = 0;
};

// deduction guide for containers
template <class Container, class = enable_if<is_container<Container, void>>>
span(Container& c)->span<remove_reference<decltype(*c.data())>>;
}
