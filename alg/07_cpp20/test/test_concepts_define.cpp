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
// 7. other concepts
//
//
//
//
//
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
        typename T::value;
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
    };

    template<typename T>
    concept concept_method7 = 
    concept_method1<T> &&
    requires(T x)
    {
        { x.fct() } -> std::same_as<std::string>;
    };
}

void test_concepts_define()
{
}

void test_concepts_define_examples()
{
}
