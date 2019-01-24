#pragma once

#include "../detail/utility.hh"

namespace tg
{
template <int Order>
struct shape
{
    static constexpr int order = Order;

    array<int, order> dims;

public:
    shape() = default;
    constexpr shape(array<int, order> const& dims) : dims(dims) {}

public:
    constexpr int operator[](int o) const { return dims[o]; }

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
constexpr shape<Order> make_shape(array<int, Order> const& dims)
{
    return {dims};
}

} // namespace tg
