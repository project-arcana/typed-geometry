#pragma once

#include <clean-core/macros.hh>

#if defined(CC_COMPILER_MSVC) || defined(CC_COMPILER_CLANG_CL)
#define TG_MUL_U128 _umul128
#else
#define TG_MUL_U128 _mulx_u64
#endif

#include <typed-geometry/functions/fixed_int/conversions.hh>
#include <typed-geometry/functions/fixed_int/fixed_int.hh>
#include <typed-geometry/functions/fixed_int/fixed_int_gen.hh>
#include <typed-geometry/functions/fixed_int/fixed_uint.hh>
#include <typed-geometry/functions/fixed_int/fixed_uint_gen.hh>
