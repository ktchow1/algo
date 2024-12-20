#include<iostream>
#include<utility.h>
#include<two_point_diff.h>


void test_two_point_diff()
{
    std::uint32_t num_trial = 1000;
    bool print_each_test_case = false;

    benchmark_vec("max_profit",           
                  gen_random_vec<std::int32_t>, 
                  alg::max_profit,           
                  alg::max_profit_bmk,  
                  num_trial, 30, 100, 500, print_each_test_case); 
    
    benchmark_vec_with_arg("count_target_profit", 
                            gen_random_vec<std::int32_t>, 
                            alg::count_target_profit,           
                            alg::count_target_profit_bmk,  
                            num_trial, 100, 100, 300, 40, print_each_test_case); 

    benchmark_vec_with_arg("count_target_abs_profit", 
                            gen_random_vec<std::int32_t>, 
                            alg::count_target_abs_profit,           
                            alg::count_target_abs_profit_bmk,  
                            num_trial, 100, 100, 300, 40, print_each_test_case); 
}
