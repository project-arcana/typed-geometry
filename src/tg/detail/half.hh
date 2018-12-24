#pragma once

#include <cstdint>

namespace tg
{
// software emulation of 16bit math
struct half
{
    constexpr half() = default;
    constexpr half(float f)
    {
        // TODO
        data = 0;
    }

    // TODO
private:
    std::uint16_t data = 0;
};

// TODO
constexpr half operator-(half const& v) { return v; }
constexpr bool operator<(half const& a, float b) { return true; }
} // namespace tg
