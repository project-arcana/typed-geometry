#pragma once

#include <cstdint>

namespace tg
{
using u64 = std::uint64_t;

// general purpose types:
template <class T, u64 N>
struct array;
template <class T>
struct span;

// scalar types
template <class T>
struct angle_t;
template <class ScalarT>
struct horizontal_fov_t;

// component-like types:
template <int D, class ScalarT>
struct vec;
template <int D, class ScalarT>
struct pos;
template <int D, class ScalarT>
struct size;
template <int D, class ScalarT>
struct dir;
template <int D, class ScalarT>
struct comp;
template <int D, class ScalarT>
struct color;
template <class ScalarT>
struct quaternion;

// matrix types:
template <int C, int R, class ScalarT>
struct mat;

// object types:
template <int D, class ScalarT>
struct line;
template <int D, class ScalarT>
struct ray;
template <int D, class ScalarT>
struct segment;
template <int D, class ScalarT>
struct plane;
template <int D, class ScalarT>
struct halfspace;

// special types:
struct splitmix;
struct xorshift;
struct pcg;
template <int... Indices>
struct swizzler;
} // namespace tg
