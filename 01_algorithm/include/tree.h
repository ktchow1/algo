#pragma once
#include<cstdint>
#include<functional>
#include<queue>
#include<stack>
// *************************************************************************** //
// 1. Tree definitions and properties
// -  what is a tree
// -  what is a binary tree
// -  what is a binary complete tree
// -  what is a binary complete sorted tree
// 2. Tree functions 
// -  search 
// -  insert 
// -  traverse : BFS, DFS_pre_order, DFS_in_order, DFS_post_order
// -  depth
// -  balance factor
// -  balance & rotate
// *************************************************************************** //
// Null check 
// * check this_node           <--- less check, more recursion, simpler code
// * check this_node->m_lhs &
//         this_node->m_rhs    <--- more check, less recursion, faster runtime
// *************************************************************************** //
  

namespace alg { namespace avl
{
    template<typename T>
    struct node
    {
        node() : m_value{}, m_lhs{nullptr}, m_rhs{nullptr}
        {
        }

        explicit node(const T& x) : m_value{x}, m_lhs{nullptr}, m_rhs{nullptr}
        {
        }

        T        m_value;
        node<T>* m_lhs;
        node<T>* m_rhs;
    };

    enum class mode : std::uint8_t
    {
        dfs_in_order_recursive,     // recursion
        dfs_pre_order_recursive,    // recursion
        dfs_post_order_recursive,   // recursion
        dfs_in_order_iterative,     // iterative using stack
        dfs_pre_order_iterative,    // iterative using stack
        bfs_iterative               // iterative using queue
    };

    template<typename T>
    class tree
    {
    public:
        node<T>* insert(const T& x)                       { return insert(m_root, x);      } // No emplace version, as we need constructed-T for tree traversal
        node<T>* search(const T& x)                       { return search(m_root, x);      }
        std::uint32_t depth()          const noexcept     { return depth(m_root);          }
        std:: int32_t balance_factor() const noexcept     { return balance_factor(m_root); }

        template<typename F> requires std::invocable<F,T>
        void traverse(const F& fct, const mode& m) 
        {
            if      (m == mode::dfs_in_order_recursive)   dfs_in_order_recursive(m_root, fct); 
            else if (m == mode::dfs_pre_order_recursive)  dfs_pre_order_recursive(m_root, fct);
            else if (m == mode::dfs_post_order_recursive) dfs_post_order_recursive(m_root, fct);
            else if (m == mode::dfs_pre_order_iterative)  dfs_pre_order_iterative(m_root, fct);
            else if (m == mode::dfs_in_order_iterative)   dfs_in_order_iterative(m_root, fct);
            else                                          bfs_iteraive(m_root, fct);
        }

        //      Y                    X
        //     / \    lhs rotate    / \
        //    X   C   <---------   A   Y
        //   / \      --------->      / \
        //  A   B     rhs rotate     B   C

        node<T>* rotate_lhs(node<T>* this_node) 
        {
            if (this_node->m_rhs == nullptr) return this_node; // no rotation

            node<T>* new_root = this_node->m_rhs;
            this_node->m_rhs = new_root->m_lhs;
            new_root->m_lhs = this_node;
            return new_root;
        }

        node<T>* rotate_rhs(node<T>* this_node)
        {
            if (this_node->m_lhs == nullptr) return this_node; // no rotation

            node<T>* new_root = this_node->m_lhs;
            this_node->m_lhs = new_root->m_rhs;
            new_root->m_rhs = this_node;
            return new_root;
        }

    private:
        node<T>* insert(const node<T>* this_node, const T& x) // return new node
        {
            if      (this_node == nullptr) { this_node = new node<T>(x); return this_node; }
            else if (x < this_node->m_value) return insert(this_node->m_lhs, x);
            else if (x > this_node->m_value) return insert(this_node->m_rhs, x);
            else                             return this_node;
        }

        node<T>* search(node<T>* this_node, const T& x)
        {
            if      (this_node == nullptr)   return nullptr;
            else if (x < this_node->m_value) return search(this_node->m_lhs, x);
            else if (x > this_node->m_value) return search(this_node->m_rhs, x);
            else                             return this_node;
        }

