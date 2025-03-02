#include<iostream>
#include<cassert>
#include<cstdint>
#include<string>
#include<concepts>

// *** alg *** //
#include<utility.h>




// ***************************** //
// *** Applying std concepts *** //
// ***************************** //
// There are 3 ways to apply concept :
// 1. as requires-clause after template parameter   (supports conjunction / disjunction of multiple concepts)
// 2. as requires-clause after function prototype   (supports conjunction / disjunction of multiple concepts)
// 3. as requires-clause in template parameter list (does not support conjunction / disjunction) <--- 1st template parameter is the one on which constraint applied
// 4. as requires-requires pattern, which is a requires-clause plus a requires-expression
//
// 
// requires-clause      : apply concept to template class or template function
// requires-expresssion : define a new concept
//
//
// Format of a requires-clause : 
// requires std::xxx<T>          <--- where xxx is a concept
// requires std::xxx<T,U,...>    <--- where xxx is a concept
//    
namespace test
{
    // ********************************************** //
    // *** std concepts with 1 template parameter *** //
    // ********************************************** //
    struct type_A { type_A() = delete;     type_A(std::uint32_t){}                                         };
    struct type_B { type_B(){}             type_B(const type_B&) = delete;     type_B(type_B&&){}          };
    struct type_C { type_C(){}             type_C(const type_C&){}             type_C(type_C&&) = delete;  };

    template<typename T, typename U> 
    void tester_no_constraint(const T&, const U&) {}

    template<typename T, typename U> requires std::default_initializable<T>
    void tester_no_A_method1(const T&, const U&) {}
    template<typename T, typename U> requires std::copy_constructible<T>
    void tester_no_B_method1(const T&, const U&) {}
    template<typename T, typename U> requires std::move_constructible<T>
    void tester_no_C_method1(const T&, const U&) {}

    template<typename T, typename U> 
    void tester_no_A_method2(const T&, const U&) requires std::default_initializable<T> {}
    template<typename T, typename U> 
    void tester_no_B_method2(const T&, const U&) requires std::copy_constructible<T> {}
    template<typename T, typename U> 
    void tester_no_C_method2(const T&, const U&) requires std::move_constructible<T> {}

    template<std::default_initializable T, typename U> 
    void tester_no_A_method3(const T&, const U&) {}
    template<std::copy_constructible T, typename U> 
    void tester_no_B_method3(const T&, const U&) {}
    template<std::move_constructible T, typename U> 
    void tester_no_C_method3(const T&, const U&) {}


    // *********************************************** //
    // *** std concepts with 2 template parameters *** //
    // *********************************************** //
    // The order of template parameter in std::is_base_of<B,D> is different from that in std::derived_from<D,B>
      
    struct type_D : public type_B {};

    template<typename T, typename U> requires std::derived_from<T, type_B>
    void tester_derived_from_B_method1(const T&, const U&) {}
    template<typename T, typename U> requires std::derived_from<T, U>
    void tester_derived_from_U_method1(const T&, const U&) {}






    // *********************************************** //
    // *** std concepts with 2 template parameters *** //
    // *********************************************** //
    template<typename T, typename U, typename V>
    void tester_no_constraint(const T&, const U&, const V&) {}

    // ************************************* //
    // *** simple definition of concepts *** //
    // ************************************* //


    // ************************* //
    // *** requires-requires *** //
    // ************************* //

}


void test_concepts_apply_constraint_on_1_para()
{
    // Comment lines cannot compile due to concept
    test::tester_no_constraint(test::type_A{123}, std::string{"abc"});
    test::tester_no_constraint(test::type_B{},    std::string{"abc"});
    test::tester_no_constraint(test::type_C{},    std::string{"abc"});

//  test::tester_no_A_method1(test::type_A{123}, std::string{"abc"});
    test::tester_no_A_method1(test::type_B{},    std::string{"abc"});
    test::tester_no_A_method1(test::type_C{},    std::string{"abc"});
    test::tester_no_B_method1(test::type_A{123}, std::string{"abc"});
//  test::tester_no_B_method1(test::type_B{},    std::string{"abc"});
//  test::tester_no_B_method1(test::type_C{},    std::string{"abc"}); // std::copy_constructible = requires copy_constructible AND move_constructible
    test::tester_no_C_method1(test::type_A{123}, std::string{"abc"});
    test::tester_no_C_method1(test::type_B{},    std::string{"abc"}); // std::move_constructible = requires move_constructible only
//  test::tester_no_C_method1(test::type_C{},    std::string{"abc"});

//  test::tester_no_A_method2(test::type_A{123}, std::string{"abc"});
    test::tester_no_A_method2(test::type_B{},    std::string{"abc"});
    test::tester_no_A_method2(test::type_C{},    std::string{"abc"});
    test::tester_no_B_method2(test::type_A{123}, std::string{"abc"});
//  test::tester_no_B_method2(test::type_B{},    std::string{"abc"});
//  test::tester_no_B_method2(test::type_C{},    std::string{"abc"}); // std::copy_constructible = requires copy_constructible AND move_constructible
    test::tester_no_C_method2(test::type_A{123}, std::string{"abc"});
    test::tester_no_C_method2(test::type_B{},    std::string{"abc"}); // std::move_constructible = requires move_constructible only
//  test::tester_no_C_method2(test::type_C{},    std::string{"abc"});
  
//  test::tester_no_A_method3(test::type_A{123}, std::string{"abc"});
    test::tester_no_A_method3(test::type_B{},    std::string{"abc"});
    test::tester_no_A_method3(test::type_C{},    std::string{"abc"});
    test::tester_no_B_method3(test::type_A{123}, std::string{"abc"});
//  test::tester_no_B_method3(test::type_B{},    std::string{"abc"});
//  test::tester_no_B_method3(test::type_C{},    std::string{"abc"}); // std::copy_constructible = requires copy_constructible AND move_constructible
    test::tester_no_C_method3(test::type_A{123}, std::string{"abc"});
    test::tester_no_C_method3(test::type_B{},    std::string{"abc"}); // std::move_constructible = requires move_constructible only
//  test::tester_no_C_method3(test::type_C{},    std::string{"abc"});
  
    print_summary("coocepts - apply constraint on 1 para", "succeeded");
}


void test_concepts_apply_constraint_on_2_para()
{

    print_summary("coocepts - apply constraint on 2 para", "succeeded");
}


void test_concepts_apply_constraint_on_3_para()
{
    print_summary("coocepts - apply constraint on 3 para", "succeeded");
}


void test_concepts_requires_requires()
{
    print_summary("coocepts - requires requires", "succeeded");
}


void test_concepts_with_abbv_function_template()
{
    print_summary("coocepts - with abbv function template", "succeeded");
}


void test_concepts_apply()
{
    test_concepts_apply_constraint_on_1_para();
    test_concepts_apply_constraint_on_2_para();
    test_concepts_apply_constraint_on_3_para();
    test_concepts_requires_requires();
    test_concepts_with_abbv_function_template();
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
