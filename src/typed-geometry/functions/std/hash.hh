#pragma once

#include <functional>

#include <typed-geometry/functions/basic/hash.hh>

namespace std
{
// -- scalars
template <class T>
struct hash<tg::angle_t<T>> : tg::hash
{
};
template <class T>
struct hash<tg::interval<T>> : tg::hash
{
};
template <class T>
struct hash<tg::fwd_diff<T>> : tg::hash
{
};
template <int w>
struct hash<tg::fixed_int<w>> : tg::hash
{
};
template <int w>
struct hash<tg::fixed_uint<w>> : tg::hash
{
};

// -- comp-likes
template <int D, class ScalarT>
struct hash<tg::comp<D, ScalarT>> : tg::hash
{
};
template <int D, class ScalarT>
struct hash<tg::vec<D, ScalarT>> : tg::hash
{
};
template <int D, class ScalarT>
struct hash<tg::dir<D, ScalarT>> : tg::hash
{
};
template <int D, class ScalarT>
struct hash<tg::pos<D, ScalarT>> : tg::hash
{
};
template <int D, class ScalarT>
struct hash<tg::size<D, ScalarT>> : tg::hash
{
};
template <int D, class ScalarT>
struct hash<tg::color<D, ScalarT>> : tg::hash
{
};
template <class ScalarT>
struct hash<tg::quaternion<ScalarT>> : tg::hash
{
};

template <int D, class ScalarT>
struct hash<tg::quadric<D, ScalarT>> : tg::hash
{
};
template <int C, int R, class ScalarT>
struct hash<tg::mat<C, R, ScalarT>> : tg::hash
{
};

// TODO: f8 and f16

// -- objects
template <int D, class ScalarT>
struct hash<tg::line<D, ScalarT>> : tg::hash
{
};
template <int D, class ScalarT>
struct hash<tg::ray<D, ScalarT>> : tg::hash
{
};
template <int D, class ScalarT>
struct hash<tg::segment<D, ScalarT>> : tg::hash
{
};

template <int D, class ScalarT>
struct hash<tg::triangle<D, ScalarT>> : tg::hash
{
};
template <int D, class ScalarT>
struct hash<tg::quad<D, ScalarT>> : tg::hash
{
};
template <int D, class ScalarT>
struct hash<tg::plane<D, ScalarT>> : tg::hash
{
};
template <int D, class ScalarT>
struct hash<tg::halfspace<D, ScalarT>> : tg::hash
{
};

template <int D, class ScalarT, class TraitsT>
struct hash<tg::aabb<D, ScalarT, TraitsT>> : tg::hash
{
};
template <int D, class ScalarT, class TraitsT>
struct hash<tg::capsule<D, ScalarT, TraitsT>> : tg::hash
{
};
template <int D, class ScalarT, class TraitsT>
struct hash<tg::cylinder<D, ScalarT, TraitsT>> : tg::hash
{
};
template <int D, class ScalarT, class TraitsT>
struct hash<tg::hemisphere<D, ScalarT, TraitsT>> : tg::hash
{
};
template <int D, class ScalarT, class TraitsT>
struct hash<tg::inf_cone<D, ScalarT, TraitsT>> : tg::hash
{
};
template <int D, class ScalarT, class TraitsT>
struct hash<tg::inf_cylinder<D, ScalarT, TraitsT>> : tg::hash
{
};

template <class BaseT, class TraitsT>
struct hash<tg::pyramid<BaseT, TraitsT>> : tg::hash
{
};

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct hash<tg::box<ObjectD, ScalarT, DomainD, TraitsT>> : tg::hash
{
};
template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct hash<tg::ellipse<ObjectD, ScalarT, DomainD, TraitsT>> : tg::hash
{
};
template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct hash<tg::sphere<ObjectD, ScalarT, DomainD, TraitsT>> : tg::hash
{
};
} // namespace std
