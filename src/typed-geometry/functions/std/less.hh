#pragma once

#include <functional>
#include <tuple>

#include <typed-geometry/tg-lean.hh>


namespace std
{
// -- comp
template <class ScalarT>
struct less<tg::comp<1, ScalarT>>
{
    typedef tg::comp<1, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return a.x < b.x; }
};
template <class ScalarT>
struct less<tg::comp<2, ScalarT>>
{
    typedef tg::comp<2, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y) < std::tie(b.x, b.y); }
};
template <class ScalarT>
struct less<tg::comp<3, ScalarT>>
{
    typedef tg::comp<3, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y, a.z) < std::tie(b.x, b.y, b.z); }
};
template <class ScalarT>
struct less<tg::comp<4, ScalarT>>
{
    typedef tg::comp<4, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y, a.z, a.w) < std::tie(b.x, b.y, b.z, b.w); }
};

// -- vec
template <class ScalarT>
struct less<tg::vec<1, ScalarT>>
{
    typedef tg::vec<1, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return a.x < b.x; }
};
template <class ScalarT>
struct less<tg::vec<2, ScalarT>>
{
    typedef tg::vec<2, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y) < std::tie(b.x, b.y); }
};
template <class ScalarT>
struct less<tg::vec<3, ScalarT>>
{
    typedef tg::vec<3, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y, a.z) < std::tie(b.x, b.y, b.z); }
};
template <class ScalarT>
struct less<tg::vec<4, ScalarT>>
{
    typedef tg::vec<4, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y, a.z, a.w) < std::tie(b.x, b.y, b.z, b.w); }
};

// -- dir
template <class ScalarT>
struct less<tg::dir<1, ScalarT>>
{
    typedef tg::dir<1, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return a.x < b.x; }
};
template <class ScalarT>
struct less<tg::dir<2, ScalarT>>
{
    typedef tg::dir<2, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y) < std::tie(b.x, b.y); }
};
template <class ScalarT>
struct less<tg::dir<3, ScalarT>>
{
    typedef tg::dir<3, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y, a.z) < std::tie(b.x, b.y, b.z); }
};
template <class ScalarT>
struct less<tg::dir<4, ScalarT>>
{
    typedef tg::dir<4, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y, a.z, a.w) < std::tie(b.x, b.y, b.z, b.w); }
};

// -- pos
template <class ScalarT>
struct less<tg::pos<1, ScalarT>>
{
    typedef tg::pos<1, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return a.x < b.x; }
};
template <class ScalarT>
struct less<tg::pos<2, ScalarT>>
{
    typedef tg::pos<2, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y) < std::tie(b.x, b.y); }
};
template <class ScalarT>
struct less<tg::pos<3, ScalarT>>
{
    typedef tg::pos<3, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y, a.z) < std::tie(b.x, b.y, b.z); }
};
template <class ScalarT>
struct less<tg::pos<4, ScalarT>>
{
    typedef tg::pos<4, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y, a.z, a.w) < std::tie(b.x, b.y, b.z, b.w); }
};

// -- size
template <class ScalarT>
struct less<tg::size<1, ScalarT>>
{
    typedef tg::size<1, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return a.width < b.width; }
};
template <class ScalarT>
struct less<tg::size<2, ScalarT>>
{
    typedef tg::size<2, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.width, a.height) < std::tie(b.width, b.height); }
};
template <class ScalarT>
struct less<tg::size<3, ScalarT>>
{
    typedef tg::size<3, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept
    {
        return std::tie(a.width, a.height, a.depth) < std::tie(b.width, b.height, b.depth);
    }
};
template <class ScalarT>
struct less<tg::size<4, ScalarT>>
{
    typedef tg::size<4, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept
    {
        return std::tie(a.width, a.height, a.depth, a.w) < std::tie(b.width, b.height, b.depth, b.w);
    }
};

// -- color
template <class ScalarT>
struct less<tg::color<3, ScalarT>>
{
    typedef tg::color<3, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.r, a.g, a.b) < std::tie(b.r, b.g, b.b); }
};
template <class ScalarT>
struct less<tg::color<4, ScalarT>>
{
    typedef tg::color<4, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.r, a.g, a.b, a.a) < std::tie(b.r, b.g, b.b, b.a); }
};

// -- quat
template <class ScalarT>
struct less<tg::quaternion<ScalarT>>
{
    typedef tg::quaternion<ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.x, a.y, a.z, a.w) < std::tie(b.x, b.y, b.z, b.w); }
};

// -- mat
template <int R, class ScalarT>
struct less<tg::mat<1, R, ScalarT>>
{
    typedef tg::mat<1, R, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return a[0] < b[0]; }
};
template <int R, class ScalarT>
struct less<tg::mat<2, R, ScalarT>>
{
    typedef tg::mat<2, R, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a[0], a[1]) < std::tie(b[0], b[1]); }
};
template <int R, class ScalarT>
struct less<tg::mat<3, R, ScalarT>>
{
    typedef tg::mat<3, R, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a[0], a[1], a[2]) < std::tie(b[0], b[1], b[2]); }
};
template <int R, class ScalarT>
struct less<tg::mat<4, R, ScalarT>>
{
    typedef tg::mat<4, R, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept
    {
        return std::tie(a[0], a[1], a[2], a[3]) < std::tie(b[0], b[1], b[2], b[3]);
    }
};

// TODO: f8 and f16

// -- objects
template <int D, class ScalarT>
struct less<tg::triangle<D, ScalarT>>
{
    typedef tg::triangle<D, ScalarT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept
    {
        return std::tie(a.pos0, a.pos1, a.pos2) < std::tie(b.pos0, b.pos1, b.pos2);
    }
};
template <int D, class ScalarT, class TraitsT>
struct less<tg::aabb<D, ScalarT, TraitsT>>
{
    typedef tg::aabb<D, ScalarT, TraitsT> arg_t;
    constexpr bool operator()(arg_t const& a, arg_t const& b) const noexcept { return std::tie(a.min, a.max) < std::tie(b.min, b.max); }
};
} // namespace std
