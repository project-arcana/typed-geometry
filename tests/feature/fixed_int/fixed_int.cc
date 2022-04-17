// GMP is not installed by default on Windows
#ifndef _MSC_VER

#include <nexus/fuzz_test.hh>

#include <algorithm>
#include <iostream>

#include <gmpxx.h>

#include <typed-geometry/feature/fixed_int.hh>
#include <typed-geometry/feature/std-interop.hh>

namespace
{
template <int w>
struct words_to_type;

template <>
struct words_to_type<1>
{
    using type = tg::i64;
};
template <>
struct words_to_type<2>
{
    using type = tg::i128;
};
template <>
struct words_to_type<3>
{
    using type = tg::i192;
};
template <>
struct words_to_type<4>
{
    using type = tg::i256;
};
template <int w>
using words_to_type_t = typename words_to_type<w>::type;

template <int w>
std::string to_full_binary(tg::fixed_int<w> const& v)
{
    std::string res;


    for (auto wi = w - 1; wi >= 0; wi--)
    {
        for (auto i = 0; i < 64; ++i)
        {
            auto const c = (v.d[wi] >> (63 - i)) & 1;
            if (c == 0)
            {
                res += "0";
            }
            else
            {
                REQUIRE(c == 1);
                res += "1";
            }
        }
    }
    if (res.empty())
        res += "0";
    return res;
}

template <int w>
std::string to_binary(tg::fixed_int<w> const& v)
{
    std::string res;

    auto cpy = v;
    if (tg::detail::less_than_zero(cpy))
    {
        res += "-";
        cpy = -cpy;
    }

    bool remove_zeros = true;
    for (auto wi = w - 1; wi >= 0; wi--)
    {
        for (auto i = 0; i < 64; ++i)
        {
            auto const c = (cpy.d[wi] >> (63 - i)) & 1;
            if (c == 0)
            {
                if (!remove_zeros)
                    res += "0";
            }
            else
            {
                REQUIRE(c == 1);
                res += "1";
                remove_zeros = false;
            }
        }
    }
    if (res.empty())
        res += "0";
    return res;
}

[[maybe_unused]] std::string to_binary(tg::i64 v)
{
    std::string res;

    auto cpy = v;
    if (cpy < 0)
    {
        cpy = -cpy;
        res += "-";
    }

    bool remove_zeros = true;

    for (auto i = 0; i < 64; ++i)
    {
        auto const c = (cpy >> (63 - i)) & 1;
        if (c == 0)
        {
            if (!remove_zeros)
                res += "0";
        }
        else
        {
            REQUIRE(c == 1);
            res += "1";
            remove_zeros = false;
        }
    }
    if (res.empty())
        res += "0";

    return res;
}

template <class IntT>
bool assert_eq(IntT const& a, mpz_class const& b)
{
    auto sa = to_binary(a);
    auto sb = b.get_str(2);
    CHECK(sa == sb);
    return sa == sb;
}

template <int w>
std::pair<words_to_type_t<w>, mpz_class> random_number(tg::rng& rng, int maxb)
{
    using int_t = words_to_type_t<w>;

    // create random number
    std::string s = "";
    int_t i = int_t(0);
    for (auto k = 0; k < maxb - 1; ++k)
    {
        if constexpr (w == 3)
        {
            auto x = tg::detail::imul<4>(i, tg::i64(2));
            i = int_t(x);
        }
        else
        {
            if constexpr (w == 1)
                i = i * 2;
            else
                i = int_t(tg::detail::imul<w>(i, tg::i64(2)));
        }
        if (tg::uniform<bool>(rng))
        {
            i = i + int_t(1);
            s = s + "1";
        }
        else
            s = s + "0";
    }
    mpz_class z;
    z.set_str(s, 2);
    if (tg::uniform<bool>(rng))
    {
        i = -i;
        z = -z;
    }

    return {i, z};
}


template <int w0, int w1>
void check_sum(tg::rng& rng)
{
    auto const [fuia, mpia] = random_number<w0>(rng, (w0 * 64) - 1);
    auto const [fuib, mpib] = random_number<w1>(rng, (w1 * 64) - 1);
    assert_eq(fuia, mpia);
    assert_eq(fuib, mpib);
    assert_eq(fuia + fuib, mpia + mpib);
}

template <int w0, int w1>
void check_diff(tg::rng& rng)
{
    auto const [fuia, mpia] = random_number<w0>(rng, (w0 * 64) - 1);
    auto const [fuib, mpib] = random_number<w1>(rng, (w1 * 64) - 1);
    assert_eq(fuia, mpia);
    assert_eq(fuib, mpib);
    if (mpia <= mpib)
        assert_eq(fuib - fuia, mpib - mpia);
    else
        assert_eq(fuia - fuib, mpia - mpib);
}

template <int w_r, int w0, int w1>
void check_mul(tg::rng& rng)
{
    auto const bits_out = w_r * 64 - 1;
    auto const bits_0 = tg::uniform(rng, 0, int(tg::min(64 * w0 - 1, bits_out) - 1));
    auto const bits_1 = tg::min(bits_out - bits_0 - 1, w1 * 64 - 1);
    REQUIRE(bits_0 + bits_1 <= w_r * 64 - 1);
    REQUIRE(bits_0 <= w0 * 64 - 1);
    REQUIRE(bits_1 <= w1 * 64 - 1);

    auto const [fuia, mpia] = random_number<w0>(rng, bits_0);
    auto const [fuib, mpib] = random_number<w1>(rng, bits_1);

    REQUIRE(fuia == -(-fuia));
    REQUIRE(fuib == -(-fuib));
    assert_eq(fuia, mpia);
    assert_eq(fuib, mpib);
    mpz_class m = mpia * mpib;
    REQUIRE(m.get_str(2).length() < w_r * 64 - 1);
    assert_eq(tg::detail::imul<w_r>(fuia, fuib), mpia * mpib);
}

template <int w>
void check_shift(tg::rng& rng)
{
    // gmp shifts do weird things...
    auto const capped_shift_left = [](mpz_class i, int shift) -> mpz_class {
        mpz_class res = i;
        for (auto i = 0; i < shift; ++i)
            res *= 2;
        return res;
    };
    auto const capped_shift_right = [](mpz_class i, int shift) -> mpz_class {
        mpz_class res = i;
        if (res < 0)
        {
            for (auto k = 0; k < shift; ++k)
                mpz_fdiv_q_ui(res.get_mpz_t(), res.get_mpz_t(), 2);
        }
        else
        {
            for (auto k = 0; k < shift; ++k)
                res /= 2;
        }
        return res;
    };

    auto const [fuia, mpia] = random_number<w>(rng, w * 64 - 1);
    auto const shift = tg::uniform(rng, 0, w * 64 - 1);
    assert_eq(fuia, mpia);

    // signed overflow is undefined
    if (tg::leading_zeros_count(fuia) > tg::u64(shift))
        assert_eq(fuia << shift, capped_shift_left(mpia, shift));

    assert_eq(fuia >> shift, capped_shift_right(mpia, shift));
}

template <int w0, int w1>
void check_div(tg::rng& rng)
{
    auto const bits_out = tg::max(w0, w1) * 64;
    auto const bits_0 = tg::uniform(rng, 0, int(tg::min(64 * w0, bits_out) - 1));
    auto const bits_1 = tg::min(bits_out - bits_0, w1 * 64);
    REQUIRE(bits_0 + bits_1 <= tg::max(w0, w1) * 64);
    REQUIRE(bits_0 <= w0 * 64);
    REQUIRE(bits_1 <= w1 * 64);
    auto const [fuia, mpia] = random_number<w0>(rng, bits_0);
    auto const [fuib, mpib] = random_number<w1>(rng, bits_1);
    if (mpia == 0 || mpib == 0)
        return;
    auto const fuiq = fuia * fuib;
    auto const mpiq = mpia * mpib;
    assert_eq(fuiq, mpiq);
    CHECK(mpiq / mpia == mpib);
    CHECK(fuiq / fuia == fuib);
    CHECK(mpiq / mpib == mpia);
    CHECK(fuiq / fuib == fuia);
}

template <int w0, int w1>
void check_mod(tg::rng& rng)
{
    auto const [fuia, mpia] = random_number<w0>(rng, w0 * 64);
    auto const [fuib, mpib] = random_number<w1>(rng, w1 * 64);
    if (mpia == 0 || mpib == 0)
        return;

    assert_eq(fuia % fuib, mpia % mpib);
    assert_eq(fuib % fuia, mpib % mpia);
}

template <int w0, int w1>
void check_compare(tg::rng& rng)
{
    auto const [fuia, mpia] = random_number<w0>(rng, w0 * 64 - 1);
    auto const [fuib, mpib] = random_number<w1>(rng, w1 * 64 - 1);
    assert_eq(fuia, mpia);
    assert_eq(fuib, mpib);
    if (mpia == mpib)
        CHECK(fuia == fuib);
    if (mpia != mpib)
        CHECK(fuia != fuib);
    if (mpia < mpib)
        CHECK(fuia < fuib);
    if (mpia > mpib)
        CHECK(fuia > fuib);
    if (mpia <= mpib)
        CHECK(fuia <= fuib);
    if (mpia >= mpib)
        CHECK(fuia >= fuib);
}

// template <int w0, int w1>
// void check_bitops(tg::rng& rng)
//{
//    //     gmp seems to use twos-complement internally,
//    //     this is a workaround to make sure inversion works correctly
//    auto const capped_inv = [](mpz_class i, size_t words) -> mpz_class {
//        std::string s;
//        s.resize(words * 64, '1');
//        mpz_class mask;
//        mask.set_str(s, 2);
//        return (mask ^ i);
//    };

//    auto const [i0, m0] = random_number<w0>(rng, w0 * 64 - 1);
//    auto const [i1, m1] = random_number<w1>(rng, w1 * 64 - 1);
//    assert_eq(i0 & i1, m0 & m1);
//    assert_eq(i0 | i1, m0 | m1);
//    assert_eq(i0 ^ i1, m0 ^ m1);
//    assert_eq(~i0, capped_inv(m0, w0));
//    assert_eq(~i1, capped_inv(m1, w1));
//}

template <int w>
void check_incdec(tg::rng& rng)
{
    auto [ia, ma] = random_number<w>(rng, w * 64);
    assert_eq(++ia, ++ma);
    auto [ib, mb] = random_number<w>(rng, w * 64);
    assert_eq(--ia, --ma);
}

template <int w>
void check_neg(tg::rng& rng)
{
    auto [ia, ma] = random_number<w>(rng, w * 64);
    assert_eq(ia, ma);
    assert_eq(-ia, -ma);
}
}

