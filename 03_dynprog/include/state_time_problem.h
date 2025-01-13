#include<iostream>
#include<cstdint>
#include<limits>
#include<queue>
#include<set>
#include<map>
#include<unordered_map>
#include<optional>

// from alg
#include<matrix.h>
#include<utility.h>


// ***************************************** //
// CMP is not for unordered_map<K,V>
// CMP = std::less<V>    for minimization
// CMP = std::greater<V> for maximization
// ***************************************** //
namespace alg
{  
    // MAP can be std::map or std::unordered_map
    template<typename CMP, typename MAP> 
    bool euler_update(MAP& states, const typename MAP::key_type& key, const typename MAP::mapped_type& value)
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

    template<typename SET> 
    bool euler_update(SET& states, const typename SET::key_type& key)
    {   
        if (auto iter = states.find(key); iter != states.end())
        {
            return false;
        }
        else
        {
            states.insert(key);
            return true;
        }
    }

    template<typename MAP>
    std::optional<typename MAP::mapped_type> find_target(const MAP& states, const typename MAP::mapped_type& key)
    {
        if (auto iter = states.find(key); iter!=states.end())
        {
            return iter->second;
        }
        else return std::nullopt;
    }
}

// ************************************************************************************************* //
// [Concepts]
//                   coin change        knapsack           job schedule       equal partition        
// --------------------------------------------------------------------------------------------------
//            x      coin value         obj weight         task workload      num  
//            y      1                  obj value          task profit        num 
//      param p      coin num           obj num            task done (0/1)    num picked (0/1)
//  prob size N     #coin type         #obj type          #task type         #num
// 
//      state s      s[N-1] = sum(x[n], p[n], for n=[0,N-1])
//      value v      v[N-1] = sum(y[n], p[n], for n=[0,N-1])
// 
//  coin change      min v under constraint  s[N-1] == target
//     knapsack      max v under constraint  s[N-1] <= weight_limit
// job schedule      max v under constraints s[n]   <= deadline[n] for all n=[0,N-1]
// eq partition      max v under constraint  s = v  <= sum(num)/2
//
// ************************************************************************************************* //
// Approaches (with speed 4 > 3 > 2 > 1)
// 1. recursive in state_graph
// 2. recursive in state_subproblem_matrix
// 3. iterative in state_graph                implemented as region_growing
// 4. iterative in state_subproblem_matrix    implemented as scanning
//
//
// 1. Recursion in state_graph connects a vertex with its neighbours in the same problem size
//
//    f(coins, target) = min[ f(coins, target-coins[0]) + 1,
//                            f(coins, target-coins[1]) + 1,
//                            f(coins, target-coins[2]) + 1,
//                            ... ] 
//
//
// 2. Recursion in state_subproblem_matrix connects a cell with its neighbours in different problem size
//
//    f(coins, target) = min[ f(coins, target-coins[0]) + 1,
//                            f(coins.exclude(coins[0]), target) ] 
//
//
// 3. State_graph consisted of : 
// *  vertices  = all possible states (i.e. 0, 1, 2, ..., target)
// *  edges     = between any 2 states, whenever they can be connected by a coin
// *  objective = minimize distance from state_0 to state_target
// *  implement = region_growing (Dijkstra) on the state_graph, represented as std::map<state,value>
//
//
// 4. State_subproblem_matrix is a 2D matrix of "states" vs "subproblem size" :
// *  row[0] = subproblem with coins[0] only
// *  row[1] = subproblem with coins[0] and coins[1]
//    ...
// *  col[0] = state 0 as target
// *  col[1] = state 1 as target
//    ...
//
// ************************************************************************************************* //
// Remark 1. For min coin change : 
// - all 4 implementations involve "std::numeric_limits<T>::max() + 1" 
// - overflow will happen, which can be avoided by either :
//   (a) $1 is always in the coin set 
//   (b) using alg::inf<T>, alg::one<T> and alg::add(x,y)
//
// Remark 2. For knapsack :
// - there is a change in constraint
//   state == target       for coin change
//   state <= weight_limit for knapsack 
// - hence at the end of algo, we need to :
//   scan for optimum in state_graph            
//   scan for optimum in state_subproblem_matrix
//  
// Remark 3. For job schedule : 
// - there are constraints on param
//   (a) takes 0/1 only 
//   (b) tasks[i] cannot be done after tasks[j], if i < j
// - to handle these constraints 
//   state_graph             needs to add next_allowed_job as part of the state
//   state_subproblem_matrix needs to modify recursion eq, so that it depends on prev rows only
//
// Remark 4. For equal partition 
// - it is a simiplified version of job schedule 
//   (a) with same constraints on param
//   (b) no constraint on state
//   (c) with value = state 
//
// Remark 5. For box stacking 
// - it is like job schedule, as param = {0,1}
// - state is not a scaler (plus next_allowed_item)
// - state is a 2D vector  (plus next_allowed_item) 
// - hence cannot use state_subproblem_matrix approach
//
// ************************************************************************************************* //


