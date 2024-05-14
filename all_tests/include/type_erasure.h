#pragma once
#include<memory>
#include<string>
#include<vector>

// ***************** //
// *** Objective *** //
// ***************** //
// Wrap irrelevant classes (classes NOT derived from same base) into the same wrapper, like std::any
// then invoke the shared interface via type_erase. The shared interface is specified in type_erase.
// Type_erase is a non-template class that can be constructed from different types, like std::any.
//
// 1. type_erase must be non-template, which housekeeps a non-template object_base pointer.
// 2. type_erase must have a template constructor, which allows construction from different types.
//
// Remark # : no these parts in shared_ptr_with_deleter
//
// **************************************** //
// *** Type erasure for member-function *** //
// **************************************** //
class type_erase_memfct
{
    class object_base
    {
    public:
        virtual ~object_base() {};
        virtual std::uint32_t shared_interface(const std::string& str) const = 0;
    };

    template<typename T>
    class object_wrapper : public object_base
    {
    public:
        object_wrapper(const T& object) : _object(object) // Remark #
        {
        }

        std::uint32_t shared_interface(const std::string& str) const override
        {
            return _object.function(str);
        }
        
    private:
        T _object; // Remark #
    };

private:
    std::unique_ptr<object_base> base_ptr;

public:
    template<typename U>
    type_erase_memfct(U&& targeted_object) : base_ptr(new object_wrapper<U>(std::forward<U>(targeted_object)))
    {
    }
    
    inline std::uint32_t shared_interface(const std::string& str) const 
    {
        return base_ptr->shared_interface(str);
    }
};

// ******************************** //
// *** Type erasure for functor *** //
// ******************************** //
class type_erase_functor
{
    class object_base
    {
    public:
        virtual ~object_base() {};
        virtual std::uint32_t shared_interface(const std::string& str) const = 0;
    };

    template<typename T>
    class object_wrapper : public object_base
    {
    public:
        object_wrapper(const T& object) : _object(object) // Remark #
        {
        }

        std::uint32_t shared_interface(const std::string& str) const override
        {
            return _object(str); // Main difference between type_erase_memfct and type_erase_functor
        }

    private:
        T _object; // Remark #
    };

private:
    std::unique_ptr<object_base> base_ptr;

public:
    template<typename U>
    type_erase_functor(U&& targeted_object) : base_ptr(new object_wrapper<U>(std::forward<U>(targeted_object)))
    {
    }
    
    inline std::uint32_t shared_interface(const std::string& str) const 
    {
        return base_ptr->shared_interface(str);
    }
};

inline void invoke_shared_interface(const type_erase_memfct& any)
{
    any.shared_interface("hello world");
}

inline void invoke_shared_interface(const type_erase_functor& any)
{
    any.shared_interface("byebye");
}

namespace test {
class A
{
public: 
    A(std::uint32_t x, std::uint32_t y) : _x(x), _y(y) {}
    inline std::uint32_t function  (const std::string& str) const { std::cout << "\n" << str << " A::function";   return 0; }
    inline std::uint32_t operator()(const std::string& str) const { std::cout << "\n" << str << " A::operator()"; return 0; }

private:
    std::uint32_t _x;
    std::uint32_t _y;
};

class B
{
public: 
    B(std::uint32_t x, std::uint32_t y, std::uint32_t z) : _x(x), _y(y), _z(z) {}
    inline std::uint32_t function  (const std::string& str) const { std::cout << "\n" << str << " B::function";   return 0; }
    inline std::uint32_t operator()(const std::string& str) const { std::cout << "\n" << str << " B::operator()"; return 0; }

private:
    std::uint32_t _x;
    std::uint32_t _y;
    std::uint32_t _z;
};

inline std::uint32_t f(const std::string& str)
{
    std::cout << "\n" << str << " global f";
    return 123;
}

inline void test_type_erasure()
{
    invoke_shared_interface(type_erase_memfct(A{1,2}));
    invoke_shared_interface(type_erase_memfct(B{1,2,3}));
    invoke_shared_interface(type_erase_functor(A{1,2}));
    invoke_shared_interface(type_erase_functor(B{1,2,3}));
    invoke_shared_interface(type_erase_functor(f));
    invoke_shared_interface(type_erase_functor(+[](const std::string& str)
    {
        std::cout << "\n" << str << " lamba prefixed with +"; return 0; 
    }));


    std::vector<type_erase_functor> vec;
    vec.push_back(type_erase_functor(A{1,2}));
    vec.push_back(type_erase_functor(B{1,2,3}));
    vec.push_back(type_erase_functor(f));
    vec.push_back(type_erase_functor(+[](const std::string& str)
    {
        std::cout << "\n" << str << " lamba prefixed with +"; return 0; 
    }));
    for(const auto& x:vec)
    {
        invoke_shared_interface(x);
    }
}
}



