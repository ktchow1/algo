#include<iostream>
#include<cstdint>
#include<limits>
#include<queue>
#include<unordered_map>
#include<optional>
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
    std::optional<V> find_target(const std::unordered_map<K,V>& states, const K& key)
    {
        if (auto iter = states.find(key); iter!=states.end())
        {
            return iter->second;
        }
        else return std::nullopt;
    }
}

// ******************************************************************************************************************************** //
// [Concepts]
//
// constraint : state = sum(x[n],p[n])        example : x = coin denomination, state = sum of coins value 
// objective  : value = sum(y[n],p[n])        example : y = 1 for all n,       value = num of coins used (in total)
// decision   : param = p[n]                  example :                        param = num of coin  used (for each type)
//
// Approaches (with speed 4 > 3 > 2 > 1)
// 1. recursive in state-graph
// 2. recursive in subproblem-table           
// 3. iterative in state-graph                iterative version of 1, aka region_growing (cannot be implemented as tabulation)
// 4. iterative in subproblem-table           iterative version of 2, aka tabulation     (cannot be implemented as region growing)
//
//
// ******************************************************************************************************************************** //
// 1. Recursion in state-graph connects a vertex with its neighbours in the same problem size
//
//    f(coins, target) = min[ f(coins, target-coins[0]) + 1,
//                            f(coins, target-coins[1]) + 1,
//                            f(coins, target-coins[2]) + 1,
//                            ... ] 
//
//
// 2. Recursion in subproblem-table connects a cell with its neighbours in different problem size
//
//    f(coins, target) = min[ f(coins, target-coins[0]) + 1,
//                            f(coins.exclude(coins[0]), target) ] 
//
//
// 3. State-graph consisted of : 
// *  vertices  = all possible states (i.e. 0, 1, 2, ..., target)
// *  edges     = between any 2 states, whenever they can be connected by a coin
// *  objective = minimize distance from state_0 to state_target
// *  implement = region_growing (Dijkstra) on the state-graph, represented as std::map<state,value>
//
//
// 4. Subproblem-table is a 2D matrix of :
// *  row[0] = subproblem with coins[0] only
// *  row[1] = subproblem with coins[0] and coins[1]
//    ...
// *  col[0] = subproblem with target = 0 
// *  col[1] = subproblem with target = 1 
//    ...
//
// ******************************************************************************************************************************** //
// BUG.1 and BUG.2
//
// 1. all 4 min_coin_change() algo
// -  involve "std::numeric_limits<T>::max() + 1", which may overflow 
// -  need to replace them with inf<T>, one<T> and add(x,y) 
//
// 2. in knapsack, unlike the constraint in coin change :  
//    state == target            for coin change
//    state <= weight_limit      for knapsack 
//
// Both BUG.1&2 will not be revealed if :
// * there is $1 coin or
// * there is 1kg object in knapsack
//
//
//
// *********************** //
// *** Min coin change *** //
// *********************** //
namespace alg
{  
    std::uint32_t min_coin_change_recursive_in_state(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        std::uint32_t ans = inf<std::uint32_t>;
        for(const auto& x:coins)
        {
            if (x == target) return 1;
            if (x <  target)
            {
                ans = std::min(ans, add(min_coin_change_recursive_in_state(coins, target-x), one<std::uint32_t>)); 
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
                return inf<std::uint32_t>;
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
                return std::min(add(min_coin_change_recursive_in_subprob(coins,     target-coins.back()), one<std::uint32_t>),
                                    min_coin_change_recursive_in_subprob(coins_sub, target));
            }
        }
    } 

    std::uint32_t min_coin_change_iterative_in_state(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        std::unordered_map<std::uint32_t, std::uint32_t> graph;   // graph in region grow
        graph[0] = 0;                                             // bug : must initialize 

        std::queue<std::uint32_t> q;                              // queue in region growstates
        q.push(0);                                                // bug : must initialize 

        while(!q.empty())
        {
            std::uint32_t s_prev = q.front();
            std::uint32_t v_prev = graph[q.front()];
            q.pop();

            // *** for each neighbour *** //
            for(const auto& x:coins)
            {
                std::uint32_t s = s_prev + x;
                std::uint32_t v = v_prev + 1;
                
                if (euler_update<std::less<std::uint32_t>>(graph, s, v) && s <= target) 
                {
                    q.push(s);
                }
            }
        }

        auto ans = find_target(graph, target);
        return (ans? *ans: inf<std::uint32_t>);
    }

    std::uint32_t min_coin_change_iterative_in_subprob(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        // n = matrix.size_y =   {1,2,3,... coins.size}
        // m = matrix.size_x = {0,1,2,3,... target} 
        matrix<std::uint32_t> mat(coins.size(), target+1, inf<std::uint32_t>);
        
        // init 1st row
        for(std::uint32_t m=0; m<=target; ++m) 
        {
            if (m % coins[0] == 0) 
            {
                mat(0,m) = m / coins[0];
            }
        }

        // init 1st col
        for(std::uint32_t n=0; n!=coins.size(); ++n) 
        {
            mat(n,0) = 0;
        }

        // iteration
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
                    mat(n,m) = std::min(mat(n-1,m), add(mat(n,m-coins[n]), one<std::uint32_t>));
                }
            }
        }
        return mat(coins.size()-1, target); 
    }
}


