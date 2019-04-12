#pragma once

namespace tg
{
namespace type_error
{
template <class T>
struct unknown_scalar_type;
template <class Base, int Bits>
struct unsupported_scalar_type;
template <class T>
struct unknown_type;
template <class A, class B>
struct cannot_promote_types;
template <class T>
struct unsupported_type;
template <int D>
struct only_support_four_dimensions;
}
}
