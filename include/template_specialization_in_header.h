#ifndef TEMPLATE_SPECIALIZATION_IN_HEADER
#define TEMPLATE_SPECIALIZATION_IN_HEADER
#include<iostream>
#include<string>
#include<unordered_map>
#include<functional>

class ARG0
{
public:
    void impl() const 
    {
        std::cout << "\nARG0::impl";
    }

    bool operator==(const ARG0& rhs) const  
    {
        return (str==rhs.str);
    }

    std::string str;
};

class ARG1
{
public:
    void impl() const 
    {
        std::cout << "\nARG1::impl";
    }

    bool operator==(const ARG1& rhs) const  
    {
        return (str==rhs.str);
    }

    std::string str;
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

// ************************************************ //
// To make ARG0 as key of std::map, we need to :
// 1. define hash function
// 2. define operator==(const ARG0&, const ARG0&)
// ************************************************ //
namespace alg
{
    struct hash_ARG0
    {
        std::size_t operator()(const ARG0& x) const
        {
            return std::hash<std::string>{}(x.str);
        }
    };
}

namespace std
{
    template<>
    struct hash<ARG1>
    {
        std::size_t operator()(const ARG1& x) const
        {
            return std::hash<std::string>{}(x.str);
        }
    };
}
#endif
