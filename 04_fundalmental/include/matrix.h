#pragma once
#include<iostream>
#include<cstdint>
#include<vector>


namespace alg
{
    template<typename T> 
    constexpr T one = 1;

    template<typename T> 
    constexpr T inf = std::numeric_limits<T>::max();

    template<typename T>
    T add(T x, T y)
    {
        if (x == inf<T> || y == inf<T>) return inf<T>;
        return x + y;
    }
}

namespace alg
{
    template<typename T>
    class matrix
    {
    public:
        matrix(std::uint32_t size_y, 
               std::uint32_t size_x)
               : m_size_y(size_y), 
                 m_size_x(size_x),
                 m_impl(size_y * size_x)
        {
        }

        matrix(std::uint32_t size_y, 
               std::uint32_t size_x,
               T init)
               : m_size_y(size_y), 
                 m_size_x(size_x),
                 m_impl(size_y * size_x, init)
        {
        }

        const T& operator()(std::uint32_t y, std::uint32_t x) const noexcept
        {
            return m_impl[y * m_size_x + x];
        }

        T& operator()(std::uint32_t y, std::uint32_t x) 
        {
            return m_impl[y * m_size_x + x];
        }

        const std::uint32_t size_y() const noexcept
        {
            return m_size_y;
        }

        const std::uint32_t size_x() const noexcept
        {
            return m_size_x;
        }
        
        void debug() const noexcept
        {
            std::cout << "matrix";
            for(std::uint32_t y=0; y!=m_size_y; ++y)
            {
                std::cout << "\n";
                for(std::uint32_t x=0; x!=m_size_x; ++x) 
                {
                    auto temp = operator()(y,x);
                    if (temp == inf<T>)
                         std::cout << "inf, ";
                    else std::cout << temp << ", ";
                }
            }
        }

    private:
        std::uint32_t  m_size_y;
        std::uint32_t  m_size_x;
        std::vector<T> m_impl;        
    };
}
