#pragma once

namespace tg
{
// software emulation of a 8bit float, i.e. 0..1 normalized
// see vulkan 2.9.1
struct quarter
{
    constexpr quarter() = default;
    constexpr quarter(float f) { data = static_cast<unsigned char>(f < 0 ? 0 : f >= 1 ? 255 : f * 256); }

    // TODO
private:
    unsigned char data = 0;
};

static_assert(sizeof(quarter) == 1, "quarter is not 8bit");

// TODO
constexpr quarter operator-(quarter const& v) { return v; }
constexpr bool operator<(quarter const&, float) { return true; }
} // namespace tg
