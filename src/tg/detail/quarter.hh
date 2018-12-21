#pragma once

#include <cstdint>

namespace tg
{
// software emulation of a 8bit float, i.e. 0..1 normalized
struct quarter
{
    // TODO
private:
    std::uint8_t data;
};

// TODO
constexpr quarter operator-(quarter const& v) { return v; }
constexpr bool operator<(quarter const& a, float b) { return true; }
} // namespace tg
