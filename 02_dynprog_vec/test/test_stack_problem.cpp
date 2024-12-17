#include<iostream>
#include<utility.h>
#include<stack_problem.h>

  
void test_stack_problem()
{
    std::uint32_t num_trial = 100000;
    bool print_each_test_case = true;
  
    benchmark_vec("count_stroke_in_histogram",           
                  gen_random_unsigned_vec, 
                  alg::count_stroke_in_histogram,           
                  alg::count_stroke_in_histogram_bmk,
                  num_trial, 800, 0, 30, print_each_test_case); 
}
  
