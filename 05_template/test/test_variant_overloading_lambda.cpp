#include<iostream>
#include<functional>
#include<variant>
#include<string>


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


void test_variant_overloading_lambda()
{
    alg::overloading_lambda::step1();
    alg::overloading_lambda::step2();
    alg::overloading_lambda::step3();
}
