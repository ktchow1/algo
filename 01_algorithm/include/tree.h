#pragma once
#include<queue>
#include<stack

namespace alg
{
    template<typename T>
    struct avl_node
    {
        avl_node() : m_value{}, m_lhs{nullptr}, m_rhs{nullptr}
        {
        }

        template<typename...ARGS>
        avl_node(ARGAS&&...args) : m_value{std::forward<ARGS>(args)...}, m_lhs{nullptr}, m_rhs{nullptr}
        {
        }

        T            m_value;
        avl_node<T>* m_lhs;
        avl_node<T>* m_rhs;
    };


    template<typename T>
    class avl_tree
    {
    public:
        std::uint32_t depth() const noexcept
        {
        }

        std::int32_t balance_factor() const noexcept
        {

        }




    private:
        avl_node<T>* m_root_node;        
    };
}
