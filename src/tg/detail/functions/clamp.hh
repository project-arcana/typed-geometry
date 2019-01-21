#pragma once
#include "../../types/scalar.hh"
#include "minmax.hh"

namespace tg
{
template <class ScalarT>
constexpr ScalarT clamp(ScalarT const& a, ScalarT const& min_value, ScalarT const& max_value)
{
    return min(max(a, min_value), max_value);
}
} // namespace tg
