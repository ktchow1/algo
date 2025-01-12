#include<iostream>
#include<iomanip>
#include<cassert>
#include<state_time_problem.h>
#include<utility.h>


void test_coin_change()
{
    std::uint32_t num_trial;
    bool print_each_test_case = false;
           
    num_trial = 100;
    benchmark<1>("min_coin_change - recursive vs iterative (state) with ans=inf",
                 std::bind(gen_random_coins, 3, 2, 9), 
                 std::bind(alg::min_coin_change_recursive_in_state, _1, 25),
                 std::bind(alg::min_coin_change_iterative_in_state, _1, 25),      
                 num_trial, print_each_test_case);
 
    num_trial = 1000;
    benchmark<1>("min_coin_change - recursive vs iterative (subprob)", 
                 std::bind(gen_random_coins, 4, 2, 30), 
                 std::bind(alg::min_coin_change_recursive_in_subprob, _1, 100),
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 100),      
                 num_trial, print_each_test_case);
          
    num_trial = 1000;
    benchmark<1>("min_coin_change - recursive vs iterative (subprob) with ans=inf", 
                 std::bind(gen_random_coins, 2, 7, 19), 
                 std::bind(alg::min_coin_change_recursive_in_subprob, _1, 73),
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 73),      
                 num_trial, print_each_test_case);
  
    num_trial = 1000;
    benchmark<1>("min_coin_change - state vs subprob (iterative)",           
                 std::bind(gen_random_coins, 8, 1, 60), 
                 std::bind(alg::min_coin_change_iterative_in_state,   _1, 600),      
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 600),
                 num_trial, print_each_test_case);

    num_trial = 1000;
    benchmark<1>("min_coin_change - state vs subprob (iterative) with ans=inf",           
                 std::bind(gen_random_coins, 2, 7, 19), 
                 std::bind(alg::min_coin_change_iterative_in_state,   _1, 173),      
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 173),
                 num_trial, print_each_test_case);
    
    num_trial = 1000;
    benchmark<1>("count_coin_change - recursive vs iterative (subprob)", 
                 std::bind(gen_random_coins, 8, 1, 30), 
                 std::bind(alg::count_coin_change_recursive_in_subprob, _1, 100),
                 std::bind(alg::count_coin_change_iterative_in_subprob, _1, 100),      
                 num_trial, print_each_test_case);
}


void test_knapsack()
{
    std::uint32_t num_trial = 1000;
    bool print_each_test_case = false;

    benchmark<1>("knapack - state vs subprob (iterative)",           
                 std::bind(gen_random_objects, 15, 10, 40, 1, 80), 
                 std::bind(alg::knapsack_iterative_in_state,   _1, 400),      
                 std::bind(alg::knapsack_iterative_in_subprob, _1, 400),
                 num_trial, print_each_test_case); 
}


void test_job_schedule()
{
    std::uint32_t num_trial = 1000;
    bool print_each_test_case = true;

    benchmark<1>("job schedule - state vs subprob (iterative)",           
                 std::bind(gen_random_jobs, 30, 2, 20, 2, 30, 10, 250), 
                 std::bind(alg::job_schedule_iterative_in_state,   _1),      
                 std::bind(alg::job_schedule_iterative_in_subprob, _1),
                 num_trial, print_each_test_case, true); 
}


void test_state_time_problem()
{
//  test_coin_change();
//  test_knapsack();
    test_job_schedule();
}
