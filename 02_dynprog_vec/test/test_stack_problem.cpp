#include<iostream>
#include<utility.h>
#include<stack_problem.h>

  
void test_stack_problem()
{
    std::uint32_t num_trial = 10;
    bool print_each_test_case = true;
/*  
    benchmark_vec("shortest_unsorted_subseq",           
                  gen_random_unsigned_partial_sorted_vec, 
                  alg::shortest_unsorted_subseq,           
                  alg::shortest_unsorted_subseq_bmk,
                  num_trial, 300, 0, 200, print_each_test_case);  

    benchmark_vec("count_stroke_in_histogram",           
                  gen_random_unsigned_vec, 
                  alg::count_stroke_in_histogram,           
                  alg::count_stroke_in_histogram_bmk,
                  num_trial, 300, 0, 50, print_each_test_case);  */

    benchmark_vec("biggest_rectangle_in_histogram",           
                  gen_random_unsigned_vec, 
                  alg::biggest_rectangle_in_histogram,           
                  alg::biggest_rectangle_in_histogram_bmk,
                  num_trial, 30, 10, 20, print_each_test_case);  
}
  
