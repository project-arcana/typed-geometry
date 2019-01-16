#pragma once

#include "scalar.hh"
#include "shape.hh"

namespace tg
{
template <int D, class ScalarT>
struct pos;

// Common position types

using pos1 = pos<1, f32>;
using pos2 = pos<2, f32>;
using pos3 = pos<3, f32>;
using pos4 = pos<4, f32>;

using fpos1 = pos<1, f32>;
using fpos2 = pos<2, f32>;
using fpos3 = pos<3, f32>;
using fpos4 = pos<4, f32>;

using dpos1 = pos<1, f64>;
using dpos2 = pos<2, f64>;
using dpos3 = pos<3, f64>;
using dpos4 = pos<4, f64>;

using ipos1 = pos<1, i32>;
using ipos2 = pos<2, i32>;
using ipos3 = pos<3, i32>;
using ipos4 = pos<4, i32>;

using upos1 = pos<1, u32>;
using upos2 = pos<2, u32>;
using upos3 = pos<3, u32>;
using upos4 = pos<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct pos
{
};

template <class ScalarT>
struct pos<1, ScalarT>
{
    using scalar_t = ScalarT;
    static constexpr shape<1> shape = make_shape(1);

    static const pos zero;

    scalar_t x = static_cast<scalar_t>(0);

    constexpr scalar_t& operator[](int i) { return (&x)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&x)[i]; }

    constexpr pos() = default;
    constexpr explicit pos(scalar_t v) : x(v) {}
    template <int D, class T, class = std::enable_if_t<D >= 1>>
    constexpr explicit pos(pos<D, T> const& v, scalar_t = ScalarT(0)) : x(ScalarT(v.x))
    {
    }
    template <int D, class T>
    constexpr explicit operator pos<D, T>() const
    {
        return pos<D, T>(*this);
    }
};
template <class ScalarT>
constexpr pos<1, ScalarT> pos<1, ScalarT>::zero = {ScalarT(0)};

template <class ScalarT>
struct pos<2, ScalarT>
{
    using scalar_t = ScalarT;
    static constexpr shape<1> shape = make_shape(2);

    scalar_t x = static_cast<scalar_t>(0);
    scalar_t y = static_cast<scalar_t>(0);

    static const pos zero;

    constexpr scalar_t& operator[](int i) { return (&x)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&x)[i]; }

    constexpr pos() = default;
    constexpr explicit pos(scalar_t v) : x(v), y(v) {}
    constexpr pos(scalar_t x, scalar_t y) : x(x), y(y) {}
    template <int D, class T, class = std::enable_if_t<D >= 2>>
    constexpr explicit pos(pos<D, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y))
    {
    }
    template <class T>
    constexpr explicit pos(pos<1, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(fill)
    {
    }
    template <int D, class T>
    constexpr explicit operator pos<D, T>() const
    {
        return pos<D, T>(*this);
    }
};
template <class ScalarT>
constexpr pos<2, ScalarT> pos<2, ScalarT>::zero = {ScalarT(0), ScalarT(0)};

template <class ScalarT>
struct pos<3, ScalarT>
{
    using scalar_t = ScalarT;
    static constexpr shape<1> shape = make_shape(3);

    scalar_t x = static_cast<scalar_t>(0);
    scalar_t y = static_cast<scalar_t>(0);
    scalar_t z = static_cast<scalar_t>(0);

    static const pos zero;

    constexpr scalar_t& operator[](int i) { return (&x)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&x)[i]; }

    constexpr pos() = default;
    constexpr explicit pos(scalar_t v) : x(v), y(v), z(v) {}
    constexpr pos(scalar_t x, scalar_t y, scalar_t z) : x(x), y(y), z(z) {}
    template <int D, class T, class = std::enable_if_t<D >= 3>>
    constexpr explicit pos(pos<D, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z))
    {
    }
    template <class T>
    constexpr explicit pos(pos<2, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(fill)
    {
    }
    template <class T>
    constexpr explicit pos(pos<1, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(fill), z(fill)
    {
    }
    template <int D, class T, class = std::enable_if_t<D <= 3>>
    constexpr explicit operator pos<D, T>() const
    {
        return pos<D, T>(*this);
    }
};
template <class ScalarT>
constexpr pos<3, ScalarT> pos<3, ScalarT>::zero = {ScalarT(0), ScalarT(0), ScalarT(0)};

template <class ScalarT>
struct pos<4, ScalarT>
{
    using scalar_t = ScalarT;
    static constexpr shape<1> shape = make_shape(4);

    static const pos zero;

    scalar_t x = static_cast<scalar_t>(0);
    scalar_t y = static_cast<scalar_t>(0);
    scalar_t z = static_cast<scalar_t>(0);
    scalar_t w = static_cast<scalar_t>(0);

    constexpr scalar_t& operator[](int i) { return (&x)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&x)[i]; }

    constexpr pos() = default;
    constexpr explicit pos(scalar_t v) : x(v), y(v), z(v), w(v) {}
    constexpr pos(scalar_t x, scalar_t y, scalar_t z, scalar_t w) : x(x), y(y), z(z), w(w) {}
    template <int D, class T, class = std::enable_if_t<D >= 4>>
    constexpr explicit pos(pos<D, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z)), w(ScalarT(v.w))
    {
    }
    template <class T>
    constexpr explicit pos(pos<3, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z)), w(fill)
    {
    }
    template <class T>
    constexpr explicit pos(pos<2, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(fill), w(fill)
    {
    }
    template <class T>
    constexpr explicit pos(pos<1, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(fill), z(fill), w(fill)
    {
    }
    template <int D, class T, class = std::enable_if_t<D <= 4>>
    constexpr explicit operator pos<D, T>() const
    {
        return pos<D, T>(*this);
    }
};
template <class ScalarT>
constexpr pos<4, ScalarT> pos<4, ScalarT>::zero = {ScalarT(0), ScalarT(0), ScalarT(0), ScalarT(0)};

} // namespace tg
