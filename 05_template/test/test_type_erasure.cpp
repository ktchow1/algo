#include<iostream>
#include<type_erasure.h>


class A
{
public: 
    A(std::uint32_t x, std::uint32_t y) : _x(x), _y(y) {}
    inline std::uint32_t function  (const std::string& str) const { std::cout << "\n" << str << " A::function";   return 0; }
    inline std::uint32_t operator()(const std::string& str) const { std::cout << "\n" << str << " A::operator()"; return 0; }

private:
    std::uint32_t _x;
    std::uint32_t _y;
};

class B
{
public: 
    B(std::uint32_t x, std::uint32_t y, std::uint32_t z) : _x(x), _y(y), _z(z) {}
    inline std::uint32_t function  (const std::string& str) const { std::cout << "\n" << str << " B::function";   return 0; }
    inline std::uint32_t operator()(const std::string& str) const { std::cout << "\n" << str << " B::operator()"; return 0; }

private:
    std::uint32_t _x;
    std::uint32_t _y;
    std::uint32_t _z;
};

class C
{
public: 
    C(std::string x, std::string y, std::string z) : _x(x), _y(y), _z(z) {}
    inline std::uint32_t function  (const std::string& str) const { std::cout << "\n" << str << " C::function";   return 0; }
    inline std::uint32_t operator()(const std::string& str) const { std::cout << "\n" << str << " C::operator()"; return 0; }

private:
    std::string _x;
    std::string _y;
    std::string _z;
};

inline std::uint32_t f(const std::string& str)
{
    std::cout << "\n" << str << " global f";
    return 123;
}

inline std::uint32_t g(const std::string& str)
{
    std::cout << "\n" << str << " global g";
    return 123;
}

void test_type_erasure()
{
    // ****************************** //
    // *** Test type erase memfct *** //
    // ****************************** //
    alg::type_erase_memfct x0(A{1,2});
    alg::type_erase_memfct x1(B{1,2,3});
    alg::type_erase_memfct x2(C{"1","2","3"});
    x0.shared_interface("hello world");
    x1.shared_interface("hello world");
    x2.shared_interface("hello world");

    // ******************************* //
    // *** Test type erase functor *** //
    // ******************************* //
    alg::type_erase_functor y0(A{1,2});
    alg::type_erase_functor y1(B{1,2,3});
    alg::type_erase_functor y2(C{"1","2","3"});
    alg::type_erase_functor y3(f);
    alg::type_erase_functor y4(g);
    alg::type_erase_functor y5(+[](const std::string& str) -> std::uint32_t
    {
        std::cout << "\n" << str << " lambda prefixed with +"; return 0; 
    });
    y0.shared_interface("byebye");
    y1.shared_interface("byebye");
    y2.shared_interface("byebye");
    y3.shared_interface("byebye");
    y4.shared_interface("byebye");
    y5.shared_interface("byebye");

    // ******************************* //
    // *** Test type erase functor *** //
    // ******************************* //
    std::vector<alg::type_erase_functor> vec;

    // 1. rvalue type_erasure
    vec.push_back(alg::type_erase_functor(A{1,2}));
    vec.push_back(alg::type_erase_functor(B{1,2,3}));
    vec.push_back(alg::type_erase_functor(C{"1","2","3"}));
    vec.push_back(alg::type_erase_functor(f));
    vec.push_back(alg::type_erase_functor(g));
    vec.push_back(alg::type_erase_functor(+[](const std::string& str) -> std::uint32_t
    {
        std::cout << "\n" << str << " lambda prefixed with +"; return 0; 
    }));
    
    // 2. lvalue type_erasure
    vec.push_back(y0);
    vec.push_back(y1);
    vec.push_back(y2);
    vec.push_back(y3);
    vec.push_back(y4);
    vec.push_back(y5);

    // 3. rvalue raw type
    vec.push_back(A{1,2});
    vec.push_back(B{1,2,3});
    vec.push_back(C{"1","2","3"});
    vec.push_back(f);
    vec.push_back(g);
    vec.push_back(+[](const std::string& str) -> std::uint32_t
    {
        std::cout << "\n" << str << " lambda prefixed with +"; return 0; 
    });

    // 4. lvalue raw type (this part is the reason why std::decay_t is needed in type_erasure)
    auto z0 = A{1,2};
    auto z1 = B{1,2,3};
    auto z2 = C{"1","2","3"};
    auto z3 = &f;
    auto z4 = &g;
    auto z5 = +[](const std::string& str) -> std::uint32_t
    {
        std::cout << "\n" << str << " lambda prefixed with +"; return 0; 
    };
    vec.push_back(z0);
    vec.push_back(z1);
    vec.push_back(z2);
    vec.push_back(z3);
    vec.push_back(z4);
    vec.push_back(z5);

    // execution
    for(const auto& x:vec)
    {
        x.shared_interface("byebye again");
    }
}

