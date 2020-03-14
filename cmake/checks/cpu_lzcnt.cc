#include <cstdint>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

int main()
{
    uint64_t val = 42;
    uint64_t res = _lzcnt_u64(val);
}
