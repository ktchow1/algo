#pragma once
#include<cassert>
#include<concepts>
#include<utility>       // for std::forward
#include<type_traits>   // for std::remove_reference


// 1. rvalueness of an expression is an attribute
//    rvalue reference is a type
// 2. rvalue reference can bind to rvalue expression
//    rvalue reference by itself is lvalue expression, if it is named
// 3. rvalue expression can be converted to lvalue by assigning it with named variable
//    lvalue expression can be converted to rvalue by static_cast std::move
//
namespace alg
{
    template<typename T> typename std::remove_reference<T>::type&& move(T&& x) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(x);
    }

    template<typename T> T&& forward(typename std::remove_reference<T>::type& x) noexcept
    {
        return static_cast<T&&>(x);
    }
}

namespace alg
{
    enum class object_source
    {
        default_constructed,
        copy_constructed,
        copy_assigned,
        move_constructed,
        move_assigned
    };

    template<typename T>
    requires std::copyable<T>
    class rvalue_factory
    {
    public:
        template<typename...ARGS>
        rvalue_factory(ARGS...args) : m_value{alg::forward<ARGS>(args)...} // note : this is alg::forward
        {
        }

        // Though it returns m_value for all create function, the return type determines what is returned.
        // Without std::move for create_xvalue(), compile error : cannot bind rvalue reference to lvalue.
        const T&  create_const_lvalue() const noexcept { return  m_value;            }
              T&  create_lvalue()             noexcept { return  m_value;            }
              T&& create_xvalue()             noexcept { return  alg::move(m_value); } // note : this is alg::move
              T   create_prvalue()      const noexcept { return  m_value;            }
        const T*  get_pointer()         const noexcept { return &m_value;            }
               
    private:
        T m_value;
    }; 
                                                                      //  const lvalue | lvalue | xvalue | prvalue
    template<typename T> void binding_const_lvalue_ref(const T&) {}   //           Y   |    Y   |    Y   |    Y
    template<typename T> void binding_lvalue_ref(T&) {}               //           N   |    Y   |    N   |    N
    template<typename T> void binding_rvalue_ref(T&&) {}              //           N   |    N   |    Y   |    Y
    template<typename T> void binding_value(T) {}                     //           Y   |    Y   |    Y   |    Y
    

    //                         | DC  CC  CA  MC  MA  DD 
    // ------------------------+-------------------------
    // if emplicitly define DC | -   *   *   *   *   * 
    // if emplicitly define CC |     -   *   *   *   *
    // if emplicitly define CA | *   *   -           * 
    // if emplicitly define MC |             -       *
    // if emplicitly define MA | *               -   *       
    // if emplicitly define DD |         *   *   *   -    ... then asterisked functions will be compiler generated 
    //
      
    struct explicitly_defined_DC { private:  explicitly_defined_DC() {} };
    struct explicitly_defined_CC { private:  explicitly_defined_CC           (const explicitly_defined_CC&) {               } };
    struct explicitly_defined_CA { private:  explicitly_defined_CA& operator=(const explicitly_defined_CA&) { return *this; } };
    struct explicitly_defined_MC { private:  explicitly_defined_MC           (explicitly_defined_MC&&)      {               } };
    struct explicitly_defined_MA { private:  explicitly_defined_MA& operator=(explicitly_defined_MA&&)      { return *this; } };
    struct explicitly_defined_DD { private: ~explicitly_defined_DD() {} };

}


