#include<iostream>
#include<cstdint>
#include<limits>
#include<queue>
#include<unordered_map>
#include<matrix.h>


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
    std::uint32_t min_coin_change_recursive_in_state(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        std::uint32_t ans = std::numeric_limits<std::uint32_t>::max();
        for(const auto& x:coins)
        {
            if (x == target) return 1;
            if (x <  target)
            {
                ans = std::min(ans, 1 + min_coin_change_recursive_in_state(coins, target-x)); 
            }
        }
        return ans;
    }
  
    std::uint32_t min_coin_change_recursive_in_subprob(const std::vector<std::uint32_t>& coins, std::uint32_t target)
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
            std::vector<std::uint32_t> coins_sub(coins.begin(), --coins.end());
            if (coins.back() > target)
            {
                return min_coin_change_recursive_in_subprob(coins_sub, target);
            }
            else
            {
                return std::min(min_coin_change_recursive_in_subprob(coins,     target-coins.back()) + 1,
                                min_coin_change_recursive_in_subprob(coins_sub, target));
            }
        }
    } 

    std::uint32_t min_coin_change_iterative_in_state(const std::vector<std::uint32_t>& coins, std::uint32_t target)
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

    std::uint32_t min_coin_change_iterative_in_subprob(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        // n = matrix.y =   {1,2,3,... coins.size}
        // m = matrix.x = {0,1,2,3,... target} 
        matrix<std::uint32_t> mat(coins.size(), target+1, std::numeric_limits<std::uint32_t>::max());
        
        // for 1st row
        for(std::uint32_t m=0; m<=target; ++m) 
        {
            if (m % coins[0] == 0) 
            {
                mat(0,m) = m / coins[0];
            }
        }
        // for 1st col
        for(std::uint32_t n=0; n!=coins.size(); ++n) 
        {
            mat(n,0) = 0;
        }

        for(std::uint32_t n=1; n!=coins.size(); ++n)
        {
            for(std::uint32_t m=1; m<=target; ++m)
            {
                if (coins[n] > m)
                {
                    mat(n,m) = mat(n-1,m);
                }
                else
                {
                    mat(n,m) = std::min(mat(n-1,m), mat(n,m-coins[n]) + 1);
                }
            }
        }
        return mat(coins.size()-1, target);
    }
/*
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
    } */
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
