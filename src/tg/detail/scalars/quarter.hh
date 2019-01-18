#pragma once

#include <cstdint>

namespace tg
{
// software emulation of a 8bit float, i.e. 0..1 normalized
// see vulkan 2.9.1
struct quarter
{
    constexpr quarter() = default;
    constexpr quarter(float f) { data = f < 0 ? 0 : f >= 1 ? 255 : static_cast<int>(f * 256); }

    // TODO
private:
    std::uint8_t data = 0;
};

// TODO
constexpr quarter operator-(quarter const& v) { return v; }
constexpr bool operator<(quarter const& a, float b) { return true; }
} // namespace tg
