#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

int main()
{
    unsigned long long a;
    unsigned long long b;
    unsigned long long h;
    unsigned long long l = _mulx_u64(a, b, &h);
}
