#pragma once
#include<tree.h>


namespace alg
{
    template<typename T, typename CMP = std::less<T>>
    class heap
    {
    public:
        using value_type = T;

        template<typename...ARGS>
        void push(ARGS&&...args)
        {
            m_impl.emplace_back(std::forward<ARGS>(args)...);
            ascend(size()-1);
        }

        void pop()
        {
            m_impl[0] = m_impl[size()-1];
            m_impl.pop_back();
            descend();
        }

        const T& top() const noexcept
        {
            return m_impl[0];
        }

        std::uint32_t size() const noexcept
        {
            return m_impl.size();
        }

        bool empty() const noexcept
        {
            return m_impl.empty();
        }

        void clear()
        {
            m_impl.clear();
        }

    private:
        void ascend(std::uint32_t n) // ascend element n
        {
            while(n > 0)
            {
                std::uint32_t m = (n-1)/2;
                if (CMP{}(m_impl[n], m_impl[m]))
                {
                    std::swap(m_impl[n], m_impl[m]);
                    n = m;
                }
                else return;
            }
        }

        void descend() // descend element 0 
        {
            std::uint32_t n = 0;
            while(n < size())
            {
                std::uint32_t m0 = 2*n+1;
                std::uint32_t m1 = 2*n+2;
                if (m1 < size())
                {
                    if (CMP{}(m_impl[m0], m_impl[m1]))
                    {
                        if (CMP{}(m_impl[m0], m_impl[n])) { std::swap(m_impl[n], m_impl[m0]); n = m0; }
                        else return;
                    }
                    else
                    {
                        if (CMP{}(m_impl[m1], m_impl[n])) { std::swap(m_impl[n], m_impl[m1]); n = m1; }
                        else return;
                    }
                }
                else if (m0 < size())
                {
                    if (CMP{}(m_impl[m0], m_impl[n])) { std::swap(m_impl[n], m_impl[m0]); n = m0; }
                    else return;
                }
                else return;
            }
        }

    private:
        std::vector<T> m_impl;
    };
}


namespace alg
{
    // **********************************************************//
    // Implementation of ascent() and descend() is same as above.
    // With extra step to convert index into iterator.
    // **********************************************************//
    template<typename T, typename CMP = std::less<T>>
    class heap_inplace
    {
    public:
        using value_type = T;
        using iter_type = std::vector<T>::iterator;

        heap_inplace(iter_type begin, iter_type end) : m_begin(begin), m_end(end)
        {
            std::uint32_t size = std::distance(begin, end);
            if (size > 0)
            {
                // *** step 1 : push *** //
                for(std::uint32_t n=1; n!=size; ++n)
                {
                    ascend(n);
                }

                // *** step 2 : pop *** //
                for(std::uint32_t n=size-1; n!=0; --n)
                {
                    iter_type iter = m_begin + n;
                    std::swap(*m_begin, *iter);
                    descend(n);
                }
            }
        }

    private:
        void ascend(std::uint32_t n) //ascend element n
        {
            while(n > 0)
            {
                std::uint32_t m = (n-1)/2;
                iter_type iter_n = m_begin + n; 
                iter_type iter_m = m_begin + m; 
                if (CMP{}(*iter_n, *iter_m))
                {
                    std::swap(*iter_n, *iter_m);
                    n = m;
                }
                else return;
            }
        }

        void descend(std::uint32_t new_size) // descend element 0
        {
            std::uint32_t n = 0;
            while(n < new_size)
            {
                std::uint32_t m0  = 2*n+1;
                std::uint32_t m1  = 2*n+2;
                iter_type iter_n  = m_begin + n; 
                iter_type iter_m0 = m_begin + m0; 
                iter_type iter_m1 = m_begin + m1; 
                if (m1 < new_size)
                {
                    if (CMP{}(*iter_m0, *iter_m1))
                    {
                        if (CMP{}(*iter_m0, *iter_n)) { std::swap(*iter_n, *iter_m0); n = m0; }
                        else return;
                    }
                    else
                    {
                        if (CMP{}(*iter_m1, *iter_n)) { std::swap(*iter_n, *iter_m1); n = m1; }
                        else return;
                    }
                }
                else if (m0 < new_size)
                {
                    if (CMP{}(*iter_m0, *iter_n)) { std::swap(*iter_n, *iter_m0); n = m0; }
                    else return;
                }
                else return;
            }
        }

    private:
        iter_type m_begin;
        iter_type m_end;
    };
}


namespace alg
{
    enum class find_mode : std::uint8_t
    {
        iterative,
        recursive,
        recursive_with_path_compression
    };

    // ************************************* //
    // 1. For disjoint_set is a tree :
    // -      represented by parent link
    // -  NOT represented by children link
    //
    // 2. Root of a set is a class-label.
    // ************************************* //
    template<typename T>
    class disjoint_set
    {
    public:
        explicit disjoint_set(find_mode mode) : m_mode(mode)
        {
        }

        const T& find(const T& x)
        {
            auto iter = m_parent.find(x);
            if (iter == m_parent.end()) 
            {
                m_parent[x] = x;
            }
            if      (m_mode == find_mode::iterative)  return find_root_iterative(x);
            else if (m_mode == find_mode::recursive)  return find_root_recursive(x);
            else                                      return find_root_recursive_with_path_compression(x);
        }

        void union_of(const T& x, const T& y)
        {
            auto root_x = find(x);
            auto root_y = find(y);
            m_parent[root_x] = root_y;
        }

        bool is_same_set(const T& x, const T& y) 
        {
            auto root_x = find(x);
            auto root_y = find(y);
            return root_x == root_y;
        }

    private:
        const T& find_root_iterative(const T& x) 
        {
            T temp = x;

            auto iter = m_parent.find(temp); 
            while(iter->second != temp)
            {
                temp = iter->second; 
                iter = m_parent.find(temp); 
            }
            return iter->second;
        }

        const T& find_root_recursive(const T& x)
        {
            auto iter = m_parent.find(x); 
            if (iter->second == x) return x;
            return find_root_recursive(iter->second);
        }

        const T& find_root_recursive_with_path_compression(const T& x)
        {
            auto iter = m_parent.find(x); 
            if (iter->second == x) return x;
            return iter->second = find_root_recursive(iter->second);
        }

    private:
        const find_mode m_mode;
        std::unordered_map<T,T> m_parent; 
    };
}


namespace alg
{
    template<typename V> // K = std::string
    class prefix_tree
    {
    public:
    private:
    };
}



