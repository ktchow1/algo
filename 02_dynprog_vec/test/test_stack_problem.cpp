#include<iostream>
#include<utility.h>
#include<stack_problem.h>

  
void test_stack_problem()
{
    std::uint32_t num_trial = 100000;
    bool print_each_test_case = true;
/*
    benchmark_vec("shortest_unsorted_subseq",           
                  gen_random_partial_sorted_vec<std::uint32_t>, 
                  alg::shortest_unsorted_subseq,           
                  alg::shortest_unsorted_subseq_bmk,
                  num_trial, 300, 0, 200, print_each_test_case); 

    benchmark_vec("count_stroke_in_histogram",           
                  gen_random_vec<std::uint32_t>, 
                  alg::count_stroke_in_histogram,           
                  alg::count_stroke_in_histogram_bmk,
                  num_trial, 300, 0, 50, print_each_test_case); */

    benchmark_vec("biggest_rect_in_hist",           
                  gen_random_vec<std::uint32_t>, 
                  alg::biggest_rect_in_hist,   
                  alg::biggest_rect_in_hist_bmk,
                  num_trial, 100, 0, 80, print_each_test_case);  
}
  
