#include<iostream>
#include<cassert>
#include<functional>
#include<variant>
#include<tuple>
#include<string>
#include<sstream>
#include<utility.h>


std::string visitor_invoked;

// *************************************************************************************************** //
// Variant vs tuple
//
// std::variant<T0,T1,T2> =     sum type, i.e. feasible set = set of T0 + set of T1 + set of T2 + ...
// std::tuple<T0,T1,T2>   = product type, i.e. feasible set = set of T0 * set of T1 * set of T2 * ...
// 
// std::variant works with std::visit (and overloading lambda)
// std::tuple works with std::apply
// *************************************************************************************************** //
void test_variant_vs_tuple()
{
    bool        b = true;
    char        c = 'c';
    int         i = 12345;
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
            std::stringstream ss;
            if      (v.index()==0) ss << "variant " << name << " is bool, value = " << std::get<0>(v);
            else if (v.index()==1) ss << "variant " << name << " is char, value = " << std::get<1>(v);
            else if (v.index()==2) ss << "variant " << name << " is int,  value = " << std::get<2>(v);
            else if (v.index()==3) ss << "variant " << name << " is str,  value = " << std::get<3>(v);
            else                   ss << "variant " << name << " is unknown type";
            visitor_invoked = ss.str();
        }
        catch(const std::bad_variant_access& ex) { std::cout << "\nbad variant access for " << name; }
    };

    auto variant_access_method1 = [](const auto& v, const std::string& name)
    {
        try
        {
            std::stringstream ss;
            if      (std::holds_alternative<bool>(v))        ss << "variant " << name << " is bool, value = " << std::get<bool>(v);
            else if (std::holds_alternative<char>(v))        ss << "variant " << name << " is char, value = " << std::get<char>(v);
            else if (std::holds_alternative<int>(v))         ss << "variant " << name << " is int,  value = " << std::get<int>(v);
            else if (std::holds_alternative<std::string>(v)) ss << "variant " << name << " is str,  value = " << std::get<std::string>(v);
            else                                             ss << "variant " << name << " is unknown type";
            visitor_invoked = ss.str();
        }
        catch(const std::bad_variant_access& ex) { std::cout << "\nbad variant access for " << name; }
    };

    auto variant_access_method2 = [](const auto& v, const std::string& name)
    {
        try
        { 
            std::stringstream ss;
            //       Don't forget the address operator ---v
            if      (const auto& iter = std::get_if<bool>(&v);        iter) ss << "variant " << name << " is bool, value = " << *iter;
            else if (const auto& iter = std::get_if<char>(&v);        iter) ss << "variant " << name << " is char, value = " << *iter;
            else if (const auto& iter = std::get_if<int>(&v);         iter) ss << "variant " << name << " is int,  value = " << *iter;
            else if (const auto& iter = std::get_if<std::string>(&v); iter) ss << "variant " << name << " is str,  value = " << *iter;
            else                                                            ss << "variant " << name << " is unknown type";
            visitor_invoked = ss.str();
        }
        catch(const std::bad_variant_access& ex) { std::cout << "\nbad variant access for " << name; }
    };

    variant_access_method0(v0, "v0");   assert(visitor_invoked == "variant v0 is bool, value = 1");
    variant_access_method0(v1, "v1");   assert(visitor_invoked == "variant v1 is char, value = c");
    variant_access_method0(v2, "v2");   assert(visitor_invoked == "variant v2 is int,  value = 12345");
    variant_access_method0(v3, "v3");   assert(visitor_invoked == "variant v3 is str,  value = str");
    variant_access_method1(v0, "v0");   assert(visitor_invoked == "variant v0 is bool, value = 1");
    variant_access_method1(v1, "v1");   assert(visitor_invoked == "variant v1 is char, value = c");
    variant_access_method1(v2, "v2");   assert(visitor_invoked == "variant v2 is int,  value = 12345");
    variant_access_method1(v3, "v3");   assert(visitor_invoked == "variant v3 is str,  value = str");
    variant_access_method2(v0, "v0");   assert(visitor_invoked == "variant v0 is bool, value = 1");
    variant_access_method2(v1, "v1");   assert(visitor_invoked == "variant v1 is char, value = c");
    variant_access_method2(v2, "v2");   assert(visitor_invoked == "variant v2 is int,  value = 12345");
    variant_access_method2(v3, "v3");   assert(visitor_invoked == "variant v3 is str,  value = str");
    print_summary("variant - variant access", "succeeded");


    // ********************* //
    // *** variant visit *** //
    // ********************* //
    struct visitor
    {
        explicit visitor(const std::string& name) : m_name(name) {}
        void operator()(const bool& b)        { m_ss << "variant " << m_name << " is bool, value = " << b; visitor_invoked = m_ss.str(); } 
        void operator()(const char& c)        { m_ss << "variant " << m_name << " is char, value = " << c; visitor_invoked = m_ss.str(); } 
        void operator()(const int&  i)        { m_ss << "variant " << m_name << " is int,  value = " << i; visitor_invoked = m_ss.str(); } 
        void operator()(const std::string& s) { m_ss << "variant " << m_name << " is str,  value = " << s; visitor_invoked = m_ss.str(); } 

        std::string m_name;
        std::stringstream m_ss;
    };

    std::visit(visitor{"v0"}, v0);   assert(visitor_invoked == "variant v0 is bool, value = 1");
    std::visit(visitor{"v1"}, v1);   assert(visitor_invoked == "variant v1 is char, value = c");
    std::visit(visitor{"v2"}, v2);   assert(visitor_invoked == "variant v2 is int,  value = 12345");
    std::visit(visitor{"v3"}, v3);   assert(visitor_invoked == "variant v3 is str,  value = str");
    print_summary("variant - variant visit", "succeeded");
    

    // ******************** //
    // *** tuple access *** //
    // ******************** //
    auto tuple_access = [](const auto& t, const std::string& name)
    {
        std::stringstream ss;
        ss << "tuple " << name 
           << " has value " << std::get<0>(t)
                    << ", " << std::get<1>(t)
                    << ", " << std::get<2>(t)
                    << ", " << std::get<3>(t);
        visitor_invoked = ss.str();
    };

    tuple_access(t0, "t0");   assert(visitor_invoked == "tuple t0 has value 1, c, 12345, str");
    tuple_access(t1, "t1");   assert(visitor_invoked == "tuple t1 has value 1, c, 12345, str");
    print_summary("variant - tuple access", "succeeded");


    // ******************* //
    // *** tuple apply *** //
    // ******************* //
    auto function = [](const bool& b, 
                       const char& c, 
                       const int&  i,
                       const std::string& s,
                       const std::string& name)
    {
        std::stringstream ss;
        ss << "tuple " << name 
           << " has value " << b
                    << ", " << c
                    << ", " << i
                    << ", " << s;
        visitor_invoked = ss.str();
    };

    using namespace std::placeholders;
    std::apply(std::bind(function, _1, _2, _3, _4, "t0"), t0);
    assert(visitor_invoked == "tuple t0 has value 1, c, 12345, str");
    std::apply(std::bind(function, _1, _2, _3, _4, "t1"), t1);
    assert(visitor_invoked == "tuple t1 has value 1, c, 12345, str");
    print_summary("variant - tuple apply", "succeeded");
}

