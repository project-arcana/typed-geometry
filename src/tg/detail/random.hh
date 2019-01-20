#pragma once

#include <cstddef>
#include <random>

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
    using result_type = uint32_t;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return UINT32_MAX; }
    friend bool operator==(splitmix const &, splitmix const &);
    friend bool operator!=(splitmix const &, splitmix const &);

    splitmix() : m_seed(1) {}
    explicit splitmix(std::random_device &rd) { seed(rd); }

    void seed(std::random_device &rd) { m_seed = uint64_t(rd()) << 31 | uint64_t(rd()); }

    result_type operator()()
    {
        uint64_t z = (m_seed += UINT64_C(0x9E3779B97F4A7C15));
        z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
        z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
        return result_type((z ^ (z >> 31)) >> 31);
    }

    void discard(unsigned long long n)
    {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

private:
    uint64_t m_seed;
};

inline bool operator==(splitmix const &lhs, splitmix const &rhs) { return lhs.m_seed == rhs.m_seed; }
inline bool operator!=(splitmix const &lhs, splitmix const &rhs) { return lhs.m_seed != rhs.m_seed; }

struct xorshift
{
public:
    using result_type = uint32_t;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return UINT32_MAX; }
    friend bool operator==(xorshift const &, xorshift const &);
    friend bool operator!=(xorshift const &, xorshift const &);

    xorshift() : m_seed(0xc1f651c67c62c6e0ull) {}
    explicit xorshift(std::random_device &rd) { seed(rd); }

    void seed(std::random_device &rd) { m_seed = uint64_t(rd()) << 31 | uint64_t(rd()); }
    void seed(size_t seed) { m_seed = uint64_t(seed | 1) << 31 | uint64_t(seed | 1); }

    result_type operator()()
    {
        uint64_t result = m_seed * 0xd989bcacc137dcd5ull;
        m_seed ^= m_seed >> 11;
        m_seed ^= m_seed << 31;
        m_seed ^= m_seed >> 18;
        return uint32_t(result >> 32ull);
    }

    void discard(unsigned long long n)
    {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

private:
    uint64_t m_seed;
};

inline bool operator==(xorshift const &lhs, xorshift const &rhs) { return lhs.m_seed == rhs.m_seed; }
inline bool operator!=(xorshift const &lhs, xorshift const &rhs) { return lhs.m_seed != rhs.m_seed; }

struct pcg
{
public:
    using result_type = uint32_t;
    static constexpr result_type(min)() { return 0; }
    static constexpr result_type(max)() { return UINT32_MAX; }
    friend bool operator==(pcg const &, pcg const &);
    friend bool operator!=(pcg const &, pcg const &);

    pcg() : m_state(0x853c49e6748fea9bULL), m_inc(0xda3e39cb94b95bdbULL) {}
    explicit pcg(std::random_device &rd) { seed(rd); }

    void seed(std::random_device &rd)
    {
        uint64_t s0 = uint64_t(rd()) << 31 | uint64_t(rd());
        uint64_t s1 = uint64_t(rd()) << 31 | uint64_t(rd());

        m_state = 0;
        m_inc = (s1 << 1) | 1;
        (void)operator()();
        m_state += s0;
        (void)operator()();
    }

    result_type operator()()
    {
        uint64_t oldstate = m_state;
        m_state = oldstate * 6364136223846793005ULL + m_inc;
        uint32_t xorshifted = uint32_t(((oldstate >> 18u) ^ oldstate) >> 27u);
        int rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    void discard(unsigned long long n)
    {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

private:
    uint64_t m_state;
    uint64_t m_inc;
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
        auto m = uint64_t(Rng::max());
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
