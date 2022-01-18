#include <template_specialization_in_header.h>

template<>
void template_function(const ARG2& x)
{
    x.fct();
}

void test_template_specialization_in_header0()
{
    std::cout << "\ntest template specialization in header";

    ARG0 arg0;
    ARG1 arg1;
    ARG2 arg2;

    template_function(arg0);
    template_function(arg1);
    template_function(arg2);
}

void test_template_specialization_in_header1()
{ 
    // Part 1 : Define hash function as custom class
    std::unordered_map<ARG0, std::uint32_t, alg::hash_ARG0> map0;

    ARG0 x0;  x0.str = "abc";
    ARG0 x1;  x1.str = "def";
    ARG0 x2;  x2.str = "ghi";
    map0[x0] = 100; 
    map0[x1] = 200; 
    map0[x2] = 300; 

    for(const auto& [k,v] : map0)
    {
        std::cout << "\nkey = " << k.str << ", value = " << v;
    }

    // Part 2 : Define hash function as specialization of std::hash
    std::unordered_map<ARG1, std::uint32_t> map1;

    ARG1 y0;  y0.str = "abc";
    ARG1 y1;  y1.str = "def";
    ARG1 y2;  y2.str = "ghi";
    map1[y0] = 100; 
    map1[y1] = 200; 
    map1[y2] = 300; 

    for(const auto& [k,v] : map1)
    {
        std::cout << "\nkey = " << k.str << ", value = " << v;
    }
}
