#include<iostream>
#include<memory>
#include<shared_ptr.h> 
#include<shared_ptr_with_deleter.h> 
#include<type_erasure.h> 

// ******************************************************************** //
// This is the reason why rejected by windermute :
// 
// For std::shared_ptr, no matter whether ~A is declared virtual or not, 
// destructor of B works perfectly, i.e. print ~B then print ~A.
//
// For my shared_ptr, we need to declare ~A virtual, 
// otherwise it prints ~A without printing ~B. WHY ?
// ******************************************************************** //
namespace interview {
class A
{
public:
    A(int x) : _x(x)
    {
        std::cout << "\nA::A" << std::flush;
    }

    // intentionally, virtual is omitted
   ~A()
    {
        std::cout << "\nA::~A" << std::flush;
    }

private:
    int _x;
};

class B : public A
{
public:
    B(int x) : A(x)
    {
        std::cout << "\nB::B" << std::flush;
    }

   ~B()
    {
        std::cout << "\nB::~B" << std::flush;
    }
};


void test_wintermute_shared_ptr()
{
    {
        shared_ptr<A> p0(new A(123)); 
        std::cout << "\np0 ref count = " << p0.ref_count(); // 1
        {
            shared_ptr<A> p1(p0);
            shared_ptr<A> p2;
            p2 = p0;
            std::cout << "\np0 ref count = " << p0.ref_count(); // 3
            std::cout << "\np1 ref count = " << p1.ref_count(); // 3
            std::cout << "\np2 ref count = " << p2.ref_count(); // 3
            {
                shared_ptr<A> p3(new A(234));
                shared_ptr<A> p4(std::move(p3));
                std::cout << "\np3 ref count = " << p3.ref_count(); // 0
                std::cout << "\np4 ref count = " << p4.ref_count(); // 1

                p0 = std::move(p4);
                p3 = p1;
                p4 = p1;
                std::cout << "\np0 ref count = " << p0.ref_count(); // 1
                std::cout << "\np1 ref count = " << p1.ref_count(); // 4
                std::cout << "\np2 ref count = " << p2.ref_count(); // 4
                std::cout << "\np3 ref count = " << p3.ref_count(); // 4
                std::cout << "\np4 ref count = " << p4.ref_count(); // 4
            }
            std::cout << "\np0 ref count = " << p0.ref_count(); // 1
            std::cout << "\np1 ref count = " << p1.ref_count(); // 2
            std::cout << "\np2 ref count = " << p2.ref_count(); // 2
        }
        std::cout << "\np0 ref count = " << p0.ref_count(); // 1
    }

    std::cout << "\n\n### using std::shared_ptr ###";
    {
        std::shared_ptr<A> p0(new A(123));
        std::shared_ptr<A> p1(new B(123));
        std::cout << "\n----- out of scope -----";
    }
    std::cout << "\n\n### using my own shared_ptr ###";
    {
        shared_ptr<A> p0(new A(123));
        shared_ptr<A> p1(new B(123));
        std::cout << "\n----- out of scope -----";
    }
    std::cout << "\n\n### using my own shared_ptr_with_deleter ###";
    {
        shared_ptr_with_deleter<A> p0(new A(123));
        shared_ptr_with_deleter<A> p1(new B(123));
        std::cout << "\n----- out of scope -----";
    }

    std::cout << "\n";
    test::test_type_erasure();

}
}
