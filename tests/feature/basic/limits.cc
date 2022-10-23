#include <nexus/test.hh>

#include <typed-geometry/feature/basic.hh>

#include <limits>

TEST("Limits")
{
    using namespace tg;

    CHECK(std::numeric_limits<i8>::lowest() == min<i8>());
    CHECK(std::numeric_limits<i16>::lowest() == min<i16>());
    CHECK(std::numeric_limits<i32>::lowest() == min<i32>());
    CHECK(std::numeric_limits<i64>::lowest() == min<i64>());

    CHECK(std::numeric_limits<i8>::max() == max<i8>());
    CHECK(std::numeric_limits<i16>::max() == max<i16>());
    CHECK(std::numeric_limits<i32>::max() == max<i32>());
    CHECK(std::numeric_limits<i64>::max() == max<i64>());


    CHECK(std::numeric_limits<u8>::lowest() == min<u8>());
    CHECK(std::numeric_limits<u16>::lowest() == min<u16>());
    CHECK(std::numeric_limits<u32>::lowest() == min<u32>());
    CHECK(std::numeric_limits<u64>::lowest() == min<u64>());

    CHECK(std::numeric_limits<u8>::max() == max<u8>());
    CHECK(std::numeric_limits<u16>::max() == max<u16>());
    CHECK(std::numeric_limits<u32>::max() == max<u32>());
    CHECK(std::numeric_limits<u64>::max() == max<u64>());


    // CHECK(std::numeric_limits<f8>::lowest() == min<f8>());
    // CHECK(std::numeric_limits<f16>::lowest() == min<f16>());
    CHECK(std::numeric_limits<f32>::lowest() == min<f32>());
    CHECK(std::numeric_limits<f64>::lowest() == min<f64>());

    // CHECK(std::numeric_limits<f8>::max() == max<f8>());
    // CHECK(std::numeric_limits<f16>::max() == max<f16>());
    CHECK(std::numeric_limits<f32>::max() == max<f32>());
    CHECK(std::numeric_limits<f64>::max() == max<f64>());

    CHECK(std::numeric_limits<f32>::epsilon() == epsilon<f32>);
    CHECK(std::numeric_limits<f64>::epsilon() == epsilon<f64>);

    CHECK(1.f < 1 + epsilon<f32>);
    CHECK(1.f == 1 + epsilon<f32> / 2);

    CHECK(1. < 1 + epsilon<f64>);
    CHECK(1. == 1 + epsilon<f64> / 2);

    CHECK(tg::is_inf(tg::inf<float>));
    CHECK(tg::is_inf(tg::inf<double>));
}