FUZZ_TEST("FixedIntNeg")(tg::rng& rng)
{
    check_neg<2>(rng);
    check_neg<3>(rng);
    check_neg<4>(rng);
}

FUZZ_TEST("FixedIntShift")(tg::rng& rng)
{
    check_shift<2>(rng);
    check_shift<3>(rng);
    check_shift<4>(rng);
}

FUZZ_TEST("FixedIntsum")(tg::rng& rng)
{
    check_sum<2, 1>(rng);
    check_sum<1, 2>(rng);
    check_sum<2, 2>(rng);

    check_sum<3, 1>(rng);
    check_sum<1, 3>(rng);
    check_sum<3, 2>(rng);
    check_sum<2, 3>(rng);
    check_sum<3, 3>(rng);

    check_sum<4, 1>(rng);
    check_sum<1, 4>(rng);
    check_sum<4, 2>(rng);
    check_sum<2, 4>(rng);
    check_sum<4, 3>(rng);
    check_sum<3, 4>(rng);
    check_sum<4, 4>(rng);
}

FUZZ_TEST("FixedIntdiff")(tg::rng& rng)
{
    check_diff<2, 1>(rng);
    check_diff<1, 2>(rng);
    check_diff<2, 2>(rng);
    check_diff<3, 1>(rng);
    check_diff<1, 3>(rng);
    check_diff<3, 2>(rng);
    check_diff<2, 3>(rng);
    check_diff<3, 3>(rng);
    check_diff<4, 1>(rng);
    check_diff<1, 4>(rng);
    check_diff<4, 2>(rng);
    check_diff<2, 4>(rng);
    check_diff<4, 3>(rng);
    check_diff<3, 4>(rng);
    check_diff<4, 4>(rng);
}