// *********************** //
// *** Min coin change *** //
// *********************** //
namespace alg
{  
    std::uint32_t min_coin_change_recursive_in_graph(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        std::uint32_t ans = inf<std::uint32_t>;
        for(const auto& x:coins)
        {
            if (x == target) return 1;
            if (x <  target)
            {
                ans = std::min(ans, add(min_coin_change_recursive_in_graph(coins, target-x), one<std::uint32_t>)); 
            }
        }
        return ans;
    }
  
    std::uint32_t min_coin_change_recursive_in_matrix(const std::vector<std::uint32_t>& coins, std::uint32_t target)
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
                return min_coin_change_recursive_in_matrix(coins_sub, target);
            }
            else
            {
                return std::min(add(min_coin_change_recursive_in_matrix(coins,     target-coins.back()), one<std::uint32_t>),
                                    min_coin_change_recursive_in_matrix(coins_sub, target));
            }
        }
    } 

    std::uint32_t min_coin_change_iterative_in_graph(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        std::unordered_map<std::uint32_t, std::uint32_t> graph;   // graph for region growing
        graph[0] = 0;                                             // BUG : must initialize 

        std::queue<std::uint32_t> queue;                          // queue for region growing
        queue.push(0);                                            // BUG : must initialize 

        while(!queue.empty())
        {
            std::uint32_t s_prev = queue.front();
            std::uint32_t v_prev = graph[queue.front()];
            queue.pop();

            // for each neighbour
            for(const auto& x:coins)
            {
                std::uint32_t s = s_prev + x;
                std::uint32_t v = v_prev + 1;
                
                // BUG : check constrant before euler update, to ensure all vertices in graph are valid 
                if (s <= target && euler_update<std::less<std::uint32_t>>(graph, s, v)) 
                {
                    queue.push(s);
                }
            }
        }

        auto ans = find_target(graph, target);
        return (ans? *ans: inf<std::uint32_t>);
    }

    std::uint32_t min_coin_change_iterative_in_matrix(const std::vector<std::uint32_t>& coins, std::uint32_t target)
    {
        matrix<std::uint32_t> mat(coins.size(), target+1, inf<std::uint32_t>);
        // where mat(n,m) = min num of coins need to reach state m with coins {0,1,2..,n}
        //              n =   {1,2,3,... coins.size}
        //              m = {0,1,2,3,... target} 
        
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

        // main iteration
        for(std::uint32_t n=1; n!=coins.size(); ++n)
        {
            for(std::uint32_t m=1; m<=target; ++m)
            {
                if (m >= coins[n])
                {
                    mat(n,m) = std::min(mat(n-1,m), add(mat(n,m-coins[n]), one<std::uint32_t>));
                }
                else
                {
                    mat(n,m) = mat(n-1,m);
                }
            }
        }
        return mat(coins.size()-1, target); 
    }
}


