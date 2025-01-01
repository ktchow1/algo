#include<iostream>
#include<cstdint>
#include<limits>
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<queue>
#include<math.h>

// Problems
// 1. redundant calculation - not every state are valid in each iteration
// 2. possibility of infinite loop - cannot stop when there is no sum equals to target 
//
enum class optimization : std::uint8_t
{
    min, 
    max
};

template<optimization OP, typename K, typename V>
bool euler_update(std::unordered_map<K,V>& state, const K& key, const V& value)
{
    if (auto iter=state.find(key); iter!=state.end())
    {
        if constexpr(OP == optimization::min)
        {
            if (iter->second > value)
            {
                iter->second = value; 
                return true;
            }
        }
        else
        {
            if (iter->second < value)
            {
                iter->second = value; 
                return true;
            }
        }
    }
    else
    {
        state[key] = value;
        return true;
    }
    return false;
}

std::uint32_t coin_change(std::unordered_set<std::uint32_t>& coins, std::uint32_t target)
{
    std::unordered_map<std::uint32_t, std::uint32_t> states;
    states[0] = 0;

    while(true)
    {
        std::unordered_map<std::uint32_t, std::uint32_t> new_states = states;
        for(const auto& state:states)
        {
            for(const auto& coin:coins)
            {
                auto new_amount = state.first  + coin;
                auto new_count  = state.second + 1;
                if (new_amount == target) return new_count;

                euler_update<optimization::min>(new_states, new_amount, new_count);
            }
        }
        states = std::move(new_states);
    }
    return std::numeric_limits<std::uint32_t>::max();
}

// Optimized version (much faster than the previous approach)
// 1. Value function for each state does not move, we keep update on it.
// 2. Maintain a queue of latest updated states, no need to consider irrelevant states.
//
std::uint32_t coin_change_optimized(std::unordered_set<std::uint32_t>& coins, std::uint32_t target)
{
    std::unordered_map<std::uint32_t, std::uint32_t> states; // amount and count
    states[0] = 0;

    std::queue<std::uint32_t> q;
    q.push(0);

    while(!q.empty())
    {
        std::uint32_t amount = q.front();
        std::uint32_t count  = states[amount];
        q.pop();
        
        for(const auto& coin:coins)
        {
            auto new_amount = amount + coin;
            auto new_count  = count  + 1;
            if (new_amount == target) return new_count;

            if (euler_update<optimization::min>(states, new_amount, new_count) && new_amount < target)
            {
                q.push(new_amount);
            }
        }
    }
    return std::numeric_limits<std::uint32_t>::max();
}
  
// See remark for the changed lines as compared to coin-change problem
std::uint32_t knapsack_optimized(std::unordered_map<std::uint32_t, std::uint32_t>& items, std::uint32_t limit)
{
    std::unordered_map<std::uint32_t, std::uint32_t> states; // weight and value
    states[0] = 0;

    std::queue<std::uint32_t> q;
    q.push(0);

    while(!q.empty())
    {
        std::uint32_t weight = q.front();
        std::uint32_t value  = states[weight];
        q.pop();
        
        for(const auto& item:items)
        {
            auto new_weight = weight + item.first;
            auto new_value  = value  + item.second; // <--- change 1
                                                    // <--- change 2

            if (euler_update<optimization::max>(states, new_weight, new_value) && new_weight < limit) // <--- change 3 (min becomes max)
            {
                q.push(new_weight);
            }
        }
    }

    // <--- change 4
    std::uint32_t ans = 0;
    for(const auto& x:states) 
    {
        if (ans < x.second) ans = x.second;
    }
    return ans;
}   

// Unlike what the note described, equal partition can be done in two dimensional scan
auto equal_partition(const std::vector<std::uint32_t>& vec) 
{
    std::unordered_map<std::uint32_t, std::vector<std::uint32_t>> map; // sum and subset-giving-the-sum
    map[0] = std::vector<std::uint32_t>{};

    for(const auto& x:vec)
    {
        std::unordered_map<std::uint32_t, std::vector<std::uint32_t>> new_map = map;
        for(auto& y:map)
        {
            std::uint32_t sum = x + y.first;
            if (auto iter = new_map.find(sum); iter!=new_map.end())
            {
                // do nothing
            }
            else
            {
                y.second.push_back(x);
                new_map[sum] = std::move(y.second);
            }
        }
        map = std::move(new_map);
    }

    // Find nearest sum to half of sum(vec)
    double half_sum_vec = 0;
    for(const auto& x:vec) 
    {
        half_sum_vec += x;
    }
    half_sum_vec /= 2;

    double min_dist = std::numeric_limits<double>::max();
    auto min_iter=map.begin();
    for(auto iter=map.begin(); iter!=map.end(); ++iter)
    {
        double dist = fabs(iter->first - half_sum_vec);
        if (min_dist > dist)
        {
            min_dist = dist;
            min_iter = iter;
        }
    }
    return *min_iter; // This is a pair, 1st item is partition sum, 2nd item is vector.
}
  
