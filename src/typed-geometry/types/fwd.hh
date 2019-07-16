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

// object types:
template <int D, class ScalarT>
struct line;
template <int D, class ScalarT>
struct ray;
template <int D, class ScalarT>
struct segment;
} // namespace tg
