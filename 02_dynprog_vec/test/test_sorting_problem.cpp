#include<iostream>
#include<utility.h>
#include<sorting_problem.h>

  
void test_sorting_problem()
{
    std::uint32_t num_trial = 10000;
    bool print_each_test_case = true;

    std::uint32_t kth_order = 99;
    benchmark_vec_with_arg("order_statistics",           
                            gen_random_vec<std::uint32_t>, 
                            alg::order_statistics,           
                            alg::order_statistics_bmk,
                            num_trial, 200, 0, 100, kth_order, print_each_test_case); 
/*
    benchmark_vec("min_number_adjacent_swap",           
                  gen_random_vec<std::uint32_t>, 
                  alg::min_number_adjacent_swap,   
                  alg::min_number_adjacent_swap_bmk,
                  num_trial, 100, 0, 80, print_each_test_case); */
}
  
