#include<iostream>
#include<cassert>
#include<cstdint>
#include<memory>
#include<vector>
#include<utility.h>


namespace alg
{
    std::vector<std::string> deleter_invoked;

    template<typename T> 
    struct custom_deleter
    {
        void operator()(T* ptr)
        { 
            deleter_invoked.push_back( "custom delete");
            ptr->~T();
        }
    };


}


// ****************************************** //
// *** Test custom deleter for unique_ptr *** //
// ****************************************** //
void test_custom_deleter_for_unique_ptr()
{
    struct A
    {
        std::uint64_t m_x;
        std::uint64_t m_y;
        std::uint64_t m_z;
    };

    {
        auto ptr = std::unique_ptr<A, alg::custom_deleter<A>>(new A, alg::custom_deleter<A>{});
    }
    
    assert(alg::deleter_invoked[0] == "custom delete"); 
    print_summary("custom deleter for unique_ptr", "succeeded");
}

/*

struct resource
{
    std::uint32_t a;
    std::uint32_t b;
    std::uint32_t c;
};

std::ostream& operator<<(std::ostream& os, const resource& x)
{
    os << "resource: a=" << x.a << " b=" << x.b << " c=" << x.c << std::flush;
    return os;
}

template<std::uint32_t N> class resource_pool
{
public:
    resource_pool()
    {
        for(std::uint32_t n=0; n!=N; ++n)
        {
            flags[n] = true;
        }
    }

    class resource_deleter
    {
    public:
        resource_deleter(resource_pool<N>& pool_, std::uint32_t index_) : pool(pool_), index(index_)
        {
        }

        void operator()(resource* ptr)
        {
            pool.flags[index] = true;
        }

        resource_pool<N>& pool;
        std::uint32_t index;
    };

public:
    static const std::uint32_t size = N;
    friend class resource_deleter;

    // Normally we cannot assign smart pointer to stack object,
    // unless we provide appropriate deleter in declaring output.
    template<typename... ARGS> auto make_unique(ARGS&&... args)
    {
        for(std::uint32_t n=0; n!=N; ++n)
        {
            if (flags[n])    
            {
                flags[n] = false;

                resource_deleter deleter(*this, n);
                std::unique_ptr<resource, resource_deleter> output(nullptr, deleter);
                output.reset
                (
                    new (&impl[n]) resource{std::forward<ARGS>(args)...}
                );  
                return output;
            }
        }
        resource_deleter deleter(*this, 0);
        std::unique_ptr<resource, resource_deleter> output(nullptr, deleter);
        return output; 
    }

    void debug() const 
    {
        for(std::uint32_t n=0; n!=N; ++n)
        {
            std::cout << "\nimpl[" << n << "] flag=" << flags[n];
            if (!flags[n]) std::cout << " " << impl[n];
        }
    }


private:
    resource impl[N];
    bool flags[N]; // true for available
}; 

void test_deleter_for_pool()
{
     auto x = std::make_unique<resource>((std::uint32_t)1,(std::uint32_t)2,(std::uint32_t)3);
     std::cout << "\n" << *x;
     std::cout << "\n";

     resource_pool<5> pool;
     auto x0 = pool.make_unique((std::uint32_t)11,(std::uint32_t)12,(std::uint32_t)13);
     auto x1 = pool.make_unique((std::uint32_t)21,(std::uint32_t)22,(std::uint32_t)23);
     {
         auto x2 = pool.make_unique((std::uint32_t)31,(std::uint32_t)32,(std::uint32_t)33);
         auto x3 = pool.make_unique((std::uint32_t)41,(std::uint32_t)42,(std::uint32_t)43);
         pool.debug();
         std::cout << "\n";
     }
     pool.debug();
     std::cout << "\n";

     auto x4(std::move(x1)); 
     auto x5 = pool.make_unique((std::uint32_t)51,(std::uint32_t)52,(std::uint32_t)53);
     auto x6 = pool.make_unique((std::uint32_t)61,(std::uint32_t)62,(std::uint32_t)63);
     auto x7 = pool.make_unique((std::uint32_t)71,(std::uint32_t)72,(std::uint32_t)73);
     auto x8 = pool.make_unique((std::uint32_t)81,(std::uint32_t)82,(std::uint32_t)83);
     pool.debug();
     std::cout << "\n";
     
     if (x6) std::cout << "\nx6 = " << *x6; else std::cout << "\nx6 is null";
     if (x7) std::cout << "\nx7 = " << *x7; else std::cout << "\nx7 is null";
     if (x8) std::cout << "\nx8 = " << *x8; else std::cout << "\nx8 is null";
     std::cout << "\n";
}

*/






void test_custom_deleter()
{
    test_custom_deleter_for_unique_ptr();
//  test_custom_deleter_for_stack_pool();
}
