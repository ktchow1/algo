#include<iostream>
#include<cstdint>
#include<cmath>
#include<vector>
#include<unordered_map>
#include<algorithm>


// ******************** //
// *** Maximization *** //
// ******************** //
namespace alg
{
    std::int32_t max_profit(const std::vector<std::int32_t>& vec)
    {
        if (vec.size()<2) return 0;

        std::int32_t sub = vec[1]-vec[0];
        std::int32_t ans = sub;
        for(std::uint32_t n=2; n!=vec.size(); ++n)
        {
            sub = std::max(sub-vec[n-1]+vec[n], vec[n]-vec[n-1]);
            ans = std::max(sub, ans);
        }
        return ans;
    }
}



// *********************************************** //
// *** Maximization - benchmark implementation *** //
// *********************************************** //
namespace alg
{
    std::int32_t max_profit_bmk(const std::vector<std::int32_t>& vec)
    {
        if (vec.size()<2) return 0;

        std::int32_t ans = std::numeric_limits<std::int32_t>::min();
        for(std::uint32_t n=0; n!=vec.size()-1; ++n) 
        {
            for(std::uint32_t m=n+1; m!=vec.size(); ++m) 
            {
                ans = std::max(vec[m]-vec[n], ans);
            }
        }
        return ans;
    }
}



// **************** //
// *** Counting *** //
// **************** //
namespace alg
{
    std::uint32_t count_target_profit(const std::vector<std::int32_t>& vec, std::int32_t target)
    {
        std::unordered_map<std::int32_t, std::uint32_t> hist;
        // unlike counting subseq sum, no need to init : hist[0] = ...

        std::uint32_t ans = 0;
        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {
            if (auto iter=hist.find(vec[n]-target); iter!=hist.end())
            {
                ans += iter->second;
            }
            if (auto iter=hist.find(vec[n]); iter!=hist.end()) 
            {
                ++iter->second;
            } 
            else 
            {
                hist[vec[n]] = 1;
            }
        }
        return ans;
    }

    std::uint32_t count_target_abs_profit(const std::vector<std::int32_t>& vec, std::int32_t target)
    {
        std::unordered_map<std::int32_t, std::uint32_t> hist;
        // unlike counting subseq sum, no need to init : hist[0] = ...
        
        std::uint32_t ans = 0;
        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {
            
            if (auto iter=hist.find(vec[n]-target); iter!=hist.end())
            {
                ans += iter->second;
            }
            if (auto iter=hist.find(vec[n]+target); iter!=hist.end())
            {
                ans += iter->second;
            }
            if (auto iter=hist.find(vec[n]); iter!=hist.end()) 
            {
                ++iter->second;
            } 
            else 
            {
                hist[vec[n]] = 1;
            }
        }
        return ans;
    }
}



// ******************************************* //
// *** Counting - benchmark implementation *** //
// ******************************************* //
namespace alg
{
    std::uint32_t count_target_profit_bmk(const std::vector<std::int32_t>& vec, std::int32_t target)
    { 
        if (vec.size()<2) return 0;

        std::uint32_t ans = 0;
        for(std::uint32_t n=0; n!=vec.size()-1; ++n) 
        {
            for(std::uint32_t m=n+1; m!=vec.size(); ++m) 
            {
               if (vec[m]-vec[n]==target) ++ans;
            }
        }
        return ans;
    }

    std::uint32_t count_target_abs_profit_bmk(const std::vector<std::int32_t>& vec, std::int32_t target)
    {
        if (vec.size()<2) return 0;

        std::uint32_t ans = 0;
        for(std::uint32_t n=0; n!=vec.size()-1; ++n) 
        {
            for(std::uint32_t m=n+1; m!=vec.size(); ++m) 
            {
               if (vec[m]-vec[n]==+target ||
                   vec[m]-vec[n]==-target) ++ans;
            }
        }
        return ans;
    }
}
