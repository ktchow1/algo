#include<iostream>
#include<iomanip>
#include<cassert>
#include<arithmetic.h>
#include<utility.h>


void test_bignum()
{
    assert((alg::bignum{"01234"} + alg::bignum{"0"}   ).to_string() == std::string{"1234"});
    assert((alg::bignum{"000"}   + alg::bignum{"1234"}).to_string() == std::string{"1234"});
    assert((alg::bignum{"000"}   + alg::bignum{"0"}   ).to_string() == std::string{"0"});
    
    assert((alg::bignum{"01234"} << 3).to_string() == std::string{"1234000"});
    assert((alg::bignum{"000"}   << 3).to_string() == std::string{"0"});
    assert((alg::bignum{"01234"} << 0).to_string() == std::string{"1234"});
    assert((alg::bignum{"000"}   << 0).to_string() == std::string{"0"});

    assert((alg::bignum{"01234"} * alg::bignum{"0"}   ).to_string() == std::string{"0"});
    assert((alg::bignum{"000"}   * alg::bignum{"1234"}).to_string() == std::string{"0"});
    assert((alg::bignum{"000"}   * alg::bignum{"0"}   ).to_string() == std::string{"0"});
    print_summary("bignum edge case", "all succeeded");

    std::uint32_t trial = 10000;
    std::uint32_t error0 = 0;   
    std::uint32_t error1 = 0;  

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        std::uint64_t a = rand();
        std::uint64_t b = rand();
        std::uint64_t c = a + b;
        alg::bignum x(std::to_string(a));
        alg::bignum y(std::to_string(b));
        alg::bignum z = x + y; 
        if (std::to_string(c) != z.to_string()) ++error0;
    }
  
    for(std::uint32_t t=0; t!=10000; ++t)
    {
        std::uint64_t a = rand();
        std::uint64_t b = rand();
        std::uint64_t c = a * b;
        alg::bignum x(std::to_string(a));
        alg::bignum y(std::to_string(b));
        alg::bignum z = x * y; 
        if (std::to_string(c) != z.to_string()) ++error1;
    }

    print_summary("bignum addition", error0, trial);
    print_summary("bignum multiply", error1, trial);
}

void test_arithmetic()
{
    test_bignum();
}
