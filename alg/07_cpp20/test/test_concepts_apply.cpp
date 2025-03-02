#include<iostream>
#include<cassert>
#include<cstdint>
#include<vector>
#include<concepts>

// *** alg *** //
#include<utility.h>




// ***************************** //
// *** Applying std concepts *** //
// ***************************** //
namespace test
{
    // define is_traits
    // compare with SFINAE



}




void test_concepts_apply()
{

}






// ************************************************************************************************************** //
// This is a abbv function template using auto (with concept).
// With auto, it accepts all function pointer, lambda, std::function by keeping their own types.
// Without auto, we need to use std::function as arg type, everything is convert to std::function, which is slow.
// ************************************************************************************************************** //
void execute(std::invocable<std::uint32_t, const std::vector<std::uint32_t>&> auto callback, auto init, const auto& data)
{
    std::cout << "\nexecute " << callback(init, data);
}

auto fct(std::uint32_t s, const std::vector<std::uint32_t>& v)
{
    std::uint32_t sum = s;
    for(const auto& x:v) s+=x;
    return s;
}

void test_template_AFT_invocable()
{
    std::vector<std::uint32_t> vec{1,2,3,4,5,6,7,8,9,10};

    execute(fct, 100, vec);
    execute([](std::uint32_t s, const std::vector<std::uint32_t>& v)
    {
        std::uint32_t sum = s;
        for(const auto& x:v) s+=x;
        return s;
    },
    100, std::vector<std::uint32_t>{1,2,3,4,5,6,7,8,9,10});
}
