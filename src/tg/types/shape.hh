#pragma once

#include <array>
#include <cassert>

namespace tg
{
template <int Order>
struct shape
{
    static constexpr int order = Order;

    std::array<int, order> dims;

public:
    shape() = default;
    constexpr shape(std::array<int, order> const& dims) : dims(dims) {}

public:
    constexpr int operator[](int o) const
    {
        assert(0 <= o && o < order);
        return dims[o];
    }

    template <int Order2>
    constexpr bool operator==(shape<Order2> const& rhs) const
    {
        if (Order == Order2)
        {
            for (auto i = 0; i < Order; ++i)
                if (dims[i] != rhs[i])
                    return false;
            return true;
        }
        else
            return false;
    }
    template <int Order2>
    constexpr bool operator!=(shape<Order2> const& rhs) const
    {
        return !operator==(rhs);
    }
};

constexpr shape<1> make_shape(int w) { return {{{w}}}; }
constexpr shape<2> make_shape(int w, int h) { return {{{w, h}}}; }
constexpr shape<3> make_shape(int w, int h, int d) { return {{{w, h, d}}}; }
template <int Order>
constexpr shape<Order> make_shape(std::array<int, Order> const& dims)
{
    return {dims};
}

} // namespace tg
