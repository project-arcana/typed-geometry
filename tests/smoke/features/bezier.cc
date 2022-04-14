#include <typed-geometry/feature/bezier.hh>

// intentionally left empty.
// (tests if the include works standalone)

[[maybe_unused]] static void smoke_test_bezier()
{
    auto b = tg::make_bezier(tg::pos3(0), tg::pos3(1), tg::pos3(2));
    auto p = b[0.1f];
    (void)p;
}
