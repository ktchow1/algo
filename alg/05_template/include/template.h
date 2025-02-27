#pragma once
#include<iostream>
#include<cstdint>


// ******************************************************************************************* //
// Template basic                                                                              //
//                                                                                             //
// *     function template   with auto deduction                                               //
// *        class template   with class-template-argument-deduction (CTAD) and deduction guide //
// * class member template                                                                     //
// *     variable template                                                                     //
// *        alias template                                                                     //
// *     template template                                                                     //
// ******************************************************************************************* //
namespace alg 
{
    namespace global
    {
        std::uint32_t overload = 0;
    }

    // ************************* //
    // *** function template *** //
    // ************************* //
    template<typename T, typename U>
    void function_template(const T& t, const U& u)
    {
        global::overload = 1;
    }

    template<typename U>
    void function_template(const std::string& str, const U& u)
    {
        global::overload = 2;
    }


    // ************************************* //
    // *** Abbreviated function template *** //
    // ************************************* //
    void abbre_function_template(const auto& x, auto&& y) // <--- support universal reference
    {
        auto temp = std::forward<decltype(y)>(y); // <--- how universal reference can be forwarded
    }


    // ********************** //
    // *** class template *** //
    // ********************** //
    // There are 6 cases :
    // class template with template type explicit specified         = generalization + specialization
    // class template with template type deduced by CTAD            = generalization + specialization
    // class template with template type deduced by deduction guide = generalization + specialization
    // 
    // CTAD            = compiler deduces all template types in constructor
    // deduction guide = compiler deduces all template types given hint 
    
    template<typename T, typename U, typename V>
    struct class_template
    {
        using type0 = T;
        using type1 = U;
        using type2 = V;

        class_template() = default;

        class_template(const T& t, const U& u) : m_t(t), m_u(u) // support deduction guide A
        {
        }
        class_template(const T& t, const U& u, const V& v) : m_t(t), m_u(u), m_v(v) // support CTAD
        {
        }
        void fct() const noexcept;
    
    private:
        T m_t; 
        U m_u;
        V m_v;
    };

    template<typename T, typename U, typename V>
    void class_template<T,U,V>::fct() const noexcept
    {
        global::overload = 3;
    }

    // *** specialization *** //
    template<typename T, typename U>
    struct class_template<T,U,std::string>
    {
        using type0 = T;
        using type1 = U;
        using type2 = std::string;

        class_template() = default;

        class_template(const T& t) : m_t(t), m_u(), m_str("xxx") // support deduction guide B
        {
        }
        class_template(const T& t, const U& u, const std::string& str) : m_t(t), m_u(u), m_str(str) // support CTAD
        {
        }
        void fct() const noexcept;

    private:
        T m_t;
        U m_u;
        std::string m_str; 
    };

    template<typename T, typename U>
    void class_template<T,U,std::string>::fct() const noexcept
    {
        global::overload = 4;
    }

    // *** deduction guide *** //
    template<typename T, typename U>
    class_template(const T&, const U&) -> class_template<T,U,int>;         // <--- deduction guide A
    template<typename T>
    class_template(const T&) -> class_template<T,std::string,std::string>; // <--- deduction guide B


    // ***************************** //
    // *** class member template *** //
    // ***************************** //
    template<typename T>
    struct class_member_template
    {
        template<typename U>
        void fct() const noexcept;
    };

    template<typename T>
    template<typename U>
    void class_member_template<T>::template fct() const noexcept // BUG : cannot compile without keyword "template"
    {
        global::overload = 5;
    }

    template<>
    struct class_member_template<std::string>
    {
        template<typename U>
        void fct() const noexcept;
    };

//  template<> // BUG : cannot compile with keyword "template"
    template<typename U>
    void class_member_template<std::string>::template fct() const noexcept 
    {
        global::overload = 6;
    }

    template<typename T>
    void class_member_template_caller(const class_member_template<T>& x)
    {
        x.template fct<std::string>(); // BUG : cannot compile without keyword "template" 
                                       // (let compiler knows template member inside template fct/class)
    }


    // ************************* //
    // *** variable template *** //
    // ************************* //
    template<typename T> constexpr int variable_template = 0;
    template<>           constexpr int variable_template<int> = 1;
    template<>           constexpr int variable_template<std::string> = 2;
    template<>           constexpr int variable_template<std::pair<int,int>> = 3;
    template<>           constexpr int variable_template<std::pair<int,std::string>> = 4;
    template<>           constexpr int variable_template<std::pair<std::string,int>> = 5;
    template<>           constexpr int variable_template<std::pair<std::string,std::string>> = 6;

    // Another usage
    template<typename T> std::vector<T> variable_template2 = {1,2,3,4,5};


    // ********************** //
    // *** alias template *** //
    // ********************** //
    // Does not allow specialization
    template<typename C> using alias_template = typename C::value_type;


    // ************************* // 
    // *** template template *** //
    // ************************* // 
    template<template<typename,typename> typename MAP, typename K, typename V>
    struct template_template
    {
         using type = MAP<K,V>;
    };


    // ******************************************** //
    // *** member pointer as template parameter *** //
    // ******************************************** //
    void hard_code(std::uint32_t& x)  { x =  123;  }
    void hard_code(std::string&   x)  { x = "123"; }

    struct A_with_members
    {
        std::uint32_t m_x;
        std::uint32_t m_y;
        std::string   m_z;
        std::string   m_w;
    };

    // method 1
    template<typename T> 
    void hard_code_method1(A_with_members& a, T mem_ptr)
    {
        hard_code(a.*mem_ptr);
    }

    // method 2
    template<typename T> 
    void hard_code_method2(A_with_members& a, T A_with_members::* mem_ptr)
    {
        hard_code(a.*mem_ptr);
    }

    // template parameter | type             | value
    // -------------------+------------------+---------------
    //          T         | std::string      | fct(s)         <--- for general template usuage
    //          T         | std::string A::* | fct(&A::m_x)   <--- for template member pointer
    //                                              ^
}


