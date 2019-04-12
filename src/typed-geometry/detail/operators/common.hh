#pragma once

namespace tg
{
template <class A, class B>
constexpr auto operator+=(A& lhs, B const& rhs) -> decltype(lhs = A(lhs + rhs), lhs)
{
    lhs = A(lhs + rhs);
    return lhs;
}
template <class A, class B>
constexpr auto operator-=(A& lhs, B const& rhs) -> decltype(lhs = A(lhs - rhs), lhs)
{
    lhs = A(lhs - rhs);
    return lhs;
}
template <class A, class B>
constexpr auto operator*=(A& lhs, B const& rhs) -> decltype(lhs = A(lhs * rhs), lhs)
{
    lhs = A(lhs * rhs);
    return lhs;
}
template <class A, class B>
constexpr auto operator/=(A& lhs, B const& rhs) -> decltype(lhs = A(lhs / rhs), lhs)
{
    lhs = A(lhs / rhs);
    return lhs;
}
template <class A, class B>
constexpr auto operator%=(A& lhs, B const& rhs) -> decltype(lhs = A(lhs % rhs), lhs)
{
    lhs = A(lhs % rhs);
    return lhs;
}
} // namespace tg
