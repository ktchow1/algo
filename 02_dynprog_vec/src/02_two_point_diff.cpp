#include<iostream>
#include<vector>
#include<unordered_map>

std::int32_t max_profit(const std::vector<std::int32_t>& vec)
{
    if (vec.size()<2) return 0;
    std::int32_t msp = vec[1]-vec[0];
    std::int32_t ans = msp;

    for(std::uint32_t n=2; n!=vec.size(); ++n)
    {
        msp = std::max(msp-vec[n-1]+vec[n], vec[n]-vec[n-1]);
        ans = std::max(ans, msp);
    }
    return ans;
}

std::uint32_t count_target_profit(const std::vector<std::int32_t>& vec, std::int32_t target)
{
    std::unordered_map<std::int32_t, std::uint32_t> map;
    std::uint32_t count = 0;

    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        // step 1
        auto iter = map.find(vec[n]-target);
        if (iter != map.end())
        {
            count += iter->second;
        }
        
        // step 2
        iter = map.find(vec[n]);
        if (iter != map.end()) 
        {
            ++iter->second;
        } 
        else 
        {
            map[vec[n]] = 1;
        }
    }
    return count;
}

std::uint32_t count_target_abs_profit(const std::vector<std::int32_t>& vec, std::int32_t target)
{
    std::unordered_map<std::int32_t, std::uint32_t> map;
    std::uint32_t count = 0;

    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        // step 1A
        auto iter = map.find(vec[n]-target);
        if (iter != map.end())
        {
            count += iter->second;
        }
        
        // step 1B
        iter = map.find(vec[n]+target);
        if (iter != map.end())
        {
            count += iter->second;
        }

        // step 2
        iter = map.find(vec[n]);
        if (iter != map.end()) 
        {
            ++iter->second;
        } 
        else 
        {
            map[vec[n]] = 1;
        }
    }
    return count;
}
