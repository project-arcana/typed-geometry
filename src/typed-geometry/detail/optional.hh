#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/feature/assert.hh>

namespace tg
{
// lightweight optional for value types
template <class T>
struct optional
{
private:
    T _value = {};
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

    [[nodiscard]] constexpr bool operator==(optional<T> const& rhs) const
    {
        if (!_has_value && !rhs._has_value)
            return true;
        if (!_has_value || !rhs._has_value)
            return false;
        return _value == rhs._value;
    }
    [[nodiscard]] constexpr bool operator!=(optional<T> const& rhs) const { return !operator==(rhs); }

    constexpr optional& operator=(optional const& rhs) noexcept = default;
    constexpr optional& operator=(optional&& rhs) noexcept = default;
};

template <class T, class Stream>
auto operator<<(Stream& os, optional<T> const& o) -> decltype(os << o.value(), os << std::declval<char const*>(), os)
{
    return o.has_value() ? (os << o.value()) : (os << "[empty]");
}
}
