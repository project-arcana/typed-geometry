#pragma once

namespace tg
{
template <class A, class B>
constexpr auto operator+=(A& lhs, B const& rhs) -> decltype(lhs = lhs + rhs, lhs)
{
    lhs = lhs + rhs;
    return lhs;
}
template <class A, class B>
constexpr auto operator-=(A& lhs, B const& rhs) -> decltype(lhs = lhs - rhs, lhs)
{
    lhs = lhs - rhs;
    return lhs;
}
template <class A, class B>
constexpr auto operator*=(A& lhs, B const& rhs) -> decltype(lhs = lhs * rhs, lhs)
{
    lhs = lhs * rhs;
    return lhs;
}
template <class A, class B>
constexpr auto operator/=(A& lhs, B const& rhs) -> decltype(lhs = lhs / rhs, lhs)
{
    lhs = lhs / rhs;
    return lhs;
}
template <class A, class B>
constexpr auto operator%=(A& lhs, B const& rhs) -> decltype(lhs = lhs % rhs, lhs)
{
    lhs = lhs % rhs;
    return lhs;
}
} // namespace tg
