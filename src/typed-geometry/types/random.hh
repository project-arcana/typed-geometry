#pragma once

#include <typed-geometry/common/random.hh>

namespace tg
{
struct splitmix;
struct xorshift;
struct pcg;

// default rng type
using rng = xorshift;
}
