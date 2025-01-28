#include<iostream>
#include<cassert>
#include<cstdint>
#include<cstring>
#include<string>
#include<vector>
#include<utility.h>


void test_placement_new_basic()
{
    struct A
    {
        std::uint32_t m_x;
        std::uint32_t m_y;
        std::uint32_t m_z;
    };

    char  stack_buf[sizeof(A)];
    char*  heap_buf = new char[sizeof(A)];

    auto* p0 = new (stack_buf) A{11,12,13};
    auto* p1 = new ( heap_buf) A{21,22,23};
    auto* p2 = reinterpret_cast<A*>(stack_buf);
    auto* p3 = reinterpret_cast<A*>( heap_buf);
    assert(p0->m_x == 11 && p0->m_y == 12 && p0->m_z == 13); 
    assert(p1->m_x == 21 && p1->m_y == 22 && p1->m_z == 23); 
    assert(p2->m_x == 11 && p2->m_y == 12 && p2->m_z == 13); 
    assert(p3->m_x == 21 && p3->m_y == 22 && p3->m_z == 23); 

    print_summary("placement new - basic", "succeeded");
    delete[] heap_buf; 
}


/*
// ********************************************************************************* //
// *** Test 1 : Why emplace is needed? How is it implemented with placement new? *** //
// ********************************************************************************* //
template<typename T> struct my_vec_fail 
{
    void push_back(const T& rhs) 
    {
        impl[count] = rhs;
        ++count;
    }

    void push_back(T&& rhs) // rvalue reference, not universal reference
    {
        impl[count] = std::move(rhs);
        ++count;
    }

    template<typename... ARGS> void emplace_back(ARGS&&... args) 
    {
        impl[count] = T(std::forward<ARGS>(args)...); // extra construction and extra assignment
        ++count;
    }

    T impl[5];
    std::uint8_t count = 0;
};

template<typename T> struct my_vec // memory is allocated, we need to explicitly invoke constructors
{
    ~my_vec() // Please explicit call destructor for placement new
    {                
        for(std::uint8_t n=0; n!=count; n+=sizeof(T))
        {            
            reinterpret_cast<T*>(impl + n)->~T();            
        }
    }

    void push_back(const T& rhs)
    {
        new (impl + count) T(rhs);
        count += sizeof(T);
    }

    void push_back(T&& rhs) // rvalue reference, not universal reference
    {
        new (impl + count) T(std::move(rhs));
        count += sizeof(T);
    }

    template<typename... ARGS> void emplace_back(ARGS&&... args) // correct implementation
    {
        new (impl + count) T(std::forward<ARGS>(args)...);
        count += sizeof(T);
    }

    char impl[sizeof(T)*5];
    std::uint8_t count = 0;
};

void test_placement_new1() // emplace implemented by placement new
{    
    std::cout << "\n\nvecA ";
    {
        std::vector<X> vecA;        
        vecA.reserve(10);                                                      // doesn't invoke constructor 0
        std::cout << "\ntrial 0 : "; X x0(2,3,4); vecA.push_back   (x0);       // invokes constructor 1,2
        std::cout << "\ntrial 1 : ";              vecA.push_back   (X{3,4,5}); // invokes constructor 1,3, destructor
        std::cout << "\ntrial 2 : "; X x1(4,5,6); vecA.emplace_back(x1);       // invokes constructor 1,2
        std::cout << "\ntrial 3 : ";              vecA.emplace_back(X{5,6,7}); // invokes constructor 1,3, destructor
        std::cout << "\ntrial 4 : ";              vecA.emplace_back  (6,7,8);  // invokes constructor 1 (no copy nor move)
        std::cout << "\n";                                                     // invokes destructor  7 times (x0, x1 and 5 in vector)
    }

    std::cout << "\n\nvecB ";
    {
        my_vec_fail<X> vecB;                                                   // invokes constructor 0 for 5 times    
        std::cout << "\ntrial 0 : "; X x2(2,3,4); vecB.push_back   (x2);       // invokes constructor 1,   copy-assignment
        std::cout << "\ntrial 1 : ";              vecB.push_back   (X{3,4,5}); // invokes constructor 1,   move-assignment, destructor
        std::cout << "\ntrial 2 : "; X x3(4,5,6); vecB.emplace_back(x3);       // invokes constructor 1,2, move-assignment, destructor
        std::cout << "\ntrial 3 : ";              vecB.emplace_back(X{5,6,7}); // invokes constructor 1,3, move-assignment, destructor x 2
        std::cout << "\ntrial 4 : ";              vecB.emplace_back(  6,7,8);  // invokes constructor 1,   move-assignment, destructor
        std::cout << "\n";                                                     // invokes destructor  7 times (x0, x1 and 5 in vector)
    }

    // same invocation as std::vector, yeah!!!    
    std::cout << "\n\nvecC ";
    {
        my_vec<X> vecC;                                                        // doesn't invoke constructor 0     
        std::cout << "\ntrial 0 : "; X x4(2,3,4); vecC.push_back   (x4);       // invokes constructor 1,2
        std::cout << "\ntrial 1 : ";              vecC.push_back   (X{3,4,5}); // invokes constructor 1,3, destructor
        std::cout << "\ntrial 2 : "; X x5(4,5,6); vecC.emplace_back(x5);       // invokes constructor 1,2
        std::cout << "\ntrial 3 : ";              vecC.emplace_back(X{5,6,7}); // invokes constructor 1,3, destructor
        std::cout << "\ntrial 4 : ";              vecC.emplace_back(  6,7,8);  // invokes constructor 1 (no copy nor move)
        std::cout << "\n";                                                     // invokes destructor  7 times (x0, x1 and 5 in vector)
    }
    std::cout << "\n\n";
}


// ************************************************** //
// *** Test 2 : Placement new vs reinterpret_cast *** //
// ************************************************** //
// 1. both work with/without pragma pack (so pragma pack is not the difference)
// 2. placement new invokes constructors while reinterpret_cast does not
//    placement new is for non-aggregate while reinterpret_cast is for aggregate
//
// Remark : T0::S and T1::S are not aggregate, 
//          while T0 and T1 are ... (please check)
// ************************************************** //
struct S
{
    S(std::uint8_t N) : size(N)
    {
        std::uint8_t n = N % 3;
        if      (n == 0) std::memcpy(ac, "abcd", 5); // '\0' is added
        else if (n == 1) std::memcpy(ac, "ijkl", 5);
        else             std::memcpy(ac, "wxyz", 5);
    }

    std::uint8_t size;
    char ac[5];
};

struct T0
{
    T0(std::uint8_t N) : n1(N), n2(n1*n1), n4(n2*n2), as{ S(N),S(N+1),S(N+2) }
    {                
    }

    std::uint8_t  n1;
    std::uint16_t n2;
    std::uint32_t n4;
    S as[3];
};

#pragma pack(push, 1) // ensure no padding 
struct T1
{
    T1(std::uint8_t N) : n1(N), n2(n1* n1), n4(n2* n2), as{ S(N),S(N + 1),S(N + 2) }
    {
    }

    std::uint8_t  n1;
    std::uint16_t n2;
    std::uint32_t n4;
    S as[3];
};
#pragma pack(pop) 

template<typename T> void print(const T& t)
{
    std::cout << "\nn1=" << (int)(t.n1) << " n2=" << t.n2 << " n4=" << t.n4 << " " << t.as[0].ac << " " << t.as[1].ac << " " << t.as[2].ac;
}

void test_placement_new2()
{
    std::cout << "\n\n\n\n";
    std::cout << "\nsizeof(T0) = " << sizeof(T0);
    std::cout << "\nsizeof(T1) = " << sizeof(T1);

    char impl[100];
    new (impl) T0(0); print(*reinterpret_cast<T0*>(impl));
    new (impl) T0(1); print(*reinterpret_cast<T0*>(impl));
    new (impl) T0(2); print(*reinterpret_cast<T0*>(impl));
    new (impl) T0(3); print(*reinterpret_cast<T0*>(impl));
    new (impl) T1(0); print(*reinterpret_cast<T1*>(impl));
    new (impl) T1(1); print(*reinterpret_cast<T1*>(impl));
    new (impl) T1(2); print(*reinterpret_cast<T1*>(impl));
    new (impl) T1(3); print(*reinterpret_cast<T1*>(impl));
}

*/

void test_custom_placement_new()
{
    test_placement_new_basic();
}
