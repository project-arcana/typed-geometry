#pragma once

namespace tg
{
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

// object types
template <int D, class ScalarT>
struct line;
template <int D, class ScalarT>
struct ray;
template <int D, class ScalarT>
struct segment;
} // namespace tg
