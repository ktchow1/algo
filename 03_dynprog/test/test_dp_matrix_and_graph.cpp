#include<iostream>
#include<iomanip>
#include<cassert>
#include<dp_matrix_and_graph.h>
#include<utility.h>


void test_coin_change()
{
    std::uint32_t num_trial;
           
    num_trial = 100;
    benchmark<1>("min_coin_change - recursive vs iterative (state) with ans=inf",
                 std::bind(gen_random_coins, 3, 2, 9), 
                 std::bind(alg::min_coin_change_recursive_in_graph, _1, 25),
                 std::bind(alg::min_coin_change_iterative_in_graph, _1, 25),      
                 num_trial);
 
    num_trial = 1000;
    benchmark<1>("min_coin_change - recursive vs iterative (subprob)", 
                 std::bind(gen_random_coins, 4, 2, 30), 
                 std::bind(alg::min_coin_change_recursive_in_matrix, _1, 100),
                 std::bind(alg::min_coin_change_iterative_in_matrix, _1, 100),      
                 num_trial);
          
    num_trial = 1000;
    benchmark<1>("min_coin_change - recursive vs iterative (subprob) with ans=inf", 
                 std::bind(gen_random_coins, 2, 7, 19), 
                 std::bind(alg::min_coin_change_recursive_in_matrix, _1, 73),
                 std::bind(alg::min_coin_change_iterative_in_matrix, _1, 73),      
                 num_trial);
  
    num_trial = 1000;
    benchmark<1>("min_coin_change - state vs subprob (iterative)",           
                 std::bind(gen_random_coins, 8, 1, 60), 
                 std::bind(alg::min_coin_change_iterative_in_graph,  _1, 600),      
                 std::bind(alg::min_coin_change_iterative_in_matrix, _1, 600),
                 num_trial);

    num_trial = 1000;
    benchmark<1>("min_coin_change - state vs subprob (iterative) with ans=inf",           
                 std::bind(gen_random_coins, 2, 7, 19), 
                 std::bind(alg::min_coin_change_iterative_in_graph,  _1, 173),      
                 std::bind(alg::min_coin_change_iterative_in_matrix, _1, 173),
                 num_trial);
    
    num_trial = 1000;
    benchmark<1>("count_coin_change - recursive vs iterative (subprob)", 
                 std::bind(gen_random_coins, 8, 1, 30), 
                 std::bind(alg::count_coin_change_recursive_in_matrix, _1, 100),
                 std::bind(alg::count_coin_change_iterative_in_matrix, _1, 100),      
                 num_trial);
}


void test_knapsack()
{
    std::uint32_t num_trial = 1000;
    benchmark<1>("knapack - state vs subprob (iterative)",           
                 std::bind(gen_random_objects, 15, 10, 40, 1, 80), 
                 std::bind(alg::knapsack_iterative_in_graph,  _1, 400),      
                 std::bind(alg::knapsack_iterative_in_matrix, _1, 400),
                 num_trial); 
}


void test_job_schedule()
{
    std::uint32_t num_trial = 1000;
    benchmark<1>("job schedule - state vs subprob (iterative)",           
                 std::bind(gen_random_jobs, 30, 2, 20, 2, 30, 10, 250), 
                 std::bind(alg::job_schedule_iterative_in_graph,  _1),      
                 std::bind(alg::job_schedule_iterative_in_matrix, _1),
                 num_trial); 
}


void test_equal_partition()
{
    std::uint32_t num_trial = 1000;
    benchmark<1>("equal partition - state vs subprob (iterative)",           
                 std::bind(gen_random_vec<std::uint32_t>, 25, 2, 50), 
                 std::bind(alg::equal_partition_iterative_in_graph,  _1),      
                 std::bind(alg::equal_partition_iterative_in_matrix, _1), 
                 num_trial);  
}


void test_box_stacking()
{
    std::uint32_t num_trial = 10;
    benchmark<1>("box stacking - state vs subprob (iterative)",           
                 std::bind(gen_random_boxes, 20, 5, 50), 
                 std::bind(alg::box_stacking_iterative_in_graph,  _1),      
                 std::bind(alg::box_stacking_iterative_in_matrix, _1), 
                 num_trial, true);  
}


void test_state_time_problem()
{
    test_coin_change();
    test_knapsack();
    test_job_schedule();
    test_equal_partition();
    test_box_stacking();
}