#pragma once

#include <clean-core/assert.hh>

// least overhead assertion macros
// see https://godbolt.org/z/BvF_yn
// [[unlikely]] produces more code in O0 so it is only used outside of debug
// sizeof(...) is an unevaluated context, thus eliminating any potential side effect
// assertion handler is customizable

// TG_ASSERT(cond) aborts if `cond` is false
// NOTE: the macro must contain side effects!

// compile flags
// TG_ENABLE_ASSERTIONS enables assertions

#define TG_ASSERT(condition) CC_ASSERT(condition)
#define TG_CONTRACT(condition) CC_CONTRACT(condition)
#define TG_INTERNAL_ASSERT(condition) CC_ASSERT(condition)
