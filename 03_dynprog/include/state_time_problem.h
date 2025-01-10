#include<iostream>
#include<cstdint>
#include<limits>
#include<queue>
#include<unordered_map>


// ***************************************** //
// CMP is not for unordered_map<K,V>
// CMP = std::less<V>    for minimization
// CMP = std::greater<V> for maximization
// ***************************************** //
namespace alg
{  
    template<typename CMP, typename K, typename V>
    bool euler_update(std::unordered_map<K,V>& states, const K& key, const V& value)
    {   
        if (auto iter = states.find(key); iter != states.end())
        {
            if (CMP{}(value, iter->second))
            {
                iter->second = value;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            states[key] = value;
            return true;
        }
    }

    template<typename K, typename V>
    V find_target(const std::unordered_map<K,V>& states, const K& key)
    {
        if (auto iter = states.find(key); iter!=states.end())
        {
            return iter->second;
        }
        else return std::numeric_limits<V>::max();
    }
}


// *** Idea *** //
//
// Notations
// key   = sum(k[n],w[n]) <--- constraint, coin value in total 
// value = sum(v[n],w[n]) <--- objective,  coin count in total
// param = w[n]           <--- decision,   coin count for each type
//
//
// *********************** //
// *** Min coin change *** //
// *********************** //
namespace alg
{  
    std::uint32_t min_coin_change_slow_recursive(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        std::uint32_t ans = std::numeric_limits<std::uint32_t>::max();
        for(const auto& x:coins)
        {
            if (x == target) return 1;
            if (x <  target)
            {
                ans = std::min(ans, 1 + min_coin_change_slow_recursive(coins, target-x)); 
            }
        }
        return ans;
    }
  
    std::uint32_t min_coin_change_fast_recursive(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        if (coins.size() == 1)
        {
            if (target % coins[0] == 0) 
            {
                return target / coins[0];
            }
            else 
            {
                return std::numeric_limits<std::uint32_t>::max();
            }
        }
        else 
        {
            std::vector<std::uint32_t> coins_sub(coins);
            coins_sub.pop_back();

            if (coins.back() > target)
            {
                return min_coin_change_fast_recursive(coins_sub, target);
            }
            else
            {
                return std::min(min_coin_change_fast_recursive(coins,     target-coins.back()) + 1,
                                min_coin_change_fast_recursive(coins_sub, target));
            }
        }
    } 

    // ****************************************************************************** //
    // Consider region growing in graph :
    // 1. vertex = {key}          with target                as destination vertex
    // 2. vertex = {key x time}   with target (for all time) as destination vertices
    //
    // Both graphs has edge connecting vertices, with cost = 1, find shortest path.
    // Both graphs work, with same implementation.
    // ****************************************************************************** //
    std::uint32_t min_coin_change_iterative_in_time(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        std::unordered_map<std::uint32_t, std::uint32_t> states;  // image in region grow
        states[0] = 0;                                            // bug : must initialize 

        std::queue<std::uint32_t> q;                              // queue in region grow
        q.push(0);                                                // bug : must initialize 

        while(!q.empty())
        {
            std::uint32_t k_prev = q.front();
            std::uint32_t v_prev = states[q.front()];
            q.pop();

            // *** for each neighbour *** //
            for(const auto& x:coins)
            {
                std::uint32_t k = k_prev + x;
                std::uint32_t v = v_prev + 1;
                
                if (euler_update<std::less<std::uint32_t>>(states, k, v) && k <= target) 
                {
                    q.push(k);
                }
            }
        }
        return find_target(states, target);
    }

    // *********************************** //
    // 1. no region grow
    // 2. use 2 maps to induce :
    //    from subproblem with 1 coin 
    //      to subproblem with 2 coins ...
    // *********************************** //
    std::uint32_t min_coin_change_iterative_in_subprob(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        std::unordered_map<std::uint32_t, std::uint32_t> states;  
        states[0] = 0;        

        for(const auto& x:coins)
        {
            std::unordered_map<std::uint32_t, std::uint32_t> next_states(states);  
            for(const auto& [k_prev, v_prev] : states)
            {
                std::uint32_t n = 1;
                while(true)
                {
                    std::uint32_t k = k_prev + n * x;
                    std::uint32_t v = v_prev + n;
                    
                    if (k <= target) 
                    {
                        euler_update<std::less<std::uint32_t>>(next_states, k, v);
                    }
                    else break;
                    ++n;
                }
            }
            states = std::move(next_states);
        }
        return find_target(states, target);
    }

}


// ************************* //
// *** Count coin change *** //
// ************************* //
// Counting instead of minimization 
//
namespace alg
{   /* 
    std::uint32_t count_coin_change_iterative_in_subprob(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
    } */
}


// **************** //
// *** Knapsack *** //
// **************** //
// objects[weight] = value
//
namespace alg
{   /* 
    std::uint32_t knapsack_iterative_in_time(const std::unordered_map<std::uint32_t, std::uint32_t>& objects, std::uint32_t weight_limit)
    {
    } 
  
    std::uint32_t knapsack_iterative_in_subprob(const std::unordered_map<std::uint32_t, std::uint32_t>& objects, std::uint32_t weight_limit)
    {
    } */
}


// ******************** //
// *** Job schedule *** //
// ******************** //
namespace alg
{  
}
