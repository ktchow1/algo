#include<iostream>

enum class msg_type : std::uint8_t
{
    type0,
    type1,
    type2,
    type3,
};

// ****************************** //
// *** Polymorphism using POD *** //
// ****************************** //
namespace pod_poly 
{

struct base
{
    msg_type type;
    void virtual_fct();
};
struct derived0
{
    base header;
    std::uint32_t a;
    std::uint32_t b;
    void virtual_fct() { std::cout << "\nderived0 implementation";  }
};
struct derived1 
{
    base header;
    std::uint32_t a;
    std::uint32_t b;
    std::uint32_t c;
    void virtual_fct() { std::cout << "\nderived1 implementation";  }
};
struct derived2 
{
    base header;
    std::string s;
    void virtual_fct() { std::cout << "\nderived2 implementation";  }
};
struct derived3 
{
    base header;
    std::string s0;
    std::string s1;
    void virtual_fct() { std::cout << "\nderived3 implementation";  }
};

}

void test_polymorphism()
{
}
