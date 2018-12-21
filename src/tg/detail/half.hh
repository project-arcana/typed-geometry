#pragma once

#include <cstdint>

namespace tg
{
// software emulation of 16bit math
struct half
{
    // TODO
private:
    uint16_t data;
};

// TODO
constexpr half operator-(half const& v) { return v; }
constexpr bool operator<(half const& a, float b) { return true; }
} // namespace tg
