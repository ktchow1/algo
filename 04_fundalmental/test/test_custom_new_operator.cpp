#include<iostream>
#include<cstdint>
#include<cassert>
#include<string>
#include<vector>
#include<utility.h>


namespace alg
{
    std::vector<std::string> new_operator_invoked;
    char buffer[1024];

    struct A
    {
        A(std::uint32_t x, std::uint32_t y, std::uint32_t z) : m_x(x), m_y(y), m_z(z)
        {
            std::cout << "\nconstructor invoked";
        }

        ~A()
        {
            std::cout << "\ndestructor invoked";
        }

        void* operator new (size_t n) 
        { 
            std::cout << "\nnew operator invoked, size=" << n << std::flush; 
            return buffer;
        }

        void* operator new[] (size_t n) 
        { 
            std::cout << "\nnew[] operator invoked, size=" << n << std::flush; 
            return buffer;
        }

        void operator delete (void* ptr) 
        {
            std::cout << "\ndelete operator invoked" << std::flush; 
            ptr = nullptr;
        }

        void operator delete[] (void* ptr) 
        {
            std::cout << "\ndelete[] operator invoked" << std::flush; 
            ptr = nullptr;
        }

        std::uint32_t m_x;
        std::uint32_t m_y;
        std::uint32_t m_z;
    };
}

void test_custom_new_operator()
{
    auto* p0 = new alg::A{11,12,13};
    assert((std::uint64_t)p0 == (std::uint64_t)alg::buffer);
    delete p0;

    auto* p1 = new alg::A[1]{{11,12,13}};
    assert((std::uint64_t)p1 == (std::uint64_t)alg::buffer + sizeof(std::uint64_t));
    delete[] p1;

    auto* p2 = new alg::A[4]{{11,12,13},{21,22,23},{31,32,33},{41,42,43}};
    assert((std::uint64_t)p2       == (std::uint64_t)alg::buffer + sizeof(std::uint64_t));
    assert((std::uint64_t)(&p2[0]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 0);
    assert((std::uint64_t)(&p2[1]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 1);
    assert((std::uint64_t)(&p2[2]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 2);
    assert((std::uint64_t)(&p2[3]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 3);
    std::cout << "\nsize = " << *(reinterpret_cast<std::uint64_t*>(alg::buffer));
    delete[] p2;

    auto* p3 = new alg::A[7]{{11,12,13},{21,22,23},{31,32,33},{41,42,43},{51,52,53},{61,62,63},{71,72,73}};
    assert((std::uint64_t)p3       == (std::uint64_t)alg::buffer + sizeof(std::uint64_t));
    assert((std::uint64_t)(&p3[0]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 0);
    assert((std::uint64_t)(&p3[1]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 1);
    assert((std::uint64_t)(&p3[2]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 2);
    assert((std::uint64_t)(&p3[3]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 3);
    assert((std::uint64_t)(&p3[4]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 4);
    assert((std::uint64_t)(&p3[5]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 5);
    assert((std::uint64_t)(&p3[6]) == (std::uint64_t)alg::buffer + sizeof(std::uint64_t) + sizeof(alg::A) * 6);
    std::cout << "\nsize = " << *(reinterpret_cast<std::uint64_t*>(alg::buffer));
    delete[] p3;
}    



