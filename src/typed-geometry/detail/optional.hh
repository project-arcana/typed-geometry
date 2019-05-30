#pragma once

#include <typed-geometry/common/assert.hh>
#include <typed-geometry/detail/utility.hh>

namespace tg
{
// lightweight optional for value types
template <class T>
struct optional
{
private:
    T _value;
    bool _has_value = false;

public:
    constexpr optional() = default;
    constexpr optional(T const& v) : _value(v), _has_value(true) {}
    constexpr optional(optional const&) noexcept = default;
    constexpr optional(optional&&) noexcept = default;

    [[nodiscard]] constexpr bool has_value() const { return _has_value; }

    [[nodiscard]] constexpr T value() const
    {
        TG_CONTRACT(has_value());
        return _value;
    }

    [[nodiscard]] constexpr bool operator==(T const& rhs) const { return _has_value && _value == rhs; }
    [[nodiscard]] constexpr bool operator!=(T const& rhs) const { return !_has_value || _value != rhs; }

    constexpr optional& operator=(optional const& rhs) noexcept = default;
    constexpr optional& operator=(optional&& rhs) noexcept = default;
};

template <class T, class Stream>
auto operator<<(Stream& os, optional<T> const& o) -> decltype(os << o.value(), os << "", os)
{
    return o.has_value() ? (os << o.value()) : (os << "[empty]");
}
}
