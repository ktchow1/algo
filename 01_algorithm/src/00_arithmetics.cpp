#include <iostream>
#include <stack>


struct bignum
{
    bignum() : impl("0") {}
    explicit bignum(std::uint64_t x) : impl(std::to_string(x)) 
    {

    }

    explicit bignum(const std::string& x) : impl(x) {}
    explicit bignum(const std::stack<char>& x)
    {
        auto temp = x;
        while(!temp.empty())
        {
            impl.push_back(temp.top());
            temp.pop();
        }
    }

    bignum& times_10()
    {
        if (impl != "0")
        {
            impl.push_back('0');
        }
        return *this;
    }

    std::string impl;
};

std::ostream& operator<<(std::ostream& os, const bignum& x)
{
    os << x.impl;
    return os;
}


// ************************************************* //
// Remark
// 1. Like merge sort :
// -  output the overlapping least sign digit part
// -  output the non-overlapping part
// -  output carry
// 2. find digit by (temp%10), do not use if
// 3. handle zero
// -  init as "0"
// -  do not return "000...000"
// ************************************************* //
bignum operator+(const bignum& lhs, const bignum& rhs) // able to handle if lhs or rhs is zero
{
    std::stack<char> result;
    std::int32_t n0 = lhs.impl.size()-1;
    std::int32_t n1 = rhs.impl.size()-1;
    std::uint32_t carry = 0;

    while(n0>=0 && n1>=0)
    {
        std::uint32_t temp = (lhs.impl[n0]-'0') +
                             (rhs.impl[n1]-'0') +
                             carry;
        carry = temp / 10;        
        result.push('0'+temp%10);
        --n0;
        --n1;
    }
    while(n0>=0)
    {
        std::uint32_t temp = (lhs.impl[n0]-'0') + carry;
        carry = temp / 10;        
        result.push('0'+temp%10);
        --n0;
    }
    while(n1>=0)
    {
        std::uint32_t temp = (rhs.impl[n1]-'0') + carry;
        carry = temp / 10;        
        result.push('0'+temp%10);
        --n1;
    }
    if (carry > 0) 
    {
        result.push('0'+carry); // carry <= 9 even for multiplication with carry
    }
    return bignum{result};
}

bignum operator*(const bignum& lhs, const std::uint32_t& rhs) // rhs should be single digit
{
    // Avoid returning bignum{"00000"}
    if (rhs==0) return bignum{0}; 

    std::stack<char> result;
    std::int32_t n = lhs.impl.size()-1;
    std::uint32_t carry = 0;

    while(n>=0)
    {
        std::uint32_t temp = (lhs.impl[n]-'0') * rhs + carry;
        carry = temp / 10;        
        result.push('0'+temp%10);
        --n;
    }
    if (carry > 0)
    {
        result.push('0'+carry); // carry <= 9 even for multiplication with carry
    } 
    return bignum{result};
}

bignum operator*(const bignum& lhs, const bignum& rhs) 
{
    bignum result; // default as zero
    for(const char& c : rhs.impl)
    {
        result.times_10();
        result = result + lhs * static_cast<std::uint32_t>(c-'0');
    }
    return result;
}

template<std::uint32_t BASE> 
std::uint32_t reverse_with_bit_shift(std::uint32_t x)
{
    std::uint32_t y = 0;
    while(x > 0)
    {
        y = y*BASE + x%BASE;
        x = x/BASE;
    }
    return y;
}

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

std::uint64_t power_with_loop(std::uint32_t x, std::uint32_t y)
{
    std::uint64_t z=1;

    for(std::uint32_t n=0; n!=y; ++n) z = z*x;
    return z;
}

void test_bignum()
{
    // Addition
    for(std::uint32_t n=0; n!=10; ++n)
    {
        std::uint64_t x = rand();
        std::uint64_t y = rand();
        bignum z = bignum(x) + bignum(y);
        std::uint64_t w = x + y;
        std::cout << "\nbignum " << x << " + " << y << " = " << z << ", checking : " << w << (z.impl==std::to_string(w)? " correct" : " incorrect");
    }

    // Multiplication with single digit
    for(std::uint32_t n=0; n!=10; ++n)
    {
        std::uint64_t x = rand();
        std::uint32_t y = rand() % 10; // support zero
        bignum z = bignum(x) * y;
        std::uint64_t w = x * y;
        std::cout << "\nbignum " << x << " * " << y << " = " << z << ", checking : " << w << (z.impl==std::to_string(w)? " correct" : " incorrect");
    }

    // Multiplication
    for(std::uint32_t n=0; n!=100; ++n)
    {
        std::uint64_t x = rand() % (65536 * 16);
        std::uint64_t y = rand() % (65536 * 16);
        bignum z = bignum(x) * bignum(y);
        std::uint64_t w = x * y;
        std::cout << "\nbignum " << x << " * " << y << " = " << z << ", checking : " << w << (z.impl==std::to_string(w)? " correct" : " incorrect");
    }
}

void test_reverse_with_bit_shift()
{
    std::uint32_t x,y;

    x = 1234567;   y = reverse_with_bit_shift<10>(x);  std::cout << "\nreverse of " << x << " is " << y;
    x = 1230000;   y = reverse_with_bit_shift<10>(x);  std::cout << "\nreverse of " << x << " is " << y;
    x = 10203040;  y = reverse_with_bit_shift<10>(x);  std::cout << "\nreverse of " << x << " is " << y;
    x = 1024;      y = reverse_with_bit_shift<2>(x);   std::cout << "\nreverse of " << x << " is " << y;
    x = 1022;      y = reverse_with_bit_shift<2>(x);   std::cout << "\nreverse of " << x << " is " << y;
}

void test_divide_with_subtraction()
{
    for(std::uint32_t n=0; n!=50; ++n)
    {
        std::uint32_t x = rand()%10000;
        std::uint32_t y = rand()%100;
        auto [q,r] = divide_with_subtraction(x,y);
        std::cout << "\ndivision " << x << "/" << y << " = " << q << " ... " << r << ", checking : " << y*q+r << (y*q+r==x? " correct": " incorrect");
    }
}

void test_power_with_multiplication()
{
    // 1. When overflow happens, both functions can generate consistent result, not sure whether they are correct.
    // 2.      Overflow happens for 4^42.
    //
    for(std::uint32_t n=0; n!=50; ++n)
    {
        std::uint32_t x = rand()%15+1; 
        std::uint32_t y = rand()%30+1;
//      auto z = power_with_multiplication(x,y);
        auto z = power_with_multiplication_optimized(x,y);
        auto w = power_with_loop(x,y);
        std::cout << "\npower " << x << "^" << y << " = " << z << ", checking : " << w << (z==w? " correct": " incorrect");
    }
}

void test_arithmetics()
{
//  test_bignum();
//  test_reverse_with_bit_shift();
//  test_divide_with_subtraction();
    test_power_with_multiplication();
}
