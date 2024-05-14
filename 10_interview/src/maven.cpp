#include<iostream>
#include<memory>
#include<assert.h>

// Objective : Implement the template struct create so that it can do what create_hardcoded does.
//
// 1. to create the factory in compile time
// 2. to apply the factory in runtime 
// 3. assert is used, NOT static_assert

namespace interview {
class base
{
    // For gcc, virtual function is needed to make a class polymorphic.
    virtual void dummy() 
    {
    }
};

template<std::uint32_t N> class derived : public base
{
};

// ****************** //
// *** Approach 1 *** //
// ****************** //
std::unique_ptr<base> create_hardcoded(std::uint32_t n)
{
    switch(n)
    {
        case 1:
            return std::unique_ptr<base>(new derived<1>{});
        case 2:
            return std::unique_ptr<base>(new derived<2>{});
        case 3:
            return std::unique_ptr<base>(new derived<3>{});
        case 4:
            return std::unique_ptr<base>(new derived<4>{});
        case 10:
            return std::unique_ptr<base>(new derived<10>{});
        case 100:
            return std::unique_ptr<base>(new derived<100>{});
        default:
            return nullptr;
    }
}

// ****************** //
// *** Approach 2 *** //
// ****************** //
// Like what I have done in meta.h ...
// please check how template class is specialized.

// 1. Generic template defines the template parameters needed, class content can be empty
template<typename B, template<std::uint32_t> typename D, std::uint32_t...Ns>
class factory
{
};

// 2. Specialized template defines the recursion
template<typename B, template<std::uint32_t> typename D, std::uint32_t N, std::uint32_t...Ns>
class factory<B,D,N,Ns...>
{
public:
    static std::unique_ptr<B> create(std::uint32_t n)
    {
        if (n==N) return std::unique_ptr<base>(new D<N>{});
        else return factory<B,D,Ns...>::create(n);
    }
};

// 3. Specialized template defines the boundary
template<typename B, template<std::uint32_t> typename D>
class factory<B,D>
{
public:
    static std::unique_ptr<B> create(std::uint32_t n)
    {
        return nullptr;
    }
};

// ******************** //
// *** Wrong syntax *** //
// ******************** //
template<typename B, template<std::uint32_t> typename D, std::uint32_t...Ns>
class factory_fail
{
public:
    static std::unique_ptr<B> create(std::uint32_t n)
    {
        return nullptr;
    }
};

/*
template<typename B, template<std::uint32_t> typename D, std::uint32_t N, std::uint32_t...Ns>
std::unique_ptr<B> factory_fail<B,D,N,Ns...>::create(std::uint32_t n)
{
    if (n==N) return std::unique_ptr<base>(new D<N>{});
    else return factory_fail<B,D,Ns...>::create(n);
}

template<typename B, template<std::uint32_t> typename D>
std::unique_ptr<B> factory_fail<B,D>::create(std::uint32_t n)
{
    return nullptr;
} 
*/

void test_maven_runtime_template()
{
    auto p0 = create_hardcoded(  1);       
    auto p1 = create_hardcoded(  2);
    auto p2 = create_hardcoded(  3);
    auto p3 = create_hardcoded(  4);
    auto p4 = create_hardcoded(  5);
    auto p5 = create_hardcoded( 10);
    auto p6 = create_hardcoded(100);
    auto p7 = create_hardcoded(123);

    assert( dynamic_cast<derived<  1>*>(p0.get()));
    assert( dynamic_cast<derived<  2>*>(p1.get()));
    assert( dynamic_cast<derived<  3>*>(p2.get()));
    assert( dynamic_cast<derived<  4>*>(p3.get()));
    assert(!dynamic_cast<derived<  5>*>(p4.get()));
    assert( dynamic_cast<derived< 10>*>(p5.get()));
    assert( dynamic_cast<derived<100>*>(p6.get()));
    assert(!dynamic_cast<derived<123>*>(p7.get()));

    auto q0 = factory<base, derived, 1,2,3,4,10,100>::create(  1);       
    auto q1 = factory<base, derived, 1,2,3,4,10,100>::create(  2);
    auto q2 = factory<base, derived, 1,2,3,4,10,100>::create(  3);
    auto q3 = factory<base, derived, 1,2,3,4,10,100>::create(  4);
    auto q4 = factory<base, derived, 1,2,3,4,10,100>::create(  5);
    auto q5 = factory<base, derived, 1,2,3,4,10,100>::create( 10);
    auto q6 = factory<base, derived, 1,2,3,4,10,100>::create(100);
    auto q7 = factory<base, derived, 1,2,3,4,10,100>::create(123);

    assert( dynamic_cast<derived<  1>*>(q0.get()));
    assert( dynamic_cast<derived<  2>*>(q1.get()));
    assert( dynamic_cast<derived<  3>*>(q2.get()));
    assert( dynamic_cast<derived<  4>*>(q3.get()));
    assert(!dynamic_cast<derived<  5>*>(q4.get()));
    assert( dynamic_cast<derived< 10>*>(q5.get()));
    assert( dynamic_cast<derived<100>*>(q6.get()));
    assert(!dynamic_cast<derived<123>*>(q7.get()));
}
}
