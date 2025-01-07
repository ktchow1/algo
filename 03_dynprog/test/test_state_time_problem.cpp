#include<iostream>
#include<iomanip>
#include<cassert>
#include<state_time_problem.h>
#include<utility.h>


void test_state_time_problem()
{
    std::uint32_t num_trial = 500;
    bool print_each_test_case = false;
  
    benchmark<1>("test min_coin_change test 1",           
                 std::bind(gen_random_coins, 7, 1, 20), 
                 std::bind(alg::min_coin_change_iterative_in_time,    _1, 120),      
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 120),
                 num_trial, print_each_test_case);  

    benchmark<1>("test min_coin_change test 2", 
                 std::bind(gen_random_coins, 3, 1, 8),
                 std::bind(alg::min_coin_change_iterative_in_time, _1, 25),         
                 std::bind(alg::min_coin_change_resursive,         _1, 25), 
                 10, print_each_test_case); 
}


