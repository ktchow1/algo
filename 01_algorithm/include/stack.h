#pragma once
#include<stack>
#include<queue>


namespace alg
{
    template<typename T>
    class queue_from_stack
    {
    public:
        void push(const T& x)
        {
            m_stack0.push(x);
        }

        void pop()
        {
            migrate();
            m_stack1.pop();
        }

        const T& front()
        {
            migrate();
            m_stack1.top();
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
        void migrate()
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
        std::stack<T> m_stack0;  // always for pushing
        std::stack<T> m_stack1;  // always for popping
    };
}

namespace alg
{
    template<typename T>
    class stack_from_queue
    {
    public:
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
                m_queue1.front();
                m_active_queue0 = true;
            }
        }

        const T& top()
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
        void migrate()
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
        bool m_active_queue0 = true;
        std::queue<T> m_queue0; // should contains >= 1 item after migrating to queue 1
        std::queue<T> m_queue1; // should contains >= 1 item after migrating to queue 0
    };
}

namespace alg
{
}

namespace alg
{
    template<typename T>
    class stack_with_medium
    {
    public:
    private:
    };
}
