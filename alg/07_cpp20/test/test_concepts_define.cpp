#include<iostream>
#include<cassert>
#include<cstdint>
#include<vector>
#include<concepts>

// *** alg *** //
#include<utility.h>




// ***************************** //
// *** Defining alg concepts *** //
// ***************************** //
// Concept is related to traits. The former is usually built on top of the latter.
//
// Naming in STL : 
// * std::is_xxx_able  <T> = traits
// * std::is_xxx_able_v<T> = std::is_xxx_able<T>::value
// * std::is_xxx_able_t<T> = std::is_xxx_able<T>::type
// * std::   xxx_able  <T> = concept based on std::is_xxx_able<T>
//
//
// Concept is defined as conjunction or disjunction of constraints. There are 7 ways to define constraints : 
// 1. value traits in class template  
// 2. value traits in variable template  
// 3. requires expression -     type requirement
// 4. requires expression -   simple requirement
// 5. requires expression - compound requirement (with brace {})
// 6. requires expression -   nested requirement
// 7. other concepts                             <--- this is same as 6? just put concept in different places
//


// ************** //
// *** Define *** //
// ************** //
namespace test
{
    struct typeK       { using type = std::uint32_t;   std::uint32_t foo() { return 123;   } };
    struct typeL       { using type = std::uint32_t;   std::uint32_t fct() { return 123;   } };
    struct typeM       { using type = std::uint32_t;   std::string   fct() { return "ABC"; } };
    struct typeN       {                               std::string   fct() { return "ABC"; } };
  
    template<typename T> struct is_KLM        { static constexpr bool value = false;  };
    template<>           struct is_KLM<typeK> { static constexpr bool value =  true;  };
    template<>           struct is_KLM<typeL> { static constexpr bool value =  true;  };
    template<>           struct is_KLM<typeM> { static constexpr bool value =  true;  };

    template<typename T> constexpr bool is_LMN        = false;
    template<>           constexpr bool is_LMN<typeL> =  true;
    template<>           constexpr bool is_LMN<typeM> =  true;
    template<>           constexpr bool is_LMN<typeN> =  true;

    template<typename T>
    concept concept_method1 = is_KLM<T>::value;

    template<typename T>
    concept concept_method2 = is_LMN<T>;

    template<typename T>
    concept concept_method3 = 
    requires
    {
        typename T::type;
        // <--- can be multi lines
    };

    template<typename T>
    concept concept_method4 = 
    requires(T x) // <--- can be multi arguments
    {
        x.fct();
    };

    template<typename T>
    concept concept_method5 = 
    requires(T x)
    {
        { x.fct() } -> std::same_as<std::string>;
    //  { x.fct() } ->              std::string;  // BUG : compile error
    };

    template<typename T>
    concept concept_method6 = 
    requires(T x)
    {
        { x.fct() } -> std::same_as<std::string>;
        requires concept_method1<T>;
        requires concept_method2<T>;
    };

    template<typename T>
    concept concept_method7 = 
    concept_method1<T> &&
    concept_method2<T> &&
    requires(T x)
    {
        { x.fct() } -> std::same_as<std::string>;
    };
}


// ************* //
// *** Apply *** //
// ************* //
namespace test
{
    template<typename T, typename U> requires concept_method1<T>
    void tester_method1(const T&, const U&) {}

    template<typename T, typename U> requires concept_method2<T>
    void tester_method2(const T&, const U&) {}

    template<typename T, typename U> requires concept_method3<T>
    void tester_method3(const T&, const U&) {}

    template<typename T, typename U> requires concept_method4<T>
    void tester_method4(const T&, const U&) {}

    template<typename T, typename U> requires concept_method5<T>
    void tester_method5(const T&, const U&) {}

    template<typename T, typename U> requires concept_method6<T>
    void tester_method6(const T&, const U&) {}
     
    template<typename T, typename U> requires concept_method7<T>
    void tester_method7(const T&, const U&) {}
}

void test_concepts_define()
{
//  test::tester_method1(std::string{"x"}, std::string{"abc"});
    test::tester_method1(test::typeK{},    std::string{"abc"});
    test::tester_method1(test::typeL{},    std::string{"abc"});
    test::tester_method1(test::typeM{},    std::string{"abc"});
//  test::tester_method1(test::typeN{},    std::string{"abc"});
  
//  test::tester_method2(std::string{"x"}, std::string{"abc"});
//  test::tester_method2(test::typeK{},    std::string{"abc"});
    test::tester_method2(test::typeL{},    std::string{"abc"});
    test::tester_method2(test::typeM{},    std::string{"abc"});
    test::tester_method2(test::typeN{},    std::string{"abc"});

//  test::tester_method3(std::string{"x"}, std::string{"abc"});
    test::tester_method3(test::typeK{},    std::string{"abc"});
    test::tester_method3(test::typeL{},    std::string{"abc"});
    test::tester_method3(test::typeM{},    std::string{"abc"});
//  test::tester_method3(test::typeN{},    std::string{"abc"});

//  test::tester_method4(std::string{"x"}, std::string{"abc"});
//  test::tester_method4(test::typeK{},    std::string{"abc"});
    test::tester_method4(test::typeL{},    std::string{"abc"});
    test::tester_method4(test::typeM{},    std::string{"abc"});
    test::tester_method4(test::typeN{},    std::string{"abc"});

//  test::tester_method5(std::string{"x"}, std::string{"abc"});
//  test::tester_method5(test::typeK{},    std::string{"abc"});
//  test::tester_method5(test::typeL{},    std::string{"abc"});
    test::tester_method5(test::typeM{},    std::string{"abc"});
    test::tester_method5(test::typeN{},    std::string{"abc"});
  
//  test::tester_method6(std::string{"x"}, std::string{"abc"});
//  test::tester_method6(test::typeK{},    std::string{"abc"});
//  test::tester_method6(test::typeL{},    std::string{"abc"});
    test::tester_method6(test::typeM{},    std::string{"abc"});
//  test::tester_method6(test::typeN{},    std::string{"abc"});

//  test::tester_method7(std::string{"x"}, std::string{"abc"});
//  test::tester_method7(test::typeK{},    std::string{"abc"});
//  test::tester_method7(test::typeL{},    std::string{"abc"});
    test::tester_method7(test::typeM{},    std::string{"abc"});
//  test::tester_method7(test::typeN{},    std::string{"abc"});
  
    print_summary("coocepts - define concept", "succeeded");
}

void test_concepts_define_examples()
{
    print_summary("coocepts - define concept (examples)", "succeeded");
}
