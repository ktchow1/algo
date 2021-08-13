#include<iostream>
#include<string>
#include<vector>

// [Objective]
// A syntax for defining struct and its operations independent on its members.
// Thus no need to change OP if SCHEMA is updated.

#define SCHEMA(OP)           \
    OP(std::string,   mem0)  \
    OP(std::string,   mem1)  \
    OP(std::uint32_t, mem2)  \
    OP(std::uint32_t, mem3)  \
    OP(std::vector<std::uint32_t>, mem4)

std::ostream& operator<<(std::ostream& os, const std::vector<std::uint32_t>& vec)
{
    os << "vector with data : ";
    for(const auto& x:vec) os << x << " ";
    return os;
}

#define DECLARE_MEM(type, name)  \
    type _##name;
#define OSTREAM_MEM(dummy, name) \
    std::cout << "\nmem = " << x._##name;
#define  ASSIGN_MEM(dummy, name) \
    this->_##name = rhs._##name;

struct my_class
{
    SCHEMA(DECLARE_MEM)
};
struct my_wrapper
{
    void init(const my_class& rhs) // Must be named as "rhs"
    {
        SCHEMA(ASSIGN_MEM)
    }
    SCHEMA(DECLARE_MEM)
};

template<typename T>
void print(const T& x) // Must be named as "x"
{
    SCHEMA(OSTREAM_MEM)
    std::cout << "\n";
}
#undef DECLARE_MEM
#undef OSTREAM_MEM
#undef  ASSIGN_MEM

void test_macro1()
{
    std::cout << "\nTechnique schema" << std::flush;

    my_class c;
    c._mem0 = "ABCDE";
    c._mem1 = "KLMNO";
    c._mem2 = 123;
    c._mem3 = 345;
    c._mem4 = {9,8,7,6,5,4,3,2};

    my_wrapper w;
    w.init(c);

    std::cout << "\n[class]";   print(c);
    std::cout << "\n[wrapper]"; print(w);
}