// ************************* //
// *** Count coin change *** //
// ************************* //
// In here, approach 1,3 do not work.
//
namespace alg
{   
    std::uint32_t count_coin_change_recursive_in_matrix(const std::vector<std::uint32_t>& coins, std::uint32_t target)
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
                return count_coin_change_recursive_in_matrix(coins_sub, target); 
            }
            else
            {
                return count_coin_change_recursive_in_matrix(coins,     target-coins.back()) + 
                       count_coin_change_recursive_in_matrix(coins_sub, target);
            }
        }
    } 

    std::uint32_t count_coin_change_iterative_in_matrix(const std::vector<std::uint32_t>& coins, std::uint32_t target)
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

        // main iteration
        for(std::uint32_t n=1; n!=coins.size(); ++n)
        {
            for(std::uint32_t m=1; m<=target; ++m)
            {
                if (m >= coins[n])
                {
                    mat(n,m) = mat(n-1,m) + mat(n,m-coins[n]); 
                }
                else
                {
                    mat(n,m) = mat(n-1,m);
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
    std::uint32_t knapsack_iterative_in_graph(const std::vector<std::pair<std::uint32_t, std::uint32_t>>& objects, std::uint32_t weight_limit)
    {
        std::unordered_map<std::uint32_t, std::uint32_t> graph;
        graph[0] = 0;
        
        std::queue<std::uint32_t> queue;    
        queue.push(0);

        while(!queue.empty())
        {
            std::uint32_t s_prev = queue.front();
            std::uint32_t v_prev = graph[queue.front()];
            queue.pop();

            // for each neighbour
            for(const auto& x:objects)
            {
                std::uint32_t s = s_prev + x.first;
                std::uint32_t v = v_prev + x.second;
                
                if (s <= weight_limit && euler_update<std::greater<std::uint32_t>>(graph, s, v)) 
                {
                    queue.push(s);
                }
            }
        }

        // Remark 2. Optimal case may not use all weight_limit
        std::uint32_t ans = 0;
        for(const auto& x:graph)
        {
            if (ans < x.second)
                ans = x.second;
        }
        return ans;
    }

    std::uint32_t knapsack_iterative_in_matrix(const std::vector<std::pair<std::uint32_t, std::uint32_t>>& objects, std::uint32_t weight_limit)
    {
        matrix<std::uint32_t> mat(objects.size(), weight_limit+1, 0);
        // where mat(n,m) = total value of objects obtained on reaching state m with objects {0,1,2..,n}
        //              n =   {1,2,3,... objects.size}
        //              m = {0,1,2,3,... weight_limit} 
        
        // init 1st row
        for(std::uint32_t m=0; m<=weight_limit; ++m) 
        {
            if (m % objects[0].first == 0) 
            {
                mat(0,m) = (m / objects[0].first) * objects[0].second;
            }
        }
        // init 1st col (redundant)

        // main iteration
        for(std::uint32_t n=1; n!=objects.size(); ++n)
        {
            for(std::uint32_t m=1; m<=weight_limit; ++m)
            {
                if (m >= objects[n].first)
                {
                    mat(n,m) = std::max(mat(n-1,m), mat(n,m-objects[n].first) + objects[n].second);
                }
                else
                {
                    mat(n,m) = mat(n-1,m);
                }
            }
        }

        // Remark 2. Optimal case may not use all weight_limit
        std::uint32_t ans = 0;
        for(std::uint32_t m=0; m!=mat.size_x(); ++m)
        {
            if (ans < mat(objects.size()-1, m))
                ans = mat(objects.size()-1, m);
        }
        return ans;
    }// 
}


// ******************** //
// *** Job schedule *** //
// ******************** //
// for std::vector<std::tuple<...>> "tasks"
// get<0> = single task workload
// get<1> = single task profit
// get<2> = single task deadline <--- tasks are ordered in ascending deadline
// 
// for std::map<...> "graph"  
// key.first  = total task workload
// key.second = next  task possible
// value      = total task profit
//
namespace alg
{ 
    std::uint32_t job_schedule_iterative_in_graph(const std::vector<std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>>& tasks)
    {
        std::map<std::pair<std::uint32_t, std::uint32_t>, std::uint32_t> graph; // Todo : use unordered_map (need to define hash)
        graph[{0,0}] = 0;
        
        std::queue<std::pair<std::uint32_t, std::uint32_t>> queue;    
        queue.push({0,0});
  
        while(!queue.empty())
        {
            auto s_prev = queue.front();
            auto v_prev = graph[queue.front()];
            queue.pop();

            // for each neighbour (Remark 3. Ensure no duplicated task. Ensure task in sequence.)
            for(std::uint32_t n=s_prev.second; n!=tasks.size(); ++n)
            {
                std::uint32_t s = s_prev.first + std::get<0>(tasks[n]);
                std::uint32_t v = v_prev       + std::get<1>(tasks[n]);
                std::uint32_t deadline =         std::get<2>(tasks[n]);
                
                if (s <= deadline && euler_update<std::greater<std::uint32_t>>(graph, std::make_pair(s,n+1), v)) 
                {
                    queue.push({s,n+1});
                }
            }
        }
  
        // Remark 2. Optimal case may not use all weight_limit
        std::uint32_t ans = 0;
        for(const auto& x:graph)
        {
            if (ans < x.second)
                ans = x.second;
        }   
        return ans;
    } 
  
    std::uint32_t job_schedule_iterative_in_matrix(const std::vector<std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>>& tasks)
    {
        std::uint32_t hard_deadline = std::get<2>(tasks.back());
        matrix<std::uint32_t> mat(tasks.size(), hard_deadline+1, 0);
        
        // init 1st row (coz main iteration does not include 1st row)
        {
            std::uint32_t workload = std::get<0>(tasks[0]);
            std::uint32_t profit   = std::get<1>(tasks[0]);
            std::uint32_t deadline = std::get<2>(tasks[0]);
            if (workload <= deadline)
            {
                mat(0,workload) = profit; 
            }
        }
        // init 1st col (redundant)

        // main iteration
        for(std::uint32_t n=1; n!=tasks.size(); ++n)
        {
            std::uint32_t workload = std::get<0>(tasks[n]);
            std::uint32_t profit   = std::get<1>(tasks[n]);
            std::uint32_t deadline = std::get<2>(tasks[n]);
            for(std::uint32_t m=1; m<=hard_deadline; ++m)
            {
                if (m >= workload && m <= deadline)
                {
                //  mat(n,m) = std::max(mat(n-1,m), mat(n,  m-workload) + profit); // Remark 3. Ensure no duplicated task.
                    mat(n,m) = std::max(mat(n-1,m), mat(n-1,m-workload) + profit);
                }
                else
                {
                    mat(n,m) = mat(n-1,m);
                }
            }
        }

        // Remark 2. Optimal case may not use all weight_limit
        std::uint32_t ans = 0;
        for(std::uint32_t m=0; m!=mat.size_x(); ++m)
        {
            if (ans < mat(tasks.size()-1, m))
                ans = mat(tasks.size()-1, m);
        }
        return ans;
    } 
}


// *********************** //
// *** Equal partition *** //
// *********************** //
namespace alg
{ 
    std::uint32_t find_half_of_sum(const std::vector<std::uint32_t>& numbers)
    {
        std::uint32_t ans = 0;
        for(const auto& x:numbers)
        {
            ans += x;
        }
        return ans/2;
    }
  
    std::uint32_t equal_partition_iterative_in_graph(const std::vector<std::uint32_t>& numbers)
    {
        std::uint32_t target = find_half_of_sum(numbers);

        std::set<std::pair<std::uint32_t, std::uint32_t>> graph;
        graph.insert({0,0});

        std::queue<std::pair<std::uint32_t, std::uint32_t>> queue; 
        queue.push({0,0});

        while(!queue.empty())
        {
            auto s_prev = queue.front();
            queue.pop();

            // for each neighbour
            for(std::uint32_t n=s_prev.second; n!=numbers.size(); ++n)
            {
                std::uint32_t s = s_prev.first + numbers[n];
                if (s <= target && euler_update(graph, std::make_pair(s,n+1))) 
                {
                    queue.push({s,n+1});
                }
            }
        }
  
        std::uint32_t ans = 0;
        for(const auto& x:graph)
        {
            if (ans < x.first)
                ans = x.first;
        }   
        return ans;
    } 
    
    std::uint32_t equal_partition_iterative_in_matrix(const std::vector<std::uint32_t>& numbers)
    {
        // Todo : should use alg::matrix<bool>, but don't know why alg::matrix<T> does not support T=bool.
        std::uint32_t target = find_half_of_sum(numbers);
        matrix<std::uint32_t> mat(numbers.size(), target+1, 0); 

        // init 1st row
        mat(0,numbers[0]) = 1; 

        // init 1st col
        for(std::uint32_t n=0; n!=numbers.size(); ++n)
        {
            mat(n,0) = 1;
        }

        // main iteration
        for(std::uint32_t n=1; n!=numbers.size(); ++n)
        {
            for(std::uint32_t m=1; m<=target; ++m)
            {
                if (m >= numbers[n])
                {
                    if (mat(n-1,m)            == 1) mat(n,m) = 1;
                    if (mat(n-1,m-numbers[n]) == 1) mat(n,m) = 1;
                }
                else
                {
                    mat(n,m) = mat(n-1,m);
                }
            }
        }

        std::uint32_t ans = 0;
        for(std::uint32_t m=0; m!=mat.size_x(); ++m)
        {
            if (ans < m && mat(numbers.size()-1, m) == 1)
                ans = m;
        }
        return ans;
    } 
}


// ******************** //
// *** Box stacking *** //
// ******************** //
// Let pick from bottom up.
//
namespace alg
{ 
    enum class orientation : std::uint8_t
    {
        x_up,
        y_up,
        z_up
    };

    struct boxes_state
    {
        std::uint32_t m_next_allowed_box; 
        std::uint32_t m_base_min;
        std::uint32_t m_base_max;

        bool is_greater_eq(const box& b, orientation b_ori) const noexcept // assuming each box has x <= y <= z
        {
            if (b_ori == orientation::x_up)
            {
                if (m_base_min < b.m_y) return false;
                if (m_base_max < b.m_z) return false;
            }
            else if (b_ori == orientation::y_up)
            {
                if (m_base_min < b.m_x) return false;
                if (m_base_max < b.m_z) return false;
            }
            else
            {
                if (m_base_min < b.m_x) return false;
                if (m_base_max < b.m_y) return false;
            }
            return true;
        }

        void update(std::uint32_t next_allowed_box, const box& b, orientation b_ori) 
        {
            m_next_allowed_box = next_allowed_box;
            if (b_ori == orientation::x_up)
            {
                m_base_min = b.m_y;
                m_base_max = b.m_z;
            }
            else if (b_ori == orientation::y_up)
            {
                m_base_min = b.m_x;
                m_base_max = b.m_z;
            }
            else
            {
                m_base_min = b.m_x;
                m_base_max = b.m_y;
            }
        } 
    };

    struct stack_state_less
    {
        bool operator()(const boxes_state& lhs, const boxes_state& rhs) const noexcept
        {
            if      (lhs.m_next_allowed_box < rhs.m_next_allowed_box) return  true;
            else if (lhs.m_next_allowed_box > rhs.m_next_allowed_box) return false;
            else if (lhs.m_base_min < rhs.m_base_min)                 return  true;
            else if (lhs.m_base_min > rhs.m_base_min)                 return false;
            else if (lhs.m_base_max < rhs.m_base_max)                 return  true;
            else if (lhs.m_base_max > rhs.m_base_max)                 return false;
            else                                                      return false; 
        }
    };

    std::uint32_t box_stacking_iterative_in_graph(const std::vector<box>& boxes) 
    {
        std::map<boxes_state, std::uint32_t, stack_state_less> graph; // value = stack height 
        graph[{0, inf<std::uint32_t>, inf<std::uint32_t>}] = 0;

        std::queue<boxes_state> queue; 
        queue.push({0, inf<std::uint32_t>, inf<std::uint32_t>});
  
        while(!queue.empty())
        {
            auto s_prev = queue.front();
            auto v_prev = graph[queue.front()];
            queue.pop();

            for(std::uint32_t n=s_prev.m_next_allowed_box; n!=boxes.size(); ++n)
            {
                // ********* //
                // *** x *** //
                // ********* //
                if (s_prev.is_greater_eq(boxes[n], orientation::x_up))
                {
                    auto s = s_prev;
                    auto v = v_prev + boxes[n].m_x; 
                    s.update(n+1, boxes[n], orientation::x_up);

                    if (euler_update<std::greater<std::uint32_t>>(graph, s, v)) queue.push(s);
                }

                // ********* //
                // *** y *** //
                // ********* //
                if (s_prev.is_greater_eq(boxes[n], orientation::y_up))
                {
                    auto s = s_prev;
                    auto v = v_prev + boxes[n].m_y; 
                    s.update(n+1, boxes[n], orientation::y_up);

                    if (euler_update<std::greater<std::uint32_t>>(graph, s, v)) queue.push(s);
                }

                // ********* //
                // *** z *** //
                // ********* //
                if (s_prev.is_greater_eq(boxes[n], orientation::z_up))
                {
                    auto s = s_prev;
                    auto v = v_prev + boxes[n].m_z; 
                    s.update(n+1, boxes[n], orientation::z_up);

                    if (euler_update<std::greater<std::uint32_t>>(graph, s, v)) queue.push(s);
                }
            }
        }
      
        std::uint32_t ans = 0;
        for(const auto& x:graph)
        {
            if (ans < x.second)
                ans = x.second;
        }   
        return ans;
    } 
   
    // ************************************************************************************ //
    // We do not use matrix here, as we cannot calculate matrix.size_x (can be very large).
    // Therefore we use 2 std::map instead, to represent prev_row and this_row.
    // ************************************************************************************ //
    std::uint32_t box_stacking_iterative_in_matrix(const std::vector<box>& boxes)
    {
/* 
        std::map<std::uint32_t> mat(numbers.size(), target+1, 0); 

        // init 1st row
        mat(0,numbers[0]) = 1; 

        // init 1st col
        for(std::uint32_t n=0; n!=numbers.size(); ++n)
        {
            mat(n,0) = 1;
        }

        // main iteration
        for(std::uint32_t n=1; n!=numbers.size(); ++n)
        {
            for(std::uint32_t m=1; m<=target; ++m)
            {
                if (m >= numbers[n])
                {
                    if (mat(n-1,m)            == 1) mat(n,m) = 1;
                    if (mat(n-1,m-numbers[n]) == 1) mat(n,m) = 1;
                }
                else
                {
                    mat(n,m) = mat(n-1,m);
                }
            }
        }
*/
        std::uint32_t ans = 0;
//      for(std::uint32_t m=0; m!=mat.size_x(); ++m)
//      {
//          if (ans < m && mat(numbers.size()-1, m) == 1)
//              ans = m;
//      }
        return ans;
    } 
}
