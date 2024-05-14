#pragma once

// ***************************************** //
// *** Dynamic Programming - vector game *** //
// ***************************************** //
// 3|2 + 1|2 + 4|5 + 2|3 + 2|3 = 27 
// --------------------------------------------
// 1.0.0 max 2-pt sum-diff
// 1.0.1 max 2-pt sum with equal digit-sum
// 1.0.2 max 2-pt sum of non-attacking rooks
// 1.1.0 count 2-pt sum in sorted vector
// 1.1.1 count K-pt sum in unsorted vector
// 1.2.0 max 2-pt profit
// 1.3.0 count 2-pt target profit
// 1.3.1 count 2-pt target absolute profit
// 1.4.0 max subseq sum 
// 1.4.1 max subseq product
// 1.4.2 max non-contiguous seq sum 
// 1.4.3 max puzzle score 
// 1.5.0 count target subseq sum
// 1.5.1 count divisible subseq sum
// 1.5.2 length target subseq sum
// 1.5.3 count less-than-target subseq sum
// 1.5.4 count less-than-target subseq product
// --------------------------------------------
// 1.6.0 longest non-duplicated substr
// 1.6.1 longest palindrome substr
// 1.7.0 shortest unsorted subseq
// 1.7.1 max rectangle in histogram
// 1.7.2 max pool size
// 1.8.0 min bribe
// 1.8.1 K-th element
// 1.9.0 subset
// 1.9.1 tree layer average 
// 1.9.2 string matching algorithm
// --------------------------------------------
// 
//               max      count
// --------------------------------------------
// 2pt_diff      sub      hist
// subseq_sum    sub      hist + cum
// --------------------------------------------

template<typename T>
struct matrix
{
    matrix(std::uint32_t _size_y, std::uint32_t _size_x) : size_y(_size_y), size_x(_size_x)
    {
        impl.resize(size_y * size_x);
    }

    matrix(std::uint32_t _size_y, std::uint32_t _size_x, const T& init) : size_y(_size_y), size_x(_size_x)
    {
        impl.resize(size_y * size_x);
        for(auto& x:init) x = init;
    }

    const T& operator()(std::uint32_t y, std::uint32_t x) const
    {
        return impl[y * size_x + x];
    }

    T& operator()(std::uint32_t y, std::uint32_t x)
    {
        return impl[y * size_x + x];
    }

    void debug() const noexcept
    {
        std::cout << "\nMatrix";
        for(std::uint32_t n=0; n!=size_y; ++n)
        {
            std::cout << "\n";
            for(std::uint32_t m=0; m!=size_x; ++m)
            {
                std::cout << (*this)(n,m) << " ";
            }
        }
        std::cout << "\n";
    }

    std::uint32_t size_y;
    std::uint32_t size_x;
    std::vector<T> impl;
};
  
template<typename T>
struct tenor
{
    tenor(std::uint32_t _size_z, std::uint32_t _size_y, std::uint32_t _size_x) : size_z(_size_z), size_y(_size_y), size_x(_size_x)
    {
        impl.resize(size_z, size_y * size_x);
    }

    tenor(std::uint32_t _size_z, std::uint32_t _size_y, std::uint32_t _size_x, const T& init) : size_z(_size_z), size_y(_size_y), size_x(_size_x)
    {
        impl.resize(size_z, size_y * size_x);
        for(auto& x:impl) x = init;
    }

    const T& operator()(std::uint32_t z, std::uint32_t y, std::uint32_t x) const
    {
        return impl[z * size_y * size_x + y * size_x + x];
    }

    T& operator()(std::uint32_t z, std::uint32_t y, std::uint32_t x)
    {
        return impl[z * size_y * size_x + y * size_x + x];
    }

    std::uint32_t size_z;
    std::uint32_t size_y;
    std::uint32_t size_x;
    std::vector<T> impl;
};
