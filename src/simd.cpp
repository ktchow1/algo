#include <iostream>
#include <array>
#include <math.h>
#include <time.h>
#include <nmmintrin.h>
#include <immintrin.h>
// SIMD programming


std::uint64_t time_diff(const timespec& t0, const timespec& t1)
{
    std::uint64_t ns0 = t0.tv_sec * 1e9 + t0.tv_nsec;
    std::uint64_t ns1 = t1.tv_sec * 1e9 + t1.tv_nsec;
    return ns1 - ns0;
}

void scalar_process(std::array<float, 40000>& ans)
{
    std::uint64_t N = ans.size();
    float a = 1.0f;
    float b = 2.0f;
    float c = 3.0f;
    float d = 4.0f;

    for(std::uint64_t n=0; n!=N; n+=4)
    {
        ans[n]   = sqrt(a);
        ans[n+1] = sqrt(b);
        ans[n+2] = sqrt(c);
        ans[n+3] = sqrt(d);
        a += 4.0f;
        b += 4.0f;
        c += 4.0f;
        d += 4.0f;
    }
}

void vector_process(std::array<float, 40000>& ans)
{
    std::uint64_t N = ans.size();
    float a = 1.0f;
    float b = 2.0f;
    float c = 3.0f;
    float d = 4.0f;

    for(std::uint64_t n=0; n!=N; n+=4)
    {
        __m128 x = _mm_set_ps(a,b,c,d);
        __m128 y = _mm_sqrt_ps(x);
        ans[n]   = reinterpret_cast<float*>(&y)[3];
        ans[n+1] = reinterpret_cast<float*>(&y)[2];
        ans[n+2] = reinterpret_cast<float*>(&y)[1];
        ans[n+3] = reinterpret_cast<float*>(&y)[0]; 
        a += 4.0f;
        b += 4.0f;
        c += 4.0f;
        d += 4.0f;
    }
}

// ***************************************** // 
// *** Single instruction, multiple data *** //
// ***************************************** // 
void test_simd0()
{
    std::cout << "\nSIMD programming";
    __m128 a = _mm_set_ps(1.0f, 1.0f, 1.0f, 1.0f);
    __m128 b = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
    __m128 c = _mm_add_ps(a,b);
    __m128 d = _mm_sqrt_ps(b);

    // Byte order is reversed 
    std::cout << "\nresult c0 = " << reinterpret_cast<float*>(&c)[3];
    std::cout << "\nresult c1 = " << reinterpret_cast<float*>(&c)[2];
    std::cout << "\nresult c2 = " << reinterpret_cast<float*>(&c)[1];
    std::cout << "\nresult c3 = " << reinterpret_cast<float*>(&c)[0];

    std::cout << "\nresult d0 = " << reinterpret_cast<float*>(&d)[3];
    std::cout << "\nresult d1 = " << reinterpret_cast<float*>(&d)[2];
    std::cout << "\nresult d2 = " << reinterpret_cast<float*>(&d)[1];
    std::cout << "\nresult d3 = " << reinterpret_cast<float*>(&d)[0];
}

void test_simd1()
{
    std::cout << "\nSIMD programming";
    std::array<float, 40000> ans0;
    std::array<float, 40000> ans1;
    timespec t0;
    timespec t1;
    timespec t2;

    clock_gettime(CLOCK_MONOTONIC, &t0);
    scalar_process(ans0);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    vector_process(ans1);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    std::cout << "\ntime for scalar-process = " << time_diff(t0,t1);
    std::cout << "\ntime for vector-process = " << time_diff(t1,t2);

    std::uint64_t error = 0;
    for(std::uint64_t n=0; n!=ans0.size(); ++n)
    {
        if (fabs(ans0[n]-ans1[n]) > 0.0001)
        {
            ++error;
        }
    }
    std::cout << "\nerror count = " << error;
}

