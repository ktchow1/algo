#pragma once
#include<cstdint>
#include<vector>


namespace alg
{
    template<typename T>
    class heap
    {
    public:
        using value_type = T;

        template<typename...ARGS>
        void push(ARGS&&...args)
        {
            m_impl.emplace_back(std::forward<ARGS>(args)...);
            asccend(size()-1);
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
        void ascend(std::uint32_t n)
        {
            while(n>0)
            {
                std::uint32_t m = (n-1)/2;
                if (m_impl[n] < m_impl[m])
                {
                    std::swap(m_impl[n], m_impl[m]);
                    n = m;
                }
                else return;
            }
        }

        void descend()
        {
            std::uint32_t n = 0;
            while(n < size())
            {
                std::uint32_t m0 = 2*n+1;
                std::uint32_t m1 = 2*n+2;

                if (m1 < size())
                {
                    if (m_impl[m0] < m_impl[m1])
                    {
                        if (m_impl[n] < m_impl[m0])
                        {
                            std::swap(m_impl[n], m_impl[m0]);
                            n = m0;
                        }
                        else return;
                    }
                    else
                    {
                        if (m_impl[n] < m_impl[m1])
                        {
                            std::swap(m_impl[n], m_impl[m1]);
                            n = m1;
                        }
                        else return;
                    }
                }
                else if (m0 < size())
                {
                    if (m_impl[n] > m_impl[m0])
                    {
                        std::swap(m_impl[n], m_impl[m0]);
                        n = m0;
                    }
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
    template<typename T>
    class disjoint_set
    {
    public:
    private:
    };
}



