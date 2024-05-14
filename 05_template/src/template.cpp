#include<iostream>
#include<vector>
#include<string>
#include<tuple>


// Template class 
// 
// When used as type-type mapping, template struct is like a multivariate function.

template<typename T0, typename T1, typename T2> // <--- line 1
struct sample_class
{
    using output_type = std::tuple<T0,T1,T2>;
};

// We define specialization like below :
// (1) make sure specialization is a subset of generic template
// (2) feasible-set defined by line 3 is a subset of feasible-set defined by line 1
// (3) feasible-set defined by line 2 is not related feasible-set defined by line 1
//

template<typename U0, typename U1, typename U2, typename U3>              // <--- line 2
struct sample_class<std::pair<U0,U1>, std::pair<U1,U2>, std::pair<U2,U3>> // <--- line 3
{
    using output_type = std::tuple<U0,U1,U2,U3>;
}; 

void test_template_class_specialization()
{
    using Y0 = sample_class<std::uint32_t, std::string, std::vector<std::uint32_t>>::output_type;
    static_assert(std::is_same_v<Y0, std::tuple<std::uint32_t, 
                                                std::string, 
                                                std::vector<std::uint32_t>>>, "type mapping incorrect");

    using Y1 = sample_class<std::pair<char,          std::uint32_t>,
                            std::pair<std::uint32_t, std::string>, 
                            std::pair<std::string,   std::vector<std::uint32_t>>>::output_type;
    static_assert(std::is_same_v<Y1, std::tuple<char, 
                                                std::uint32_t, 
                                                std::string, 
                                                std::vector<std::uint32_t>>>, "type mapping incorrect");

    using Y2 = sample_class<std::pair<char,          std::uint32_t>,
                            std::pair<std::string,   std::string>, 
                            std::pair<std::string,   std::vector<std::uint32_t>>>::output_type;
    static_assert(std::is_same_v<Y2, std::tuple<std::pair<char,        std::uint32_t>,
                                                std::pair<std::string, std::string>, 
                                                std::pair<std::string, std::vector<std::uint32_t>>>>, "type mapping incorrect");
}

// Unlike template class, there is no such limitation on template function,  
// as template functions are just function overload. 
// We can even put parameter after parameter-pack (see line A).

template<typename...T>  
void sample_function(const T&... x)
{
    std::cout << "\nsample function = overrload 0";
}

template<typename...T, typename U> // <--- line A
void sample_function(const std::tuple<T..., U>& tup) 
{
    std::cout << "\nsample function = overrload 1";
}

template<typename T0, typename T1, typename T2> 
void sample_function(const T0& x0, const T1& x1, const T2& x2)
{
    std::cout << "\nsample function = overrload 2";
}

template<typename T0, typename T1, typename T2, typename T3>  
void sample_function(const T0& x0, const T1& x1, const T2& x2, const T3& x3)
{
    std::cout << "\nsample function = overrload 3";
}

void test_template_function_specialization()
{
    // invoke 0,0,1,2,0 respectively
    sample_function(std::uint32_t{1});
    sample_function(std::uint32_t{1}, std::uint32_t{1});
    sample_function(std::uint32_t{1}, std::uint32_t{1}, std::uint32_t{1});
    sample_function(std::uint32_t{1}, std::uint32_t{1}, std::uint32_t{1}, std::uint32_t{1});
    sample_function(std::uint32_t{1}, std::uint32_t{1}, std::uint32_t{1}, std::uint32_t{1}, std::uint32_t{1});
    sample_function(std::make_tuple(1));       // unfornately, 0 is invoked
    sample_function(std::make_tuple(1,2));     // unfornately, 0 is invoked
    sample_function(std::make_tuple(1,2,3));   // unfornately, 0 is invoked
}

void test_template()
{
    test_template_class_specialization();
    test_template_function_specialization();
}
