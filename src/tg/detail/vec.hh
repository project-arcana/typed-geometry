#pragma once

#include "shape.hh"

namespace tg
{
template <int D, class ScalarT>
struct vec
{
};

template <class ScalarT>
struct vec<1, ScalarT>
{
    using scalar_t = ScalarT;
    static inline constexpr shape shape = make_shape(1);

    scalar_t x = static_cast<scalar_t>(0);

    vec() = default;
    explicit vec(scalar_t v) : x(v) {}
};
template <class ScalarT>
struct vec<2, ScalarT>
{
    using scalar_t = ScalarT;
    static inline constexpr shape shape = make_shape(2);

    scalar_t x = static_cast<scalar_t>(0);
    scalar_t y = static_cast<scalar_t>(0);

    vec() = default;
    explicit vec(scalar_t v) : x(v), y(v) {}
    vec(scalar_t x, scalar_t y) : x(x), y(y) {}
};
template <class ScalarT>
struct vec<3, ScalarT>
{
    using scalar_t = ScalarT;
    static inline constexpr shape shape = make_shape(3);

    scalar_t x = static_cast<scalar_t>(0);
    scalar_t y = static_cast<scalar_t>(0);
    scalar_t z = static_cast<scalar_t>(0);

    vec() = default;
    explicit vec(scalar_t v) : x(v), y(v), z(v) {}
    vec(scalar_t x, scalar_t y, scalar_t z) : x(x), y(y), z(z) {}
};
template <class ScalarT>
struct vec<4, ScalarT>
{
    using scalar_t = ScalarT;
    static inline constexpr shape shape = make_shape(4);

    scalar_t x = static_cast<scalar_t>(0);
    scalar_t y = static_cast<scalar_t>(0);
    scalar_t z = static_cast<scalar_t>(0);
    scalar_t w = static_cast<scalar_t>(0);

    vec() = default;
    explicit vec(scalar_t v) : x(v), y(v), z(v), w(v) {}
    vec(scalar_t x, scalar_t y, scalar_t z, scalar_t w) : x(x), y(y), z(z), w(w) {}
};

} // namespace tg
