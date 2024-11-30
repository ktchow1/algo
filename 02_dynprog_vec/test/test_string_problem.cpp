#include<iostream>
#include<utility.h>
#include<string_problem.h>

  
void test_string_problem()
{
    std::uint32_t num_trial = 10;
    bool print_each_test_case = true;

    benchmark_str("longest_non_duplicate_substr",           
                  gen_random_str, 
                  alg::longest_non_duplicated_substr,           
                  alg::longest_non_duplicated_substr_bmk,
                  num_trial, 10, print_each_test_case);
}
  