FUZZ_TEST("FixedIntMul")(tg::rng& rng)
{
    check_mul<2, 1, 1>(rng);
    check_mul<2, 1, 2>(rng);
    check_mul<2, 2, 1>(rng);
    check_mul<2, 2, 2>(rng);

    check_mul<3, 1, 2>(rng);
    check_mul<3, 2, 1>(rng);
    check_mul<3, 2, 2>(rng);
    check_mul<3, 1, 3>(rng);
    check_mul<3, 3, 1>(rng);
    check_mul<3, 2, 3>(rng);
    check_mul<3, 3, 2>(rng);
    check_mul<3, 3, 3>(rng);

    check_mul<4, 2, 2>(rng);
    check_mul<4, 1, 3>(rng);
    check_mul<4, 3, 1>(rng);
    check_mul<4, 2, 3>(rng);
    check_mul<4, 3, 2>(rng);
    check_mul<4, 3, 3>(rng);
    check_mul<4, 1, 4>(rng);
    check_mul<4, 4, 1>(rng);
    check_mul<4, 2, 4>(rng);
    check_mul<4, 4, 2>(rng);
    check_mul<4, 3, 4>(rng);
    check_mul<4, 4, 3>(rng);
    check_mul<4, 4, 4>(rng);
}

FUZZ_TEST("FixedIntdiv")(tg::rng& rng)
{
    check_div<2, 1>(rng);
    check_div<1, 2>(rng);
    check_div<2, 2>(rng);
    check_div<3, 1>(rng);
    check_div<1, 3>(rng);
    check_div<3, 2>(rng);
    check_div<2, 3>(rng);
    check_div<3, 3>(rng);
    check_div<4, 1>(rng);
    check_div<1, 4>(rng);
    check_div<4, 2>(rng);
    check_div<2, 4>(rng);
    check_div<4, 3>(rng);
    check_div<3, 4>(rng);
    check_div<4, 4>(rng);
}

