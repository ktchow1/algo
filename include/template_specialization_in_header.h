#ifndef TEMPLATE_SPECIALIZATION_IN_HEADER
#define TEMPLATE_SPECIALIZATION_IN_HEADER
#include<iostream>

class ARG0
{
public:
    void impl() const 
    {
        std::cout << "\nARG0::impl";
    }
};

class ARG1
{
public:
    void impl() const 
    {
        std::cout << "\nARG1::impl";
    }
};

class ARG2
{
public:
    void impl() const 
    {
        std::cout << "\nARG2::impl";
    }

    void fct() const 
    {
        std::cout << "\nARG2::fct";
    }
};

template<typename T>
void template_function(const T& x)
{
    x.impl();
}

template<>
void template_function(const ARG2& x);

// Compilation error : Do not implement template specialization header
// template<>
// void template_function(const ARG2& x)
// {
//     x.fct();
// } 
#endif
