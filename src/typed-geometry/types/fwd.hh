#pragma once

namespace tg
{
using u64 = unsigned long long;

// general purpose types:
template <class T, u64 N>
struct array;
template <class T>
struct span;

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

// special types:
struct splitmix;
struct xorshift;
struct pcg;
template <int... Indices>
struct swizzler;
} // namespace tg