FUZZ_TEST("FixedIntmod")(tg::rng& rng)
{
    check_mod<2, 1>(rng);
    check_mod<1, 2>(rng);
    check_mod<2, 2>(rng);
    check_mod<3, 1>(rng);
    check_mod<1, 3>(rng);
    check_mod<3, 2>(rng);
    check_mod<2, 3>(rng);
    check_mod<3, 3>(rng);
    check_mod<4, 1>(rng);
    check_mod<1, 4>(rng);
    check_mod<4, 2>(rng);
    check_mod<2, 4>(rng);
    check_mod<4, 3>(rng);
    check_mod<3, 4>(rng);
    check_mod<4, 4>(rng);
}

FUZZ_TEST("FixedIntCompare")(tg::rng& rng)
{
    check_compare<1, 2>(rng);
    check_compare<2, 1>(rng);
    check_compare<2, 2>(rng);

    check_compare<1, 3>(rng);
    check_compare<3, 1>(rng);
    check_compare<2, 3>(rng);
    check_compare<3, 2>(rng);
    check_compare<3, 3>(rng);

    check_compare<1, 4>(rng);
    check_compare<4, 1>(rng);
    check_compare<2, 4>(rng);
    check_compare<4, 2>(rng);
    check_compare<3, 4>(rng);
    check_compare<4, 3>(rng);
    check_compare<3, 4>(rng);
    check_compare<4, 4>(rng);
}

// we dont test this as gmp behaves funny, ie. keeps the sign.
// These should work correctly as they behave exactly the same way the unsigned version of them
// TG_FUZZ_TEST(TypedGeometry, FixedIntBitops)
//{
//    check_bitops<1, 2>(rng);
//    check_bitops<2, 1>(rng);
//    check_bitops<2, 2>(rng);
//    check_bitops<1, 3>(rng);
//    check_bitops<3, 1>(rng);
//    check_bitops<2, 3>(rng);
//    check_bitops<3, 2>(rng);
//    check_bitops<3, 3>(rng);
//    check_bitops<1, 4>(rng);
//    check_bitops<4, 1>(rng);
//    check_bitops<2, 4>(rng);
//    check_bitops<4, 2>(rng);
//    check_bitops<3, 4>(rng);
//    check_bitops<4, 3>(rng);
//    check_bitops<3, 4>(rng);
//    check_bitops<4, 4>(rng);
//}

FUZZ_TEST("FixedIntIncDec")(tg::rng& rng)
{
    check_incdec<2>(rng);
    check_incdec<3>(rng);
    check_incdec<4>(rng);
}

#endif // MSVC check
