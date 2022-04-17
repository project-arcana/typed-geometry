#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

namespace
{
template <class ObjectT>
struct construct
{
};
template <int D, class ScalarT>
struct construct<tg::pos<D, ScalarT>>
{
    static tg::pos<D, ScalarT> uniform(tg::rng& rng)
    {
        auto min = tg::pos<D, ScalarT>(-2);
        auto max = tg::pos<D, ScalarT>(+2);
        auto range = tg::aabb<D, ScalarT>(min, max);
        return tg::uniform(rng, range);
    }
};
template <int D, class ScalarT>
struct construct<tg::aabb<D, ScalarT>>
{
    using pos_t = tg::pos<D, ScalarT>;

    static tg::aabb<D, ScalarT> uniform(tg::rng& rng)
    {
        auto p0 = construct<pos_t>::uniform(rng);
        auto p1 = construct<pos_t>::uniform(rng);
        return aabb_of(p0, p1);
    }
};

template <class ObjectT>
struct object_tester
{
    static auto test_uniform(tg::rng& rng) -> decltype(void(tg::uniform(rng, ObjectT())), void())
    {
        auto o = construct<ObjectT>::uniform(rng);
        auto p = uniform(rng, o);
        CHECK(contains(o, p));
    }
    static auto test_uniform(...)
    { /* uniform not supported */
    }

    static void test(tg::rng& rng) { test_uniform(rng); }
};
} // namespace

FUZZ_TEST("Object")(tg::rng& rng)
{
    object_tester<tg::aabb1>::test(rng);
    object_tester<tg::aabb2>::test(rng);
    object_tester<tg::aabb3>::test(rng);
    object_tester<tg::aabb4>::test(rng);
}
