#include<iostream>
#include<iomanip>
#include<cassert>
#include<state_time_problem.h>
#include<utility.h>


void test_coin_change()
{
    std::uint32_t num_trial;
    bool print_each_test_case = false;
     
    num_trial = 20;
    benchmark<1>("min_coin_change recursive vs iterative (state)",
                 std::bind(gen_random_coins, 3, 2, 10), 
                 std::bind(alg::min_coin_change_recursive_in_state, _1, 25),
                 std::bind(alg::min_coin_change_iterative_in_state, _1, 25),      
                 num_trial, print_each_test_case);
 
    num_trial = 1000;
    benchmark<1>("min_coin_change recursive vs iterative (subprob)", 
                 std::bind(gen_random_coins, 6, 2, 30), 
                 std::bind(alg::min_coin_change_recursive_in_subprob, _1, 100),
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 100),      
                 num_trial, print_each_test_case);
    
    num_trial = 1000;
    benchmark<1>("min_coin_change state vs subprob (iterative)",           
                 std::bind(gen_random_coins, 20, 2, 100), 
                 std::bind(alg::min_coin_change_iterative_in_state,   _1, 800),      
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 800),
                 num_trial, print_each_test_case);

    num_trial = 1000;
    benchmark<1>("count_coin_change recursive vs iterative (subprob)", 
                 std::bind(gen_random_coins, 6, 2, 30), 
                 std::bind(alg::count_coin_change_recursive_in_subprob, _1, 100),
                 std::bind(alg::count_coin_change_iterative_in_subprob, _1, 100),      
                 num_trial, print_each_test_case);
}


void test_knapsack()
{
    std::uint32_t num_trial = 10;
    bool print_each_test_case = true;

    benchmark<1>("knapack state vs subprob (iterative)",           
                 std::bind(gen_random_objects, 3, 2, 10, 2, 10), 
                 std::bind(alg::knapsack_iterative_in_state,   _1, 20),      
                 std::bind(alg::knapsack_iterative_in_subprob, _1, 20),
                 num_trial, print_each_test_case);
    /*
    benchmark<1>("knapack state vs subprob (iterative)",           
                 std::bind(gen_random_objects, 20, 10, 50, 1, 100), 
                 std::bind(alg::knapsack_iterative_in_state,   _1, 400),      
                 std::bind(alg::knapsack_iterative_in_subprob, _1, 400),
                 num_trial, print_each_test_case); */
}


void test_state_time_problem()
{
//  test_coin_change();
    test_knapsack();
}
