#pragma once

#include <typed-geometry/functions/random/random.hh>

namespace tg
{
struct splitmix;
struct xorshift;
struct pcg;

// default rng type
using rng = xorshift;
}
