#include<iostream>
#include<iomanip>
#include<cassert>
#include<state_time_problem.h>
#include<utility.h>


void test_state_time_problem()
{
    std::uint32_t num_trial = 1000;
    bool print_each_test_case = false;
  
    benchmark_vec_with_alg_input("min_coin_change test 1",           
                                 gen_random_coins, 
                                 alg::min_coin_change_iterative_in_time,           
                                 alg::min_coin_change_iterative_in_subprob,  
                                 num_trial, 7, 1, 20, 120, print_each_test_case);  

    benchmark_vec_with_alg_input("min_coin_change test 2",           
                                 gen_random_coins, 
                                 alg::min_coin_change_iterative_in_time,           
                                 alg::min_coin_change_resursive,  
                                 10, 3, 1, 8, 25, print_each_test_case); 
}


