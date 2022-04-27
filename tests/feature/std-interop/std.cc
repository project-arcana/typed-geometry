#include <nexus/test.hh>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <iostream>
#include <sstream>

#include <typed-geometry/tg-std.hh>

namespace
{
template <class F>
void test_all(F&& f)
{
    f(tg::pos1());
    f(tg::ipos2());
    f(tg::dpos3());
    f(tg::upos4());

    f(tg::usize1());
    f(tg::dsize2());
    f(tg::isize3());
    f(tg::size4());

    f(tg::dvec1());
    f(tg::vec2());
    f(tg::uvec3());
    f(tg::ivec4());

    f(tg::dcomp1());
    f(tg::comp2());
    f(tg::ucomp3());
    f(tg::icomp4());

    f(tg::ddir1());
    f(tg::dir2());
    f(tg::idir4());

    f(tg::color3());
    f(tg::color4());

    f(tg::mat2());
    f(tg::imat3());
    f(tg::dmat4());
    f(tg::mat2x3());
    f(tg::dmat4x2());

    f(tg::triangle2());
    f(tg::itriangle3());
    f(tg::triangle3());

    f(tg::iaabb1());
    f(tg::daabb2());
    f(tg::aabb3());
    f(tg::uaabb4());
}
} // namespace

TEST("StdIo")
{
    test_all(
        [](auto v)
        {
            std::stringstream ss;
            ss << v;
        });

    CHECK(true); // compile check only
}

TEST("StdLess")
{
    test_all(
        [](auto v)
        {
            using T = decltype(v);
            std::map<T, int> m;
            std::set<T> s;
        });

    CHECK(true); // compile check only
}

TEST("StdHash")
{
    test_all(
        [](auto v)
        {
            using T = decltype(v);
            std::unordered_map<T, int> um;
            std::unordered_set<T> us;
        });

    CHECK(true); // compile check only
}

// NOTE: currently, msvc has some internal error for this one
#ifndef CC_OS_WINDOWS
TEST("StdTrivialTypes")
{
    test_all(
        [](auto v)
        {
            using T = decltype(v);

            static_assert(std::is_standard_layout<T>::value);

            static_assert(std::is_trivially_copyable<T>::value);
            static_assert(std::is_trivially_assignable<T&, T>::value);
            static_assert(std::is_trivially_destructible<T>::value);
            static_assert(std::is_trivially_copy_constructible<T>::value);
            static_assert(std::is_trivially_copy_assignable<T>::value);
            static_assert(std::is_trivially_move_constructible<T>::value);
            static_assert(std::is_trivially_move_assignable<T>::value);

            static_assert(std::is_nothrow_assignable<T&, T>::value);
            static_assert(std::is_nothrow_destructible<T>::value);
            static_assert(std::is_nothrow_constructible<T>::value);
            static_assert(std::is_nothrow_copy_assignable<T>::value);
            static_assert(std::is_nothrow_copy_constructible<T>::value);
            static_assert(std::is_nothrow_move_assignable<T>::value);
            static_assert(std::is_nothrow_move_constructible<T>::value);
            static_assert(std::is_nothrow_default_constructible<T>::value);

            // static_assert(std::has_unique_object_representations<T>::value);

            // non-trivial ctor: we want zero-init!
            static_assert(!std::is_trivially_default_constructible<T>::value);
            static_assert(!std::is_trivially_constructible<T>::value);
        });

    CHECK(true); // compile check only
}
#endif
