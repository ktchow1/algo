#include<iostream>
#include<cstdint>
#include<vector>
#include<algorithm>


// Since : 
// n >= vec[n]-2
// m >= vec[m]-2 
//
// if we need to check : vec[n] < vec[m], then m must fulfill
// 
// m >= vec[m]-2 > vec[n]-2
//
std::int32_t num_of_bribes(const std::vector<std::uint32_t>& vec_original_position)
{
    // Step1 : check n >= vec_original_position[n] - 2
    for(std::uint32_t n=0; n!=vec_original_position.size(); ++n) 
    {
        if (n+2 < vec_original_position[n]) return -1; // instead of checking n < vec_...[n]-2, as the latter may underflow 
    } 

    // Step2 : count number of swap
    std::int32_t ans=0;
    for(std::uint32_t n=0; n!=vec_original_position.size(); ++n)
    {
        std::uint32_t m0 = 0;
        if (vec_original_position[n] > 2)
        {
            m0 = vec_original_position[n] - 2;
        }
        for(std::uint32_t m=m0; m!=n; ++m) // for m<n
        {
            if (vec_original_position[n]<vec_original_position[m]) ++ans;
        }
    }
    return ans;
}

std::int32_t num_of_bribes_not_optimized0(const std::vector<std::uint32_t>& vec_original_position)
{
    // Step2 : count number of swap
    std::int32_t ans=0;
    for(std::uint32_t n=0; n!=vec_original_position.size(); ++n)
    {
        for(std::uint32_t m=0; m!=n; ++m) // for m<n
        {
            if (vec_original_position[n]<vec_original_position[m]) ++ans;
        }
    }
    return ans;
}

std::int32_t num_of_bribes_not_optimized1(const std::vector<std::uint32_t>& vec_original_position)
{
    // Step2 : count number of swap
    std::int32_t ans=0;
    for(std::uint32_t n=0; n!=vec_original_position.size(); ++n)
    {
        for(std::uint32_t m=n+1; m!=vec_original_position.size(); ++m) // for m>n
        {
            if (vec_original_position[n]>vec_original_position[m]) ++ans;
        }
    }
    return ans;
}

// ************ //
// *** Test *** //
// ************ //

std::vector<std::uint32_t> gen_random_bribes(std::uint32_t length)
{
    std::vector<std::uint32_t> ans;
    for(std::uint32_t n=0; n!=length; ++n)
    {
        ans.push_back(n);
    }

    std::uint32_t N=rand()%length;
    for(std::uint32_t n=0; n!=N; ++n)
    {
        auto temp = rand()%3;
        if (temp==1 && n>=1)
        {
            std::swap(ans[n],ans[n-1]);
        }
        else if (temp==2 && n>=2)
        {
            std::swap(ans[n],ans[n-1]);
            std::swap(ans[n-1],ans[n-2]);
        }
    }
    return ans;
}

