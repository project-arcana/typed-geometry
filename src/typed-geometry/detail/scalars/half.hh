#pragma once

namespace tg
{
// software emulation of 16bit math
struct half
{
    constexpr half() = default;
    constexpr half(float)
    {
        // TODO
        data = 0;
    }

    constexpr operator float() const
    {
        // TODO
        return 0;
    }

    // TODO
private:
    unsigned short data = 0;
};

static_assert(sizeof(half) == 2, "half is not 16bit");

// TODO
constexpr half operator-(half const& v) { return v; }
constexpr bool operator<(half const&, float) { return true; }
constexpr bool operator<(half const&, int) { return true; }
} // namespace tg
