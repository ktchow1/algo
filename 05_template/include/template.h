#pragma once
#include<iostream>

// ******************************************************************************************* //
// Template basic                                                                              //
// * function template       with auto deduction                                               //
// * class template          with class-template-argument-deduction (CTAD) and deduction guide //
// * class member template                                                                     //
// * variable template                                                                         //
// * alias template                                                                            //
// * template template                                                                         //
// ******************************************************************************************* //
namespace alg 
{
    // ************************* //
    // *** function template *** //
    // ************************* //
    template<typename T, typename U>
    void function_template(const T& t, const U& u)
    {
        std::cout << "\nfunction template :"
                  << " T=" << typeid(t).name() 
                  << " U=" << typeid(u).name();
    }

    template<typename U>
    void function_template(const std::string& str, const U& u)
    {
        std::cout << "\nfunction template specialization :"
                  << " T=std::string"
                  << " U=" << typeid(u).name();
    }


    // ********************** //
    // *** class template *** //
    // ********************** //
    // There are 6 cases :
    // class template with type explicit specified         = generalization + specialization
    // class template with type deduced by CTAD            = generalization + specialization
    // class template with type deduced by deduction guide = generalization + specialization
    
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
        std::cout << "\nclass template :"
                  << " T=" << typeid(m_t).name() 
                  << " U=" << typeid(m_u).name()
                  << " V=" << typeid(m_v).name();
    }

    // *** specialization *** //
    template<typename T, typename U>
    struct class_template<T,U,std::string>
    {
        using type0 = T;
        using type1 = U;
        using type2 = std::string;

        class_template() = default;

        class_template(const T& t) : m_t(t), m_u(), m_str("deduction guide B to specialization") // support deduction guide B
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
        std::cout << "\nclass template specialization :"
                  << " T=" << typeid(m_t).name()
                  << " U=" << typeid(m_u).name()
                  << " V=std::string";
    }

    // *** deduction guide *** //
    template<typename T, typename U>
    class_template(const T&, const U&) -> class_template<T,U,int>; // <--- deduction guide A
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
    void class_member_template<T>::template fct() const noexcept // BUG : cannot compile without template
    {
        T t; U u;
        std::cout << "\nclass member template :"
                  << " T=" << typeid(t).name() 
                  << " U=" << typeid(u).name();
    }

    template<>
    struct class_member_template<std::string>
    {
        template<typename U>
        void fct() const noexcept;
    };

//  template<> // BUG : cannot compile with template
    template<typename U>
    void class_member_template<std::string>::template fct() const noexcept 
    {
        U u;
        std::cout << "\nclass member template specialization :"
                  << " T=std::string"
                  << " U=" << typeid(u).name();
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
}


