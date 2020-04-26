#pragma once

#include <typed-geometry/detail/macros.hh>

// least overhead assertion macros
// see https://godbolt.org/z/BvF_yn
// [[unlikely]] produces more code in O0 so it is only used outside of debug
// sizeof(...) is an unevaluated context, thus eliminating any potential side effect
// assertion handler is customizable

// TG_ASSERT(cond) aborts if `cond` is false
// NOTE: the macro must contain side effects!

// compile flags
// TG_ENABLE_ASSERTIONS enables assertions

#ifndef TG_ENABLE_ASSERTIONS
#define TG_ASSERT(condition) TG_UNUSED(bool((condition)))
#else
#define TG_ASSERT(condition) \
    (TG_LIKELY((condition)) ? void(0) : ::tg::detail::assertion_failed({#condition, TG_PRETTY_FUNC, __FILE__, __LINE__})) // force ;
#endif

#ifndef TG_ENABLE_CONTRACTS
#define TG_CONTRACT(condition) TG_UNUSED(bool((condition)))
#else
#define TG_CONTRACT(condition) TG_ASSERT(condition && "contract violation")
#endif

#ifndef TG_ENABLE_INTERNAL_ASSERTIONS
#define TG_INTERNAL_ASSERT(condition) TG_UNUSED(bool((condition)))
#else
#define TG_INTERNAL_ASSERT(condition) TG_ASSERT(condition)
#endif

namespace tg
{
namespace detail
{
struct assertion_info
{
    char const* expr;
    char const* func;
    char const* file;
    int line;
};

[[noreturn]] TG_COLD_FUNC TG_DONT_INLINE void assertion_failed(assertion_info const& info);
}
} // namespace tg

namespace tg
{
/// handler that is called whenever an assertion is violated
/// pass nullptr to reset to default handler
/// this is a thread_local handler
/// the handler must be replaced before it is deleted (non-owning view)
void set_assertion_handler(void (*handler)(detail::assertion_info const& info));
} // namespace tg
