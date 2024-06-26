#pragma once
#include<iostream>
#include<functional>
#include<variant>
#include<tuple>
#include<string>

// *************************************************************************************************** //
// Variant vs tuple
//
// std::variant<T0,T1,T2> =     sum type, i.e. feasible set = set of T0 + set of T1 + set of T2 + ...
// std::tuple<T0,T1,T2>   = product type, i.e. feasible set = set of T0 * set of T1 * set of T2 * ...
// 
// std::variant works with std::visit (and overloading lambda)
// std::tuple works with std::apply
// *************************************************************************************************** //
namespace alg
{
    void variant_vs_tuple()
    {
        bool b = true;
        char c = 'c';
        int  i = 12345;
        std::string s = "str";

        std::variant<bool,char,int,std::string> v0 = b;
        std::variant<bool,char,int,std::string> v1 = c;
        std::variant<bool,char,int,std::string> v2 = i;
        std::variant<bool,char,int,std::string> v3 = s;

        std::tuple<bool,char,int,std::string> t0{b,c,i,s};
        auto t1 = std::make_tuple(b,c,i,s);


        // ********************** //
        // *** variant access *** //
        // ********************** //
        auto variant_access_method0 = [](const auto& v, const std::string& name)
        {
            try
            {
                if      (v.index()==0) std::cout << "\nvariant " << name << " is bool, value = " << std::get<0>(v);
                else if (v.index()==1) std::cout << "\nvariant " << name << " is char, value = " << std::get<1>(v);
                else if (v.index()==2) std::cout << "\nvariant " << name << " is int,  value = " << std::get<2>(v);
                else if (v.index()==3) std::cout << "\nvariant " << name << " is str,  value = " << std::get<3>(v);
                else                   std::cout << "\nvariant " << name << " is unknown type";
            }
            catch(const std::bad_variant_access& ex) { std::cout << "\nbad variant access for " << name; }
        };

        auto variant_access_method1 = [](const auto& v, const std::string& name)
        {
            try
            {
                if      (std::holds_alternative<bool>(v))        std::cout << "\nvariant " << name << " is bool, value = " << std::get<bool>(v);
                else if (std::holds_alternative<char>(v))        std::cout << "\nvariant " << name << " is char, value = " << std::get<char>(v);
                else if (std::holds_alternative<int>(v))         std::cout << "\nvariant " << name << " is int,  value = " << std::get<int>(v);
                else if (std::holds_alternative<std::string>(v)) std::cout << "\nvariant " << name << " is str,  value = " << std::get<std::string>(v);
                else                                             std::cout << "\nvariant " << name << " is unknown type";
            }
            catch(const std::bad_variant_access& ex) { std::cout << "\nbad variant access for " << name; }
        };

        auto variant_access_method2 = [](const auto& v, const std::string& name)
        {
            try
            {   //       Don't forget the address operator ---v
                if      (const auto& iter = std::get_if<bool>(&v);        iter) std::cout << "\nvariant " << name << " is bool, value = " << *iter;
                else if (const auto& iter = std::get_if<char>(&v);        iter) std::cout << "\nvariant " << name << " is char, value = " << *iter;
                else if (const auto& iter = std::get_if<int>(&v);         iter) std::cout << "\nvariant " << name << " is int,  value = " << *iter;
                else if (const auto& iter = std::get_if<std::string>(&v); iter) std::cout << "\nvariant " << name << " is str,  value = " << *iter;
                else                                                            std::cout << "\nvariant " << name << " is unknown type";
            }
            catch(const std::bad_variant_access& ex) { std::cout << "\nbad variant access for " << name; }
        };

        std::cout << "\n\nvariant access";
        variant_access_method0(v0, "v0");
        variant_access_method0(v1, "v1");
        variant_access_method0(v2, "v2");
        variant_access_method0(v3, "v3");
        variant_access_method1(v0, "v0");
        variant_access_method1(v1, "v1");
        variant_access_method1(v2, "v2");
        variant_access_method1(v3, "v3");
        variant_access_method2(v0, "v0");
        variant_access_method2(v1, "v1");
        variant_access_method2(v2, "v2");
        variant_access_method2(v3, "v3");


        // ********************* //
        // *** variant visit *** //
        // ********************* //
        struct visitor
        {
            explicit visitor(const std::string& name) : m_name(name) {}
            void operator()(const bool& b)        const { std::cout << "\nvariant " << m_name << " is bool, value = " << b; } 
            void operator()(const char& c)        const { std::cout << "\nvariant " << m_name << " is char, value = " << c; } 
            void operator()(const int&  i)        const { std::cout << "\nvariant " << m_name << " is int,  value = " << i; } 
            void operator()(const std::string& s) const { std::cout << "\nvariant " << m_name << " is str,  value = " << s; } 

            std::string m_name;
        };

        std::cout << "\n\nvariant visit";
        std::visit(visitor{"v0"}, v0);
        std::visit(visitor{"v1"}, v1);
        std::visit(visitor{"v2"}, v2);
        std::visit(visitor{"v3"}, v3);
        

        // ******************** //
        // *** tuple access *** //
        // ******************** //
        auto tuple_access = [](const auto& t, const std::string& name)
        {
            std::cout << "\ntuple " << name << " has bool, value = " << std::get<0>(t);
            std::cout << "\ntuple " << name << " has char, value = " << std::get<1>(t);
            std::cout << "\ntuple " << name << " has int,  value = " << std::get<2>(t);
            std::cout << "\ntuple " << name << " has str,  value = " << std::get<3>(t);
        };

        std::cout << "\n\ntuple access";
        tuple_access(t0, "t0");
        tuple_access(t1, "t1");


        // ******************* //
        // *** tuple apply *** //
        // ******************* //
        auto function = [](const bool& b, 
                           const char& c, 
                           const int&  i,
                           const std::string& s,
                           const std::string& name)
        {
            std::cout << "\ntuple " << name << " has bool, value = " << b;
            std::cout << "\ntuple " << name << " has char, value = " << c;
            std::cout << "\ntuple " << name << " has int,  value = " << i;
            std::cout << "\ntuple " << name << " has str,  value = " << s;
        };

        std::cout << "\n\ntuple apply";
        std::apply(std::bind(function, std::placeholders::_1, 
                                       std::placeholders::_2,
                                       std::placeholders::_3,
                                       std::placeholders::_4,
                                       "t0"), t0);

        std::apply(std::bind(function, std::placeholders::_1, 
                                       std::placeholders::_2,
                                       std::placeholders::_3,
                                       std::placeholders::_4,
                                       "t1"), t1);
    }
}

