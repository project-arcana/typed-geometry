#include <nexus/test.hh>

#include <chrono>

#include <typed-geometry/feature/swizzling.hh>
#include <typed-geometry/tg-std.hh>

#define GENERATE_SWIZZLERS 0

#if GENERATE_SWIZZLERS
TEST("generate swizzlers")
{
    auto const gen_swizzlers = [](bool remove_dupl, bool remove_wrong_order) {
        for (auto i : {0, 1, 2, 3})
            for (auto j : {0, 1, 2, 3})
            {
                if (remove_dupl && i == j)
                    continue;
                if (remove_wrong_order && j < i)
                    continue;
                std::cout << "constexpr swizzler<" << i << ", " << j << "> "
                          << "xyzw"[i] << "xyzw"[j] << ";" << std::endl;
            }
        std::cout << std::endl;
        for (auto i : {0, 1, 2, 3})
            for (auto j : {0, 1, 2, 3})
                for (auto k : {0, 1, 2, 3})
                {
                    if (remove_dupl && (i == j || i == k || j == k))
                        continue;
                    if (remove_wrong_order && !(i <= j && j <= k))
                        continue;
                    std::cout << "constexpr swizzler<" << i << ", " << j << ", " << k << "> "
                              << "xyzw"[i] << "xyzw"[j] << "xyzw"[k] << ";" << std::endl;
                }
        std::cout << std::endl;
        for (auto i : {0, 1, 2, 3})
            for (auto j : {0, 1, 2, 3})
                for (auto k : {0, 1, 2, 3})
                    for (auto l : {0, 1, 2, 3})
                    {
                        if (remove_dupl && (i == j || i == k || j == k || i == l || j == l || k == l))
                            continue;
                        if (remove_wrong_order && !(i <= j && j <= k && k <= l))
                            continue;
                        std::cout << "constexpr swizzler<" << i << ", " << j << ", " << k << ", " << l << "> "
                                  << "xyzw"[i] << "xyzw"[j] << "xyzw"[k] << "xyzw"[l] << ";" << std::endl;
                    }
        std::cout << std::endl;
    };

    gen_swizzlers(true, true);

    // empty file: 20ms
    // swizzling decl: 21ms
    // all swizzlers: 60ms
    // no dupl, no wrong order: 22ms
    // no dupl, any order: 28ms
    // zapcc NOT faster!
    // for (auto i = 0; i < 10; ++i)
    // {
    //     auto t_start = std::chrono::high_resolution_clock::now();
    //     system("/usr/bin/clang++-7 -std=c++17 -c /tmp/swizzle-test.cc -o /tmp/swizzle-test.o");
    //     // system("zapcc++ -std=c++17 -c /tmp/swizzle-test.cc -o /tmp/swizzle-test.o");
    //     auto t_end = std::chrono::high_resolution_clock::now();
    //     std::cout << std::chrono::duration<double>(t_end - t_start).count() * 1000 << " ms" << std::endl;
    // }
}
#endif

TEST("swizzling")
{
    using namespace tg::swizzling;

    auto v = tg::vec3(1, 2, 3);
    auto const cv = tg::vec4(1, 2, 3, 4);
    auto const cp = tg::pos4(5, 6, 7, 8);
    CHECK(v == tg::vec3(1, 2, 3));
    CHECK(cv == tg::vec4(1, 2, 3, 4));
    CHECK(cp == tg::pos4(5, 6, 7, 8));

    // braced-init-list swizzle
    yxz[v] = {5, 6, 7};
    CHECK(v == tg::vec3(6, 5, 7));

    // self-swizzle
    zxy[v] = v;
    CHECK(v == tg::vec3(5, 7, 6));

    // scalar siwzzle
    xz[v] = 3;
    CHECK(v == tg::vec3(3, 7, 3));

    // sub-swizzle
    yx[v] = {1, 2};
    CHECK(v == tg::vec3(2, 1, 3));

    // rhs swizzle
    v = zxy(v);
    CHECK(v == tg::vec3(3, 2, 1));

    // rhs sub-swizzle
    yz[v] = xy(v);
    CHECK(v == tg::vec3(3, 3, 2));

    // id sub-swizzle
    yz[v] = yz(v);
    CHECK(v == tg::vec3(3, 3, 2));

    // rhs sub-swizzle with cv
    xy[v] = yw(cv);
    CHECK(v == tg::vec3(2, 4, 2));

    // rhs sub-swizzle with rval ref
    yz[v] = xz(tg::vec3(5, 6, 7));
    CHECK(v == tg::vec3(2, 5, 7));

    // rhs swizzle
    v = xyw(cv);
    CHECK(v == tg::vec3(1, 2, 4));

    // repetition
    v = yyx(v);
    CHECK(v == tg::vec3(2, 2, 1));

    // conversion
    v = tg::vec3(xyw(cp));
    CHECK(v == tg::vec3(5, 6, 8));

    // ref-compatible self-swizzle
    yz[v] = yz[v];
    CHECK(v == tg::vec3(5, 6, 8));

    // UFCS swizzle
    v[xz] = {1, 2};
    CHECK(v == tg::vec3(1, 6, 2));

    v[yz] = cv[zz];
    CHECK(v == tg::vec3(1, 3, 3));

    v[xy] = cv(wx);
    CHECK(v == tg::vec3(4, 1, 3));
}
