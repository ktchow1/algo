#include<iostream>
#include<concepts>
#include<string>

struct A {};
struct B {};

struct type0 
{
    using value_type = void;
    operator A() { return A{}; } 
    operator B() { return B{}; } 
    void          f() const {}
    std::uint32_t g() const { return 0UL; }

};

struct type1 
{
//  using value_type = void;
    operator A() { return A{}; } 
    operator B() { return B{}; } 
    void          f() const {}
    std::uint32_t g() const { return 0UL; }
};

struct type2 
{
    using value_type = void;
//  operator A() { return A{}; } 
    operator B() { return B{}; } 
    void          f() const {}
    std::uint32_t g() const { return 0UL; }
};

struct type3 
{
    using value_type = void;
    operator A() { return A{}; } 
//  operator B() { return B{}; } 
    void          f() const {}
    std::uint32_t g() const { return 0UL; }
};

struct type4 
{
    using value_type = void;
    operator A() { return A{}; } 
    operator B() { return B{}; } 
//  void          f() const {}
    std::uint32_t g() const { return 0UL; }
};

struct type5 
{
    using value_type = void;
    operator A() { return A{}; } 
    operator B() { return B{}; } 
    void          f() const {}
//  std::uint32_t g() const { return 0UL; }
};

struct type6 
{
    using value_type = void;
    operator A() { return A{}; } 
    operator B() { return B{}; } 
    void          f() const {}
    std::uint32_t g() const { return 0UL; }
};

struct type7 
{
    using value_type = void;
    operator A() { return A{}; } 
    operator B() { return B{}; } 
    void          f() const {}
    std::uint32_t g() const { return 0UL; }
};

template<typename T> struct is_xxx        { static const bool value = false; };
template<>           struct is_xxx<type0> { static const bool value = true;  };
template<>           struct is_xxx<type1> { static const bool value = true;  };
template<>           struct is_xxx<type2> { static const bool value = true;  };
template<>           struct is_xxx<type3> { static const bool value = true;  };
template<>           struct is_xxx<type4> { static const bool value = true;  };
template<>           struct is_xxx<type5> { static const bool value = true;  };
//                   struct is_xxx<type6> { static const bool value = true;  };
template<>           struct is_xxx<type7> { static const bool value = true;  };

template<typename T> struct is_yyy        { static const bool value = false; };
template<>           struct is_yyy<type0> { static const bool value = true;  };
template<>           struct is_yyy<type1> { static const bool value = true;  };
template<>           struct is_yyy<type2> { static const bool value = true;  };
template<>           struct is_yyy<type3> { static const bool value = true;  };
template<>           struct is_yyy<type4> { static const bool value = true;  };
template<>           struct is_yyy<type5> { static const bool value = true;  };
template<>           struct is_yyy<type6> { static const bool value = true;  };
//                   struct is_yyy<type7> { static const bool value = true;  };
template<typename T> constexpr bool is_yyy_v = is_yyy<T>::value;


template<typename T> concept my_requirements = 
std::convertible_to<T,A>  &&
is_xxx<T>::value     &&
is_yyy_v<T>          &&
requires (T x)
{
    typename T::value_type;
      x.f();
    { x.g() } -> std::same_as<std::uint32_t>;
    requires std::convertible_to<T,B>;
};


template<typename T> void f(const T& x) { std::cout << "\ngeneric version f"; }
template<typename T> void g(const T& x) { std::cout << "  generic version g"; }
template<typename T> void h(const T& x) { std::cout << "  generic version h"; }

template<my_requirements T> 
void f(const T& x) { std::cout << "\nspecial version f"; }

template<typename T> requires my_requirements<T> 
void g(const T& x) { std::cout << "  special version g"; }

template<typename T> 
void h(const T& x) requires my_requirements<T> 
{ std::cout << "  special version h"; }

void test_concepts()
{
    type0 x0; f(x0); g(x0); h(x0); // only this type invokes special version
    type1 x1; f(x1); g(x1); h(x1); // yet other types invoke generic version
    type2 x2; f(x2); g(x2); h(x2);
    type3 x3; f(x3); g(x3); h(x3);
    type4 x4; f(x4); g(x4); h(x4);
    type5 x5; f(x5); g(x5); h(x5);
    type6 x6; f(x6); g(x6); h(x6);
    type7 x7; f(x7); g(x7); h(x7);
}


#include<concepts>
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