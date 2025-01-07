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


// *********************** //
// *** Min coin change *** //
// *********************** //
// Notations
// key   = sum(k[n],w[n]) <--- constraint, coin value in total 
// value = sum(v[n],w[n]) <--- objective,  coin count in total
// param = w[n]           <--- decision,   coin count for each type
//
namespace alg
{  
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

            // *** region grow to neighbour *** //
            for(const auto& x:coins)
            {
                std::uint32_t k = k_prev + x;
                std::uint32_t v = v_prev + 1;
                
                // *** region grow stop criteria *** //
                if (euler_update<std::less<std::uint32_t>>(states, k, v) && v <= target)
                {
                    q.push(k);
                }
            }
        }
        return find_target(states, target);
    }

    std::uint32_t min_coin_change_iterative_in_subprob(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        // 1. no region grow
        // 2. use 2 maps to induce :
        //    from subproblem of size n 
        //      to subproblem of size n+1
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
                    
                    // *** stop criteria *** //
                    if (v <= target)
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

    std::uint32_t min_coin_change_recursive(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        bool any_coin_less_than_target = false;
        for(const auto& x:coins)
        {
            if (x == target) return 1;
            if (x <  target) any_coin_less_than_target = true; 
        }

        // *** recursion *** //
        std::uint32_t ans = std::numeric_limits<std::uint32_t>::max();
        if (any_coin_less_than_target) 
        {
            for(const auto& x:coins)
            {
                if (x < target) // BUG : missing this check will result in infinity loop
                {
                    ans = std::min(ans, 1 + min_coin_change_recursive(coins, target-x)); 
                }
            }
            return ans;
        }
        else return ans;
    }
}


// **************** //
// *** Knapsack *** //
// **************** //
// objects[weight] = value
//
namespace alg
{ /* 
    std::uint32_t knapsack_iterative_in_time(const std::unordered_map<std::uint32_t, std::uint32_t>& objects, std::uint32_t weight_limit)
    {
        std::unordered_map<std::uint32_t, std::uint32_t> states;  
        states[0] = 0;  

        std::queue<std::uint32_t> q; 
        q.push(0);     

        while(!q.empty())
        {
            std::uint32_t k_prev = q.front();
            std::uint32_t v_prev = states[q.front()];
            q.pop();

            // *** region grow to neighbour *** //
            for(const auto& x:coins)
            {
                std::uint32_t k = k_prev + x;
                std::uint32_t v = v_prev + 1;
                
                // *** region grow stop criteria *** //
                if (euler_update<std::less<std::uint32_t>>(states, k, v) && v <= target)
                {
                    q.push(k);
                }
            }
        }
        return find_target(states, target);
    } */
/*
    std::uint32_t knapsack_iterative_in_subprob(const std::unordered_map<std::uint32_t, std::uint32_t>& objects, std::uint32_t weight_limit)
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
                    
                    // *** stop criteria *** //
                    if (v <= target)
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
    } */
}


// ******************** //
// *** Job schedule *** //
// ******************** //
namespace alg
{  
}
