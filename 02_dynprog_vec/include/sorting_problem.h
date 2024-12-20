#include<iostream>
#include<vector>
#include<algorithm>


namespace alg
{  
    // *************************** //
    // Using combination of :
    // 1. inner loop - quick sort 
    // 2. outer loop - bisection
    // *************************** //
    std::uint32_t order_statistics(const std::vector<std::uint32_t>& vec, std::uint32_t kth_order)
    {
        if (kth_order >= vec.size()) return 0;

        std::vector<std::uint32_t> v(vec);
        std::uint32_t lower = 0;
        std::uint32_t upper = v.size()-1;
        while(lower != upper)
        {
            std::uint32_t i = lower;
            std::uint32_t j = upper;
            while(i!=j)
            {
                if (v[i] <= v[j])
                {
                    --j;
                }
                else
                {
                    std::uint32_t temp = v[i];
                    v[i] = v[j];
                    v[j] = v[i+1];
                    v[i+1] = temp;
                    ++i;
                }
            }
            if (i < kth_order)
            {
            //  lower = i;   // BUG : infinity loop, as v[i] is in correct position
                lower = i+1; // Will lower > upper? No, since i < kth_order
            }
            else if (i > kth_order)
            {
            //  upper = i;   // BUG : infinity loop, as v[i] is in correct position
                upper = i-1; // Will lower > upper? No, since i > kth_order
            }
            else return v[i];
        }
        return v[lower];
    }

    // *********************************** //
    // Using : 
    // 1. bubble sort
    // 2. logic to skip useless comparison
    // *********************************** //
    std::uint32_t min_number_adjacent_swap(const std::vector<std::uint32_t>& vec)
    {
        std::uint32_t ans;
        return ans;
    }
}


// ******************************** //
// *** Benchmark implementation *** //
// ******************************** //
namespace alg
{
    std::uint32_t order_statistics_bmk(const std::vector<std::uint32_t>& vec, std::uint32_t kth_order)
    {
        std::vector<std::uint32_t> sorted_vec(vec);
        std::sort(sorted_vec.begin(), sorted_vec.end());
        return sorted_vec[kth_order];
    }

    std::uint32_t min_number_adjacent_swap_bmk(const std::vector<std::uint32_t>& vec)
    {
        std::uint32_t ans;
        return ans;
    }
}
