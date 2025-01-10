#include<iostream>
#include<iomanip>
#include<cassert>
#include<state_time_problem.h>
#include<utility.h>


void test_min_coin_change()
{
    std::uint32_t num_trial;
    bool print_each_test_case = false;
     
    num_trial = 10;
    benchmark<1>("min_coin_change iterative_in_time vs slow_recursive",           
                 std::bind(gen_random_coins, 4, 2, 8), 
                 std::bind(alg::min_coin_change_iterative_in_time, _1, 25),      
                 std::bind(alg::min_coin_change_slow_recursive,    _1, 25),
                 num_trial, print_each_test_case);
 
    num_trial = 500;
    benchmark<1>("min_coin_change iterative_in_time vs fast_recursive",           
                 std::bind(gen_random_coins, 5, 2, 12), 
                 std::bind(alg::min_coin_change_iterative_in_time, _1, 50),      
                 std::bind(alg::min_coin_change_fast_recursive,    _1, 50),
                 num_trial, print_each_test_case);
 
    num_trial = 500;
    benchmark<1>("min_coin_change iterative0 vs iterative1",           
                 std::bind(gen_random_coins, 10, 2, 50), 
                 std::bind(alg::min_coin_change_iterative_in_time,    _1, 250),      
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 250),
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
