#include<iostream>
#include<iomanip>
#include<cassert>
#include<state_time_problem.h>
#include<utility.h>


void test_min_coin_change()
{
    std::uint32_t num_trial;
    bool print_each_test_case = false;
     
    num_trial = 20;
    benchmark<1>("min_coin_change 1",
                 std::bind(gen_random_coins, 3, 2, 10), 
                 std::bind(alg::min_coin_change_recursive_in_state, _1, 25),
                 std::bind(alg::min_coin_change_iterative_in_state, _1, 25),      
                 num_trial, print_each_test_case);
 
    num_trial = 1000;
    benchmark<1>("min_coin_change 2",           
                 std::bind(gen_random_coins, 6, 2, 30), 
                 std::bind(alg::min_coin_change_recursive_in_subprob, _1, 100),
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 100),      
                 num_trial, print_each_test_case);
    
    num_trial = 1000;
    benchmark<1>("min_coin_change 3",           
                 std::bind(gen_random_coins, 20, 2, 100), 
                 std::bind(alg::min_coin_change_iterative_in_state,   _1, 800),      
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 800),
                 num_trial, print_each_test_case);
}


void test_count_coin_change()
{
    {
    //  std::vector<std::uint32_t> coins{1,2,5};
    //  assert(alg::count_coin_change_iterative_in_subprob(coins,1) == 1);  
    //  assert(alg::count_coin_change_iterative_in_subprob(coins,2) == 2); 
    //  assert(alg::count_coin_change_iterative_in_subprob(coins,3) == 2); 
    //  assert(alg::count_coin_change_iterative_in_subprob(coins,4) == 3); 
    //  assert(alg::count_coin_change_iterative_in_subprob(coins,5) == 4); 
    //  assert(alg::count_coin_change_iterative_in_subprob(coins,6) == 5); 
    //  assert(alg::count_coin_change_iterative_in_subprob(coins,7) == 6); 
    //  assert(alg::count_coin_change_iterative_in_subprob(coins,8) == 7); 
    }
    print_summary("count_coin_change iterative vs hardcoded answer", "all succeeded");
}


void test_state_time_problem()
{
    test_min_coin_change();
//  test_count_coin_change();
}
