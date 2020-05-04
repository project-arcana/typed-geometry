#pragma once

#include <typed-geometry/types/vec.hh>

namespace tg
{
namespace noise
{
// Helper functions for simplex.hh and perlin.hh

template <class ScalarT>
ScalarT mod289(ScalarT const x)
{
    return x - floor(x * (ScalarT(1) / ScalarT(289))) * ScalarT(289);
}

template <int D, class ScalarT>
pos<D, ScalarT> mod289(pos<D, ScalarT> const& x)
{
    return pos<D, ScalarT>(x - floor(x * (ScalarT(1) / ScalarT(289))) * ScalarT(289));
}

template <class ScalarT>
ScalarT permute(ScalarT const x)
{
    return mod289(((x * ScalarT(34)) + ScalarT(1)) * x);
}

template <int D, class ScalarT>
pos<D, ScalarT> permute(const pos<D, ScalarT>& x)
{
    auto ret = comp<D, ScalarT>(x) * ((comp<D, ScalarT>(x) * ScalarT(34)) + ScalarT(1));
    return mod289(pos<D, ScalarT>(ret));
}

template <class ScalarT>
ScalarT taylorInvSqrt(ScalarT const r)
{
    return ScalarT(1.79284291400159) - ScalarT(0.85373472095314) * r;
}

template <int D, class ScalarT>
pos<D, ScalarT> taylorInvSqrt(pos<D, ScalarT> const& r)
{
    return ScalarT(1.79284291400159) - ScalarT(0.85373472095314) * r;
}

template <int D, class ScalarT>
pos<D, ScalarT> fade(const vec<D, ScalarT>& t)
{
    auto ret = comp<D, ScalarT>(t);
    return pos<D, ScalarT>(ret * ret * ret * (ret * (ret * ScalarT(6) - ScalarT(15)) + ScalarT(10)));
}

// See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/step.xhtml
template <int D, class ScalarT>
vec<D, ScalarT> step(const vec<D, ScalarT>& edge, const vec<D, ScalarT>& x)
{
    auto ret = vec<D, ScalarT>::zero;
    for (auto i = 0; i < D; i++)
    {
        if (x[i] >= edge[i])
            ret[i] = ScalarT(1);
        // else: 0
    }
    return ret;
}

// See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/lessThan.xhtml
template <class ScalarT>
vec<4, ScalarT> lessThan(vec<4, ScalarT> const& x, vec<4, ScalarT> const& y)
{
    vec<4, ScalarT> ret;
    for (auto i = 0; i < 4; i++)
    {
        ret[i] = x[i] < y[i] ? ScalarT(1) : ScalarT(0);
    }
    return ret;
}

template <class ScalarT>
vec<4, ScalarT> grad4(ScalarT j, vec<4, ScalarT> ip)
{
    auto ones = vec<4, ScalarT>::one;
    ones.w = -1;
    vec<4, ScalarT> p, s;

    auto res = floor(fract(vec<3, ScalarT>(comp<3, ScalarT>(j) * comp<3, ScalarT>(ip.x, ip.y, ip.z))) * ScalarT(7)) * ip.z - ScalarT(1);
    p.x = res.x;
    p.y = res.y;
    p.z = res.z;
    p.w = ScalarT(1.5) - dot(abs(vec<3, ScalarT>(p.x, p.y, p.z)), vec<3, ScalarT>(ones.x, ones.y, ones.z));
    s = vec<4, ScalarT>(lessThan(p, vec<4, ScalarT>::zero));

    auto xyz = comp<3, ScalarT>(p.x, p.y, p.z) + (comp<3, ScalarT>(s.x, s.y, s.z) * ScalarT(2) - comp<3, ScalarT>(1)) * comp<3, ScalarT>(s.w);
    p.x = xyz[0];
    p.y = xyz[1];
    p.z = xyz[2];

    return p;
}

template <int D, class ScalarT>
vec<D, ScalarT> clamp(vec<D, ScalarT> const& v, const ScalarT minval, const ScalarT maxval)
{
    vec<D, ScalarT> ret;
    for (auto i = 0; i < D; i++)
    {
        ret[i] = max(min(v[i], maxval), minval);
    }
    return ret;
}

// Helper functions (and data) for simplex noise https://github.com/SRombauts/SimplexNoise/blob/master/src/SimplexNoise.cpp
/**
 * Permutation table. This is just a random jumble of all numbers 0-255.
 *
 * This produce a repeatable pattern of 256, but Ken Perlin stated
 * that it is not a problem for graphic texture as the noise features disappear
 * at a distance far enough to be able to see a repeatable pattern of 256.
 *
 * This needs to be exactly the same for all instances on all platforms,
 * so it's easiest to just keep it as static explicit data.
 * This also removes the need for any initialisation of this class.
 *
 * Note that making this an uint32_t[] instead of a uint8_t[] might make the
 * code run faster on platforms with a high penalty for unaligned single
 * byte addressing. Intel x86 is generally single-byte-friendly, but
 * some other CPUs are faster with 4-aligned reads.
 * However, a char[] is smaller, which avoids cache trashing, and that
 * is probably the most important aspect on most architectures.
 * This array is accessed a *lot* by the noise functions.
 * A vector-valued noise over 3D accesses it 96 times, and a
 * float-valued 4D noise 64 times. We want this to fit in the cache!
 */
static const tg::u8 perm[256] = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,
    190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,
    125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,
    41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130,
    116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207,
    206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,
    172, 9,   129, 22,  39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144,
    12,  191, 179, 162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,
    127, 4,   150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156, 180};

/**
 * Helper function to hash an integer using the above permutation table
 *
 *  This inline function costs around 1ns, and is called N+1 times for a noise of N dimension.
 *
 *  Using a real hash function would be better to improve the "repeatability of 256" of the above permutation table,
 * but fast integer Hash functions uses more time and have bad random properties.
 *
 * @param[in] i Integer value to hash
 *
 * @return 8-bits hashed value
 */
static inline tg::u8 hash(tg::i32 i) { return perm[static_cast<tg::u8>(i)]; }

/**
 * Helper function to compute gradients-dot-residual vectors (1D)
 *
 * @note that these generate gradients of more than unit length. To make
 * a close match with the value range of classic Perlin noise, the final
 * noise values need to be rescaled to fit nicely within [-1,1].
 * (The simplex noise functions as such also have different scaling.)
 * Note also that these noise functions are the most practical and useful
 * signed version of Perlin noise.
 *
 * @param[in] hash  hash value
 * @param[in] x     distance to the corner
 *
 * @return gradient value
 */
template <class ScalarT>
static ScalarT grad(tg::i32 hash, ScalarT x)
{
    const tg::i32 h = hash & 0x0F;       // Convert low 4 bits of hash code
    ScalarT grad = ScalarT(1) + (h & 7); // Gradient value 1.0, 2.0, ..., 8.0
    if ((h & 8) != 0)
        grad = -grad;  // Set a random sign for the gradient
                       //  float grad = gradients1D[h];    // NOTE : Test of Gradient look-up table instead of the above
    return (grad * x); // Multiply the gradient with the distance
}

/**
 * Helper functions to compute gradients-dot-residual vectors (2D)
 *
 * @param[in] hash  hash value
 * @param[in] x     x coord of the distance to the corner
 * @param[in] y     y coord of the distance to the corner
 *
 * @return gradient value
 */
template <class ScalarT>
static ScalarT grad(tg::i32 hash, ScalarT x, ScalarT y)
{
    const tg::i32 h = hash & 0x3F;   // Convert low 3 bits of hash code
    const ScalarT u = h < 4 ? x : y; // into 8 simple gradient directions,
    const ScalarT v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -ScalarT(2) * v : ScalarT(2) * v); // and compute the dot product with (x,y).
}

