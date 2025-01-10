#pragma once
#include<iostream>
#include<cstdint>
#include<vector>


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
               const T& init)
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

    private:
        std::uint32_t  m_size_y;
        std::uint32_t  m_size_x;
        std::vector<T> m_impl;        
    };




}
