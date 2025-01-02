#include<iostream>
#include<cstdint>
#include<stack>


std::pair<std::uint32_t, std::uint32_t> divide_with_subtraction(std::uint32_t x, std::uint32_t y)
{
    if (y ==0) return {0,0};
    if (y > x) return {0,x};

    std::uint32_t shift_y = y;
    while(shift_y < x) shift_y = shift_y << 1; // Bug : bit shift operator does not modify lhs operand
    shift_y = shift_y >> 1;

    std::uint32_t z = 0;
    while(shift_y >= y)
    {
        if (x >= shift_y)
        {
            z = 2*z+1;
            x = x-shift_y;
        }
        else
        {
            z = 2*z;
        }
        shift_y = shift_y >> 1;
    }
    return {z,x}; // quotient and remainder
}

std::uint64_t power_with_multiplication(std::uint32_t x, std::uint32_t y)
{
    if (y==0) return 1;

    // First of all, convert y into bits array (Is this redundant?)
    std::uint32_t shift_2 = 1;
    while(shift_2 <= y) shift_2 = shift_2 << 1;
    shift_2 = shift_2 >> 1;

    std::stack<bool> bits; // MSB first in, LSB first out
//  while(y > 0)           // BUG : checking against y will result in incorrect result, when ... 
    while(shift_2 > 0)     //       bool representation of y ends with one or more zeros.
    {
        if (y >= shift_2)
        {
            bits.push(true);
            y = y-shift_2;
        }
        else
        {
            bits.push(false);
        }
        shift_2 = shift_2 >> 1;
    }

    // Final part to generate answer
    std::uint64_t z  = 1;
    std::uint64_t xx = x;
    while(!bits.empty())
    {
        if (bits.top()) z *= xx;
        bits.pop();
        xx = xx * xx;
    }
    return z;
}

std::uint64_t power_with_multiplication_optimized(std::uint32_t x, std::uint32_t y)
{
    if (y==0) return 1;

    std::uint64_t z  = 1;
    std::uint64_t xx = x;
    while(y>0)
    {
        if (y%2==1) z *= xx;
        y=y>>1;
        xx = xx * xx;
    }
    return z;
}