/**
 * Helper functions to compute gradients-dot-residual vectors (3D)
 *
 * @param[in] hash  hash value
 * @param[in] x     x coord of the distance to the corner
 * @param[in] y     y coord of the distance to the corner
 * @param[in] z     z coord of the distance to the corner
 *
 * @return gradient value
 */
template <class ScalarT>
static ScalarT grad(tg::i32 hash, ScalarT x, ScalarT y, ScalarT z)
{
    tg::i32 h = hash & 15;                              // Convert low 4 bits of hash code into 12 simple
    ScalarT u = h < 8 ? x : y;                          // gradient directions, and compute dot product.
    ScalarT v = h < 4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

/**
 * Computes the largest integer value not greater than the float one
 *
 * This method is faster than using (int32_t)std::floor(fp).
 *
 * I measured it to be approximately twice as fast:
 *  float:  ~18.4ns instead of ~39.6ns on an AMD APU),
 *  double: ~20.6ns instead of ~36.6ns on an AMD APU),
 * Reference: http://www.codeproject.com/Tips/700780/Fast-floor-ceiling-functions
 *
 * @param[in] fp    float input value
 *
 * @return largest integer value not greater than fp
 */
template <class ScalarT>
static inline tg::i32 fastfloor(ScalarT fp)
{
    auto i = static_cast<tg::i32>(fp);
    return (fp < i) ? (i - 1) : (i);
}


} // namespace noise
} // namespace tg