// ************************** // 
// *** Overloading lambda *** //
// ************************** // 
// using 3 things in variadic inheritance :
// 1. variadic using 
// 2. CTAD and deduction guide 
// 3. aggregate initialization of multi base classes

namespace alg { namespace overloading_lambda
{           
    bool b = true;
    char c = 'a';
    int  i = 123;
    std::string s = "abc";
    

    // ********************* //
    // *** original idea *** //
    // ********************* //
    void step1()
    {
        struct vb { void operator()(const bool& b)        const { std::cout << "\nvariant has bool, value = " << b; } };
        struct vc { void operator()(const char& c)        const { std::cout << "\nvariant has char, value = " << c; } };
        struct vi { void operator()(const  int& i)        const { std::cout << "\nvariant has inr,  value = " << i; } };
        struct vs { void operator()(const std::string& s) const { std::cout << "\nvariant has str,  value = " << s; } };
        struct hardcoded_visitor : public vb,vc,vi,vs
        {
            using vb::operator(); // without this using, there will be ambiguous error, but why?
            using vc::operator();
            using vi::operator();
            using vs::operator();
        };
        
        std::cout << "\n\noverloading lambda step 1";
        hardcoded_visitor visitor;
        visitor(b);
        visitor(c);
        visitor(i);
        visitor(s);
    }
    

    // ********************************* //
    // *** using lambda and variadic *** //
    // ********************************* //
    template<typename...Bs> struct variadic_visitor : public Bs... // cannot declare template class inside function, hence place it here
    {
        using Bs::operator()...;
    };

    void step2()
    {
        auto vb = [](const bool& b)        { std::cout << "\nvariant has bool, value = " << b; };
        auto vc = [](const char& c)        { std::cout << "\nvariant has char, value = " << c; };
        auto vi = [](const  int& i)        { std::cout << "\nvariant has inr,  value = " << i; };
        auto vs = [](const std::string& s) { std::cout << "\nvariant has str,  value = " << s; };
        
        std::cout << "\n\noverloading lambda step 2";
        variadic_visitor<decltype(vb), decltype(vc), decltype(vi), decltype(vs)> visitor; // will remove decltype in step3
        visitor(b);
        visitor(c);
        visitor(i);
        visitor(s);
    }


    // ********************************************************** //
    // *** using deduction guide and aggregate initialization *** //
    // ********************************************************** //
    template<typename...Bs> struct overloading_lambda_visitor : public Bs... 
    {
        using Bs::operator()...;
    };
    template<typename...Bs> overloading_lambda_visitor(Bs...) 
                         -> overloading_lambda_visitor<Bs...>;
                     //  ^                             ^--- we have not defined constructor of visitor, 
                     //  |                                  this visitor can still be constructed, due to 
                     //  deduction guide                    aggregate initialization of multi base classes

    void step3()
    {
        std::cout << "\n\noverloading lambda step 3";
        overloading_lambda_visitor visitor
        {
            [](const bool& b)        { std::cout << "\nvariant has bool, value = " << b; },
            [](const char& c)        { std::cout << "\nvariant has char, value = " << c; },
            [](const  int& i)        { std::cout << "\nvariant has inr,  value = " << i; },
            [](const std::string& s) { std::cout << "\nvariant has str,  value = " << s; }
        };
        visitor(b);
        visitor(c);
        visitor(i);
        visitor(s);

        std::cout << "\n\noverloading lambda step 3 (with std::visit)";
        std::variant<bool,char,int,std::string> v0 = b;
        std::variant<bool,char,int,std::string> v1 = c;
        std::variant<bool,char,int,std::string> v2 = i;
        std::variant<bool,char,int,std::string> v3 = s;
        std::visit(visitor, v0);
        std::visit(visitor, v1);
        std::visit(visitor, v2);
        std::visit(visitor, v3);
    }
}}