// ************************* //
// *** Count coin change *** //
// ************************* //
// Unlike minimization coin change problem,
// this is a counting coin change problem,
// in which, approach 1,3 do not apply.
//
namespace alg
{   
    std::uint32_t count_coin_change_recursive_in_subprob(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        if (coins.size() == 1)
        {
            if (target % coins[0] == 0) 
            {
                return 1; 
            }
            else 
            {
                return 0;
            }
        }
        else 
        {
            std::vector<std::uint32_t> coins_sub(coins.begin(), --coins.end());
            if (coins.back() > target)
            {
                return count_coin_change_recursive_in_subprob(coins_sub, target); 
            }
            else
            {
                return count_coin_change_recursive_in_subprob(coins,     target-coins.back()) + 
                       count_coin_change_recursive_in_subprob(coins_sub, target);
            }
        }
    } 

    std::uint32_t count_coin_change_iterative_in_subprob(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        matrix<std::uint32_t> mat(coins.size(), target+1, 0); 
        
        // init 1st row
        for(std::uint32_t m=0; m<=target; ++m) 
        {
            if (m % coins[0] == 0) 
            {
                mat(0,m) = 1;
            }
        }

        // init 1st col
        for(std::uint32_t n=0; n!=coins.size(); ++n) 
        {
            mat(n,0) = 1; 
        }

        // iteration
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
                    mat(n,m) = mat(n-1,m) + mat(n,m-coins[n]); 
                }
            }
        }
        return mat(coins.size()-1, target);
    }
}


// **************** //
// *** Knapsack *** //
// **************** //
// for std::vector<std::pair<...>> "objects"
// first  = single object weight
// second = single object value
// 
// for std::unordered_map<...> "graph" 
// key   = total objects weight
// value = total objects value
//
namespace alg
{  

    void knapsack_init(const std::vector<std::pair<std::uint32_t, std::uint32_t>>& objects, auto& graph, auto& queue) 
    {
        // BUG.2 : Need to add more init points, otherwise some states in graph can never be reached
        //
        // we can either (both proved to work in test) :
        // * add all states, i.e. from 0,1,2,...,weight_limit, OR
        // * add all states below min_object_weight only        
        //
        std::uint32_t min_object_weight = inf<std::uint32_t>;
        for(const auto& x:objects)
        {
            if (min_object_weight > x.first)
                min_object_weight = x.first;
        }
        for(std::uint32_t m=0; m!=min_object_weight; ++m)
        {
            graph[m] = 0;
            queue.push(m);
        }
    }

    std::uint32_t knapsack_iterative_in_state(const std::vector<std::pair<std::uint32_t, std::uint32_t>>& objects, std::uint32_t weight_limit)
    {
        std::unordered_map<std::uint32_t, std::uint32_t> graph;
        std::queue<std::uint32_t> q;    
        knapsack_init(objects, graph, q); // <--- BUG.2

        while(!q.empty())
        {
            std::uint32_t s_prev = q.front();
            std::uint32_t v_prev = graph[q.front()];
            q.pop();

            // *** for each neighbour *** //
            for(const auto& x:objects)
            {
                std::uint32_t s = s_prev + x.first;
                std::uint32_t v = v_prev + x.second;
                
                if (euler_update<std::greater<std::uint32_t>>(graph, s, v) && s <= weight_limit) 
                {
                    q.push(s);
                }
            }
        }

        auto ans = find_target(graph, weight_limit);
        return (ans? *ans: 0);
    }

    std::uint32_t knapsack_iterative_in_subprob(const std::vector<std::pair<std::uint32_t, std::uint32_t>>& objects, std::uint32_t weight_limit)
    {
        // n = matrix.size_y =   {1,2,3,... objects.size}
        // m = matrix.size_x = {0,1,2,3,... weight_limit} 
        matrix<std::uint32_t> mat(objects.size(), weight_limit+1, 0);
        
        // init 1st row
        for(std::uint32_t m=0; m<=weight_limit; ++m) 
        {
        //  if (m % objects[0].first == 0) <--- BUG.2, as constraint is : state <= weight_limit, 
        //                                                            NOT state == weight_limit,
        //                                                            we can still put items in knapsack 
        //                                                            even if state % object_weight != 0
            mat(0,m) = (m / objects[0].first) * objects[0].second;
        }

        // init 1st col
        for(std::uint32_t n=0; n!=objects.size(); ++n) 
        {
            mat(n,0) = 0;
        }

        // iteration
        for(std::uint32_t n=1; n!=objects.size(); ++n)
        {
            for(std::uint32_t m=1; m<=weight_limit; ++m)
            {
                if (objects[n].first > m)
                {
                    mat(n,m) = mat(n-1,m);
                }
                else
                {
                    mat(n,m) = std::max(mat(n-1,m), mat(n,m-objects[n].first) + objects[n].second);
                }
            }
        }
        return mat(objects.size()-1, weight_limit);
    }
}


// ******************** //
// *** Job schedule *** //
// ******************** //
namespace alg
{  
}
