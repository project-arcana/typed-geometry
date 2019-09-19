#pragma once

namespace tg
{
struct splitmix;
struct xorshift;
struct pcg;

// default rng type
using rng = xorshift;
}
