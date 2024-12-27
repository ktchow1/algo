#include<cassert>
#include<vector>
#include<list>
#include<map>
#include<unordered_map>
#include<template.h>
#include<traits.h>


void test_template()
{
    // function template
    std::cout << "\nfunction template";
    alg::function_template(int{}, int{}); 
    alg::function_template(int{}, std::string{}); 
    alg::function_template(std::string{}, int{}); 
    alg::function_template(std::string{}, int{}); 

    // class template 
    std::cout << "\n\nclass template";
    alg::class_template<int,int,int> x0;
    alg::class_template<int,std::string,int> x1;
    alg::class_template<int,std::string,std::string> x2;
    alg::class_template<std::string,std::string,std::string> x3;
    x0.fct();
    x1.fct();
    x2.fct();
    x3.fct();

    // class template with CTAD 
    std::cout << "\n\nclass template with CTAD";
    alg::class_template y0(int{},int{},int{});
    alg::class_template y1(int{},std::string{},int{});
    alg::class_template y2(int{},std::string{},std::string{});
    alg::class_template y3(std::string{},std::string{},std::string{});
    y0.fct();
    y1.fct();
    y2.fct();
    y3.fct();
    static_assert(alg::is_same_v<decltype(y0)::type0, int>,         "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y0)::type1, int>,         "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y0)::type2, int>,         "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y1)::type0, int>,         "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y1)::type1, std::string>, "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y1)::type2, int>,         "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y2)::type0, int>,         "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y2)::type1, std::string>, "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y2)::type2, std::string>, "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y3)::type0, std::string>, "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y3)::type1, std::string>, "failed to use class template with CTAD");
    static_assert(alg::is_same_v<decltype(y3)::type2, std::string>, "failed to use class template with CTAD");

    // class template with deduction guide 
    std::cout << "\n\nclass template with deduction guide";
    alg::class_template z0(int{},int{});                            // general template
    alg::class_template z1(int{},std::string{});                    // general template
    alg::class_template z2(int{});                                  // specialization
    alg::class_template z3(std::string{});                          // specialization
    z0.fct();
    z1.fct();
    z2.fct();
    z3.fct();
    static_assert(alg::is_same_v<decltype(z0)::type0, int>,         "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z0)::type1, int>,         "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z0)::type2, int>,         "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z1)::type0, int>,         "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z1)::type1, std::string>, "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z1)::type2, int>,         "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z2)::type0, int>,         "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z2)::type1, std::string>, "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z2)::type2, std::string>, "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z3)::type0, std::string>, "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z3)::type1, std::string>, "failed to use class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z3)::type2, std::string>, "failed to use class template with deduction guide");

    // *** class member template *** //
    std::cout << "\n\nclass member template";
    alg::class_member_template<int> w0;
    alg::class_member_template<std::string> w1;
    w0.fct<int>();
    w0.fct<std::string>();
    w1.fct<int>();
    w1.fct<std::string>();

    // *** variable template *** //
    std::cout << "\n\nvariable template";
    std::cout << "\nvariable template : std::uint8_t                       = " << alg::variable_template<std::uint8_t>;
    std::cout << "\nvariable template : std::uint32_t                      = " << alg::variable_template<std::uint16_t>;
    std::cout << "\nvariable template : std::uint16_t                      = " << alg::variable_template<std::uint32_t>;
    std::cout << "\nvariable template : std::uint64_t                      = " << alg::variable_template<std::uint64_t>;
    std::cout << "\nvariable template : int                                = " << alg::variable_template<int>;
    std::cout << "\nvariable template : std::string                        = " << alg::variable_template<std::string>;
    std::cout << "\nvariable template : std::pair<int,int>                 = " << alg::variable_template<std::pair<int,int>>;
    std::cout << "\nvariable template : std::pair<int,std::string>         = " << alg::variable_template<std::pair<int,std::string>>;
    std::cout << "\nvariable template : std::pair<std::string,int>         = " << alg::variable_template<std::pair<std::string,int>>;
    std::cout << "\nvariable template : std::pair<std::string,std::string> = " << alg::variable_template<std::pair<std::string,std::string>>;

    // *** alias template *** //
    static_assert(alg::is_same_v<alg::alias_template<std::vector<int>>,         int>,         "failed to use alias template");
    static_assert(alg::is_same_v<alg::alias_template<std::vector<std::string>>, std::string>, "failed to use alias template");
    static_assert(alg::is_same_v<alg::alias_template<std::list<int>>,           int>,         "failed to use alias template");
    static_assert(alg::is_same_v<alg::alias_template<std::list<std::string>>,   std::string>, "failed to use alias template");

    // *** template template *** //
    static_assert(alg::is_same_v<alg::template_template<std::map,int,int>::type,                   std::map<int,int>>,                   "failed to use template template");
    static_assert(alg::is_same_v<alg::template_template<std::map,int,std::string>::type,           std::map<int,std::string>>,           "failed to use template template");
    static_assert(alg::is_same_v<alg::template_template<std::unordered_map,int,int>::type,         std::unordered_map<int,int>>,         "failed to use template template");
    static_assert(alg::is_same_v<alg::template_template<std::unordered_map,int,std::string>::type, std::unordered_map<int,std::string>>, "failed to use template template");
}


