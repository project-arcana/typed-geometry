#pragma once

#include "../types/scalar.hh"
#include "functions/minmax.hh"

/**
 * Provides random generators:
 *  - splitmix
 *  - xorshift
 *  - pcg
 *
 * Default rng: tg::rng
 *
 * Provides detail::uniform01<float / double>(rng) for 0..1 (inclusive)
 */

namespace tg
{
struct splitmix;
struct xorshift;
struct pcg;

// default rng type
using rng = xorshift;

struct splitmix
{
public:
    using result_type = u32;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return tg::max<u32>(); }
    friend bool operator==(splitmix const &, splitmix const &);
    friend bool operator!=(splitmix const &, splitmix const &);

    splitmix() : m_seed(1) {}
    template <class SeedT>
    explicit splitmix(SeedT &&rd)
    {
        seed(rd);
    }

    template <class Rng>
    void seed(Rng &rd)
    {
        m_seed = u64(rd()) << 31 | u64(rd());
    }
    void seed(size_t seed) { m_seed = (seed ^ 0x2b41a160bab708aduLL) + size_t(seed == 0x2b41a160bab708aduLL); }

    result_type operator()()
    {
        u64 z = (m_seed += u64(0x9E3779B97F4A7C15));
        z = (z ^ (z >> 30)) * u64(0xBF58476D1CE4E5B9);
        z = (z ^ (z >> 27)) * u64(0x94D049BB133111EB);
        return result_type((z ^ (z >> 31)) >> 31);
    }

    void discard(unsigned long long n)
    {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

private:
    u64 m_seed;
};

inline bool operator==(splitmix const &lhs, splitmix const &rhs) { return lhs.m_seed == rhs.m_seed; }
inline bool operator!=(splitmix const &lhs, splitmix const &rhs) { return lhs.m_seed != rhs.m_seed; }

struct xorshift
{
public:
    using result_type = u32;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return tg::max<u32>(); }
    friend bool operator==(xorshift const &, xorshift const &);
    friend bool operator!=(xorshift const &, xorshift const &);

    xorshift() : m_seed(0xc1f651c67c62c6e0ull) {}
    template <class SeedT>
    explicit xorshift(SeedT &&rd)
    {
        seed(rd);
    }

    template <class Rng>
    void seed(Rng &rd)
    {
        m_seed = u64(rd()) << 31 | u64(rd());
    }
    void seed(size_t seed) { m_seed = (seed ^ 0x2b41a160bab708aduLL) + size_t(seed == 0x2b41a160bab708aduLL); }

    result_type operator()()
    {
        u64 result = m_seed * 0xd989bcacc137dcd5ull;
        m_seed ^= m_seed >> 11;
        m_seed ^= m_seed << 31;
        m_seed ^= m_seed >> 18;
        return u32(result >> 32ull);
    }

    void discard(unsigned long long n)
    {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

private:
    u64 m_seed;
};

inline bool operator==(xorshift const &lhs, xorshift const &rhs) { return lhs.m_seed == rhs.m_seed; }
inline bool operator!=(xorshift const &lhs, xorshift const &rhs) { return lhs.m_seed != rhs.m_seed; }

struct pcg
{
public:
    using result_type = u32;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return tg::max<u32>(); }
    friend bool operator==(pcg const &, pcg const &);
    friend bool operator!=(pcg const &, pcg const &);

    pcg() : m_state(0x853c49e6748fea9bULL), m_inc(0xda3e39cb94b95bdbULL) {}
    template <class SeedT>
    explicit pcg(SeedT &&rd)
    {
        seed(rd);
    }

    template <class Rng>
    void seed(Rng &rd)
    {
        u64 s0 = u64(rd()) << 31 | u64(rd());
        u64 s1 = u64(rd()) << 31 | u64(rd());

        m_state = 0;
        m_inc = (s1 << 1) | 1;
        (void)operator()();
        m_state += s0;
        (void)operator()();
    }

    result_type operator()()
    {
        u64 oldstate = m_state;
        m_state = oldstate * 6364136223846793005ULL + m_inc;
        u32 xorshifted = u32(((oldstate >> 18u) ^ oldstate) >> 27u);
        int rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    void discard(unsigned long long n)
    {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

private:
    u64 m_state;
    u64 m_inc;
};

inline bool operator==(pcg const &lhs, pcg const &rhs) { return lhs.m_state == rhs.m_state && lhs.m_inc == rhs.m_inc; }
inline bool operator!=(pcg const &lhs, pcg const &rhs) { return lhs.m_state != rhs.m_state || lhs.m_inc != rhs.m_inc; }

namespace detail
{
template <class ScalarT>
struct unit_uniform
{
};
template <>
struct unit_uniform<float>
{
    template <class Rng>
    static float sample(Rng &rng)
    {
        return float(rng()) / float(Rng::max());
    }
};
template <>
struct unit_uniform<double>
{
    template <class Rng>
    static double sample(Rng &rng)
    {
        auto m = u64(Rng::max());
        auto x = rng() * m + rng();
        return double(x) / double(m * m + m);
    }
};
template <class ScalarT, class Rng>
ScalarT uniform01(Rng &rng)
{
    return unit_uniform<ScalarT>::sample(rng);
}
} // namespace detail
} // namespace tg