        std::uint32_t depth(const node<T>* this_node) const noexcept
        {
            if (this_node) return 1 + std::max(depth(this_node->m_lhs), depth(this_node->m_rhs));
            else           return 0;
        }

        std::int32_t balance_factor(const node<T>* this_node) const noexcept
        {
            if (this_node) return depth(this_node->m_lhs) - depth(this_node->m_rhs);
            else           return 0;
        }

    private:
        template<typename F> requires std::invocable<F,T>
        void dfs_in_order_recursive(node<T>* this_node, const F& fct) 
        {
            if (this_node == nullptr) return;
            
            dfs_in_order_recursive(this_node->m_lhs, fct);
            fct(this_node->m_value); 
            dfs_in_order_recursive(this_node->m_rhs, fct);
        }

        template<typename F> requires std::invocable<F,T>
        void dfs_pre_order_recursive(node<T>* this_node, const F& fct) 
        {
            if (this_node == nullptr) return;
            
            fct(this_node->m_value); 
            dfs_pre_order_recursive(this_node->m_lhs, fct);
            dfs_pre_order_recursive(this_node->m_rhs, fct);
        }

        template<typename F> requires std::invocable<F,T>
        void dfs_post_order_recursive(node<T>* this_node, const F& fct) 
        {
            if (this_node == nullptr) return;
            
            dfs_post_order_recursive(this_node->m_lhs, fct);
            dfs_post_order_recursive(this_node->m_rhs, fct);
            fct(this_node->m_value); 
        }

        template<typename F> requires std::invocable<F,T>
        void dfs_pre_order_iterative(node<T>* this_node, const F& fct) 
        {
            std::stack<node<T>*> s; // there exists nullptr in s
            s.push(this_node);

            while(!s.empty())
            {
                this_node = s.top(); // reuse this_node
                if (this_node) 
                {
                    fct(this_node->m_value);
                    s.push(this_node->m_lhs);
                    s.push(this_node->m_rhs);
                }
                s.pop();
            }
        }

        template<typename F> requires std::invocable<F,T>
        void dfs_in_order_iterative(node<T>* this_node, const F& fct) 
        {
            std::stack<node<T>*> s; // there are no nullptr in s, while this_node can be nullptr

            while(this_node || !s.empty())
            {
                // rule 1 : overtake this_node by lhs child, cache overtaken node as it is the next-to-process
                if (this_node != nullptr) 
                {
                    s.push(this_node);
                    this_node = this_node->m_lhs;
                }
                // rule 2 : pop overtaken node, process it, visit rhs child, immediately goto rule 1
                else
                {
                    this_node = s.top();
                    fct(this_node->m_value);
                    this_node = this_node->m_rhs;
                    s.pop();
                }
            }
        }

        template<typename F> requires std::invocable<F,T>
        void bfs_iterative(node<T>* this_node, const F& fct) 
        {
            std::queue<node<T>*> q; // there exists nullptr in q
            q.push(this_node);

            while(!q.empty())
            {
                this_node = q.front(); // reuse this_node
                if (this_node) 
                {
                    fct(this_node->m_value);
                    q.push(this_node->m_lhs);
                    q.push(this_node->m_rhs);
                }
                q.pop();
            }
        }

    private:
        node<T>* m_root;        
    };
}}


// *********************** //
// *** Tree algorithms *** //
// *********************** //
namespace alg { namespace avl
{
    template<typename T>
    bool is_vec_post_ordered(const std::vector<T>& vec)
    {
        return false;
    }

    template<typename T>
    bool is_avl_tree_sorted(const node<T>* root)
    {
        return false;
    }

    template<typename T>
    node<T>* create_avl_tree_from_sorted_vec(const std::vector<T>& vec)
    {
        return nullptr;
    }

    template<typename T>
    std::pair<node<T>*, node<T>*> create_doubly_list_from_avl_tree(node<T>* root)
    {
        node<T>* head;
        node<T>* tail;
        return std::make_pair(head, tail);
    }

}}