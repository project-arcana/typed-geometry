#pragma once

#include "../detail/macros.hh"
#include "scalar.hh"
#include "shape.hh"

namespace tg
{
template <int D, class ScalarT>
struct size;

// Common size types

using size1 = size<1, f32>;
using size2 = size<2, f32>;
using size3 = size<3, f32>;
using size4 = size<4, f32>;

using fsize1 = size<1, f32>;
using fsize2 = size<2, f32>;
using fsize3 = size<3, f32>;
using fsize4 = size<4, f32>;

using dsize1 = size<1, f64>;
using dsize2 = size<2, f64>;
using dsize3 = size<3, f64>;
using dsize4 = size<4, f64>;

using isize1 = size<1, i32>;
using isize2 = size<2, i32>;
using isize3 = size<3, i32>;
using isize4 = size<4, i32>;

using usize1 = size<1, u32>;
using usize2 = size<2, u32>;
using usize3 = size<3, u32>;
using usize4 = size<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct size
{
};

template <class ScalarT>
struct size<1, ScalarT>
{
    using scalar_t = ScalarT;
    static constexpr shape<1> shape = make_shape(1);

    scalar_t width = static_cast<scalar_t>(0);

    constexpr scalar_t& operator[](int i) { return (&width)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&width)[i]; }

    constexpr size() = default;
    constexpr explicit size(scalar_t v) : width(v) {}
    template <int D, class T, class = std::enable_if_t<D >= 1>>
    constexpr explicit size(size<D, T> const& v) : width(ScalarT(v.width))
    {
    }
    template <int D, class T, class = std::enable_if_t<D <= 1>>
    constexpr explicit operator size<D, T>() const
    {
        return size<D, T>(*this);
    }
};

template <class ScalarT>
struct size<2, ScalarT>
{
    using scalar_t = ScalarT;
    static constexpr shape<1> shape = make_shape(2);

    scalar_t width = static_cast<scalar_t>(0);
    scalar_t height = static_cast<scalar_t>(0);

    constexpr scalar_t& operator[](int i) { return (&width)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&width)[i]; }

    constexpr size() = default;
    constexpr explicit size(scalar_t v) : width(v), height(v) {}
    constexpr size(scalar_t width, scalar_t height) : width(width), height(height) {}
    template <int D, class T, class = std::enable_if_t<D >= 2>>
    constexpr explicit size(size<D, T> const& v) : width(ScalarT(v.width)), height(ScalarT(v.height))
    {
    }
    template <int D, class T, class = std::enable_if_t<D <= 2>>
    constexpr explicit operator size<D, T>() const
    {
        return size<D, T>(*this);
    }
};

template <class ScalarT>
struct size<3, ScalarT>
{
    using scalar_t = ScalarT;
    static constexpr shape<1> shape = make_shape(3);

    scalar_t width = static_cast<scalar_t>(0);
    scalar_t height = static_cast<scalar_t>(0);
    scalar_t depth = static_cast<scalar_t>(0);

    constexpr scalar_t& operator[](int i) { return (&width)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&width)[i]; }

    constexpr size() = default;
    constexpr explicit size(scalar_t v) : width(v), height(v), depth(v) {}
    constexpr size(scalar_t width, scalar_t height, scalar_t depth) : width(width), height(height), depth(depth) {}
    template <int D, class T, class = std::enable_if_t<D >= 3>>
    constexpr explicit size(size<D, T> const& v) : width(ScalarT(v.width)), height(ScalarT(v.height)), depth(ScalarT(v.depth))
    {
    }
    template <int D, class T, class = std::enable_if_t<D <= 3>>
    constexpr explicit operator size<D, T>() const
    {
        return size<D, T>(*this);
    }
};

template <class ScalarT>
struct size<4, ScalarT>
{
    using scalar_t = ScalarT;
    static constexpr shape<1> shape = make_shape(4);

    scalar_t width = static_cast<scalar_t>(0);
    scalar_t height = static_cast<scalar_t>(0);
    scalar_t depth = static_cast<scalar_t>(0);
    scalar_t w = static_cast<scalar_t>(0);

    constexpr scalar_t& operator[](int i) { return (&width)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&width)[i]; }

    constexpr size() = default;
    constexpr explicit size(scalar_t v) : width(v), height(v), depth(v), w(v) {}
    constexpr size(scalar_t width, scalar_t height, scalar_t depth, scalar_t w) : width(width), height(height), depth(depth), w(w) {}
    template <int D, class T, class = std::enable_if_t<D >= 4>>
    constexpr explicit size(size<D, T> const& v) : width(ScalarT(v.width)), height(ScalarT(v.height)), depth(ScalarT(v.depth)), w(ScalarT(v.w))
    {
    }
    template <int D, class T, class = std::enable_if_t<D <= 4>>
    constexpr explicit operator size<D, T>() const
    {
        return size<D, T>(*this);
    }
};

TG_IMPL_DEFINE_REDUCTION_OP_BINARY(size, size, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(size, size, bool, operator!=, ||, !=);
} // namespace tg
