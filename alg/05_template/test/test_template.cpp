#include<cassert>
#include<vector>
#include<list>
#include<map>
#include<unordered_map>
#include<template.h>
#include<traits.h>
#include<utility.h>


void test_template()
{
    struct A{};
    struct B{};
    struct C{};
    struct D{};


    // function template
    alg::function_template(int{}, int{}); 
    assert(alg::global::overload == 1);
    alg::function_template(int{}, A{}); 
    assert(alg::global::overload == 1);
    alg::function_template(std::string{}, int{}); 
    assert(alg::global::overload == 2);
    alg::function_template(std::string{}, A{}); 
    assert(alg::global::overload == 2);
    print_summary("function template", "succeeded");


    // class template 
    alg::class_template<A,B,int> x0;
    alg::class_template<A,B,C> x1;
    alg::class_template<A,B,std::string> x2;
    alg::class_template<A,C,std::string> x3;
    x0.fct();
    assert(alg::global::overload == 3);
    x1.fct();
    assert(alg::global::overload == 3);
    x2.fct();
    assert(alg::global::overload == 4);
    x3.fct();
    assert(alg::global::overload == 4);
    print_summary("   class template", "succeeded");


    // class template with CTAD 
    alg::class_template y0(A{},B{},int{});
    alg::class_template y1(A{},B{},C{});
    alg::class_template y2(A{},B{},std::string{});
    alg::class_template y3(A{},C{},std::string{});
    y0.fct();
    assert(alg::global::overload == 3);
    y1.fct();
    assert(alg::global::overload == 3);
    y2.fct();
    assert(alg::global::overload == 4);
    y3.fct();
    assert(alg::global::overload == 4);
    static_assert(alg::is_same_v<decltype(y0)::type0, A>,           "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y0)::type1, B>,           "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y0)::type2, int>,         "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y1)::type0, A>,           "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y1)::type1, B>,           "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y1)::type2, C>,           "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y2)::type0, A>,           "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y2)::type1, B>,           "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y2)::type2, std::string>, "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y3)::type0, A>,           "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y3)::type1, C>,           "failed class template with CTAD");
    static_assert(alg::is_same_v<decltype(y3)::type2, std::string>, "failed class template with CTAD");
    print_summary("   class template with CTAD", "succeeded");


    // class template with deduction guide 
    alg::class_template z0(A{},B{});                                // general template
    alg::class_template z1(A{},C{});                                // general template
    alg::class_template z2(A{});                                    // specialization
    alg::class_template z3(B{});                                    // specialization
    z0.fct();
    assert(alg::global::overload == 3);
    z1.fct();
    assert(alg::global::overload == 3);
    z2.fct();
    assert(alg::global::overload == 4);
    z3.fct();
    assert(alg::global::overload == 4);
    static_assert(alg::is_same_v<decltype(z0)::type0, A>,           "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z0)::type1, B>,           "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z0)::type2, int>,         "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z1)::type0, A>,           "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z1)::type1, C>,           "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z1)::type2, int>,         "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z2)::type0, A>,           "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z2)::type1, std::string>, "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z2)::type2, std::string>, "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z3)::type0, B>,           "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z3)::type1, std::string>, "failed class template with deduction guide");
    static_assert(alg::is_same_v<decltype(z3)::type2, std::string>, "failed class template with deduction guide");
    print_summary("   class template with deduction guide", "succeeded");


    // *** class member template *** //
    alg::class_member_template<int> w0;
    alg::class_member_template<std::string> w1;

    w0.fct<A>();
    assert(alg::global::overload == 5);
    w0.fct<B>();
    assert(alg::global::overload == 5);
    w1.fct<C>();
    assert(alg::global::overload == 6);
    w1.fct<D>();
    assert(alg::global::overload == 6);

    alg::class_member_template_caller(w0);
    assert(alg::global::overload == 5);
    alg::class_member_template_caller(w1);
    assert(alg::global::overload == 6);
    print_summary("   class member template", "succeeded");


    // *** variable template *** //
    assert( alg::variable_template<std::uint8_t>  == 0);
    assert( alg::variable_template<std::uint16_t> == 0);
    assert( alg::variable_template<std::uint32_t> == 0);
    assert( alg::variable_template<std::uint64_t> == 0);
    assert( alg::variable_template<int>           == 1);
    assert( alg::variable_template<std::string>   == 2);
    assert((alg::variable_template<std::pair<int,int>>                 == 3));
    assert((alg::variable_template<std::pair<int,std::string>>         == 4));
    assert((alg::variable_template<std::pair<std::string,int>>         == 5));
    assert((alg::variable_template<std::pair<std::string,std::string>> == 6)); // <--- extra bracket to compile assert

    assert(alg::variable_template2<std::uint16_t>.size() == 5);
    assert(alg::variable_template2<std::uint32_t>.size() == 5);
    assert(alg::variable_template2<std::uint64_t>.size() == 5);
    assert(alg::variable_template2<double>.size() == 5);
    assert(alg::variable_template2<std::uint16_t>[0] == 1);
    assert(alg::variable_template2<std::uint16_t>[1] == 2);
    assert(alg::variable_template2<std::uint16_t>[2] == 3);
    assert(alg::variable_template2<std::uint16_t>[3] == 4);
    assert(alg::variable_template2<std::uint16_t>[4] == 5);
    assert(std::abs(alg::variable_template2<double>[0] - 1.0) < 0.0001);
    assert(std::abs(alg::variable_template2<double>[1] - 2.0) < 0.0001);
    assert(std::abs(alg::variable_template2<double>[2] - 3.0) < 0.0001);
    assert(std::abs(alg::variable_template2<double>[3] - 4.0) < 0.0001);
    assert(std::abs(alg::variable_template2<double>[4] - 5.0) < 0.0001);
    print_summary("variable template", "succeeded");


    // *** alias template *** //
    static_assert(alg::is_same_v<alg::alias_template<std::vector<int>>,         int>,         "failed to use alias template");
    static_assert(alg::is_same_v<alg::alias_template<std::vector<std::string>>, std::string>, "failed to use alias template");
    static_assert(alg::is_same_v<alg::alias_template<std::list<int>>,           int>,         "failed to use alias template");
    static_assert(alg::is_same_v<alg::alias_template<std::list<std::string>>,   std::string>, "failed to use alias template");
    print_summary("   alias template", "succeeded in compile time");


    // *** template template *** //
    static_assert(alg::is_same_v<alg::template_template<std::map,int,int>::type,                   std::map<int,int>>,                   "failed to use template template");
    static_assert(alg::is_same_v<alg::template_template<std::map,int,std::string>::type,           std::map<int,std::string>>,           "failed to use template template");
    static_assert(alg::is_same_v<alg::template_template<std::unordered_map,int,int>::type,         std::unordered_map<int,int>>,         "failed to use template template");
    static_assert(alg::is_same_v<alg::template_template<std::unordered_map,int,std::string>::type, std::unordered_map<int,std::string>>, "failed to use template template");
    print_summary("template template", "succeeded in compile time");


    // *** member pointer as template parameter - method 1 *** //
    alg::A_with_members a1{0,0,"",""};
    assert(a1.m_x == 0);
    assert(a1.m_y == 0);
    assert(a1.m_z == std::string{});
    assert(a1.m_w == std::string{});

    alg::hard_code_method1(a1, &alg::A_with_members::m_x);
    assert(a1.m_x == 123);
    assert(a1.m_y == 0);
    assert(a1.m_z == std::string{});
    assert(a1.m_w == std::string{});

    alg::hard_code_method1(a1, &alg::A_with_members::m_y);
    assert(a1.m_x == 123);
    assert(a1.m_y == 123);
    assert(a1.m_z == std::string{});
    assert(a1.m_w == std::string{});

    alg::hard_code_method1(a1, &alg::A_with_members::m_z);
    assert(a1.m_x == 123);
    assert(a1.m_y == 123);
    assert(a1.m_z == std::string{"123"});
    assert(a1.m_w == std::string{});

    alg::hard_code_method1(a1, &alg::A_with_members::m_w);
    assert(a1.m_x == 123);
    assert(a1.m_y == 123);
    assert(a1.m_z == std::string{"123"});
    assert(a1.m_w == std::string{"123"});

    // *** member pointer as template parameter - method 2 *** // (both work)
    alg::A_with_members a2{0,0,"",""};
    assert(a2.m_x == 0);
    assert(a2.m_y == 0);
    assert(a2.m_z == std::string{});
    assert(a2.m_w == std::string{});

    alg::hard_code_method1(a2, &alg::A_with_members::m_x);
    assert(a2.m_x == 123);
    assert(a2.m_y == 0);
    assert(a2.m_z == std::string{});
    assert(a2.m_w == std::string{});

    alg::hard_code_method1(a2, &alg::A_with_members::m_y);
    assert(a2.m_x == 123);
    assert(a2.m_y == 123);
    assert(a2.m_z == std::string{});
    assert(a2.m_w == std::string{});

    alg::hard_code_method1(a2, &alg::A_with_members::m_z);
    assert(a2.m_x == 123);
    assert(a2.m_y == 123);
    assert(a2.m_z == std::string{"123"});
    assert(a2.m_w == std::string{});

    alg::hard_code_method1(a2, &alg::A_with_members::m_w);
    assert(a2.m_x == 123);
    assert(a2.m_y == 123);
    assert(a2.m_z == std::string{"123"});
    assert(a2.m_w == std::string{"123"});
    print_summary("  member pointer as template parameter", "succeeded in compile time");
}


