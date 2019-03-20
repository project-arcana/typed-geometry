#pragma once

namespace tg
{
namespace error
{
template <class T>
struct unknown_scalar_type;
template <class Base, int Bits>
struct unsupported_scalar_type;
template <class T>
struct unknown_type;
template <class A, class B>
struct cannot_promote_types;
template <int D>
struct only_support_four_dimensions;
}
}
