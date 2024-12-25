#pragma once
#include<cstdint>
#include<limits>
#include<stack>
#include<queue>


namespace alg
{
    template<typename C0, typename C1>
    requires std::is_same_v<typename C0::value_type, typename C1::value_type>
    class container_comparator
    {
    public:
        // ******************************************** //
        // *** Pass member pointer as concrete type *** //
        // ******************************************** //
        void apply0(void (C0::* fp0)(), 
                    void (C1::* fp1)())
        {
            (m_container0.*fp0)();
            (m_container1.*fp1)();
        } 

        template<typename T>
        void apply0(void (C0::* fp0)(const T&), 
                    void (C1::* fp1)(const T&),
                    const T& x)
        {
            (m_container0.*fp0)(x); // BUG : Dont forget the bracket (mem.*fp)(x)
            (m_container1.*fp1)(x);
        }

        // ******************************************** //
        // *** Pass member pointer as template type *** //
        // ******************************************** //
        template<typename FP0, typename FP1>
        void apply1(FP0 fp0, FP1 fp1)
        {
            (m_container0.*fp0)();
            (m_container1.*fp1)();
        } 
        
        template<typename FP0, typename FP1, typename T>
        void apply1(FP0 fp0, FP1 fp1, const T& x)
        {
            (m_container0.*fp0)(x);
            (m_container1.*fp1)(x);
        } 

        template<typename FP0, typename FP1>
        bool compare(FP0 fp0, FP1 fp1)
        {
            if (m_container0.size() != m_container1.size()) return false;
            while (!m_container0.empty())
            {
                if (m_container1.empty()) return false;
                if((m_container0.*fp0)() !=
                   (m_container1.*fp1)())
                {
                    return false;
                }
                m_container0.pop();
                m_container1.pop();
            }
            if (!m_container1.empty()) return false;
            return true;
        }

    private:
        C0 m_container0;
        C1 m_container1;
    };

    template<typename C> 
    requires std::is_same_v<typename C::value_type, std::int32_t>
    class stack_with_min_comparator
    {
    public:


    private:
        std::stack<std::int32_t> m_stack0_value;
        std::stack<std::int32_t> m_stack0_min;
        C                        m_stack1;
    };
}

namespace alg
{
    template<typename T>
    class queue
    {
    public:
        using value_type = T;

        void push(const T& x)
        {
            m_stack0.push(x);
        }

        void pop()
        {
            migrate();
            m_stack1.pop();
        }

        const T& front() const noexcept
        {
            migrate();
            return m_stack1.top();
        }

        void clear()
        {
            m_stack0.clear();
            m_stack1.clear();
        }

        std::uint32_t size() const noexcept
        {
            return m_stack0.size() + m_stack1.size();
        }

        bool empty() const noexcept
        {
            return m_stack0.empty() && m_stack1.empty();
        }

    private:
        void migrate() const noexcept
        {
            if (m_stack1.empty())
            {
                while(!m_stack0.empty())
                {
                    m_stack1.push(m_stack0.top());
                    m_stack0.pop();
                }
            }
        }

    private:
        mutable std::stack<T> m_stack0;  // always for pushing
        mutable std::stack<T> m_stack1;  // always for popping
    };
}

namespace alg
{
    template<typename T>
    class stack
    {
    public:
        using value_type = T;

        void push(const T& x)
        {
            if (m_active_queue0)
            {
                m_queue0.push(x);
            }
            else
            {
                m_queue1.push(x);
            }
        }

        void pop()
        {
            migrate();
            if (m_active_queue0) 
            {
                m_queue0.pop();
                m_active_queue0 = false;
            }
            else 
            {
                m_queue1.pop();
                m_active_queue0 = true;
            }
        }

        const T& top() const noexcept
        {
            migrate();
            if (m_active_queue0)
            {
                return m_queue0.front();
            }
            else 
            {
                return m_queue1.front();
            }
        }

        void clear()
        {
            m_active_queue0 = true;
            m_queue0.clear();
            m_queue1.clear();
        }

        std::uint32_t size() const noexcept
        {
            return m_queue0.size() + m_queue1.size();
        }

        bool empty() const noexcept
        {
            return m_queue0.empty() && m_queue1.empty();
        }

    private:
        void migrate() const noexcept
        {
            if (m_active_queue0)
            {
                while(m_queue0.size() > 1)
                {
                    m_queue1.push(m_queue0.front());
                    m_queue0.pop();
                }
            }
            else
            {
                while(m_queue1.size() > 1)
                {
                    m_queue0.push(m_queue1.front());
                    m_queue1.pop();
                }
            }
        }

    private:
        mutable bool m_active_queue0 = true;
        mutable std::queue<T> m_queue0; // should contains >= 1 item after migrating to queue 1
        mutable std::queue<T> m_queue1; // should contains >= 1 item after migrating to queue 0
    };
}

namespace alg
{
}

namespace alg
{
    class stack_with_min // for signed int only
    {
    public:
        void push(std::int32_t x)
        {
            if (m_impl.empty())
            {
                m_impl.push(x);
                m_min = x;
            }
            else if (x < m_min)
            {
                m_impl.push(x - (m_min-x));
                m_min = x;
            }
            else
            {
                m_impl.push(x);
            }
        }

        void pop()
        {
            if (m_impl.top() < m_min)
            {
                m_min = m_min + (m_min-m_impl.top());
                m_impl.pop();
            }
            else
            {
                m_impl.pop();
            }
        }

        std::int32_t top() const noexcept
        {
            if (m_impl.top() < m_min)
            {
                return m_min;
            }
            else
            {
                return m_impl.top();
            }
        }
        
        std::int32_t min() const noexcept
        {
            return m_min;
        }

        void clear()
        {
            while(!empty()) pop();
        }

        std::uint32_t size() const noexcept
        {
            return m_impl.size();
        }

        bool empty() const noexcept
        {
            return m_impl.empty();
        }

    private:
        std::stack<std::int32_t> m_impl;
        std::int32_t m_min;
    };
}
