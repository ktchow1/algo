#include<iostream>
#include<utility.h>
#include<subseq.h>


void test_subseq_sum()
{
    std::uint32_t num_trial = 1000;
    bool print_each_test_case = false;

    benchmark_vec("max_subseq_sum",           
                  gen_random_vec<std::int32_t>, 
                  alg::max_subseq_sum,           
                  alg::max_subseq_sum_bmk,  
                  num_trial, 50, -30, +30, print_each_test_case); 

    benchmark_vec("max_subseq_prd",           
                  gen_random_vec<std::int32_t>, 
                  alg::max_subseq_prd,           
                  alg::max_subseq_prd_bmk,  
                  num_trial, 10, -11, 11, print_each_test_case); 

    benchmark_vec("max_non_contiguous_subseq_sum",           
                  gen_random_vec<std::int32_t>, 
                  alg::max_non_contiguous_subseq_sum,           
                  alg::max_non_contiguous_subseq_sum_bmk,  
                  num_trial, 50, -30, +30, print_each_test_case); 

    benchmark_vec("max_non_contiguous_subseq_sum",           
                  gen_random_vec<std::int32_t>, 
                  alg::max_non_contiguous_subseq_sum,           
                  alg::max_non_contiguous_subseq_sum_bmk,  
                  num_trial, 50, -30, +0, print_each_test_case); // all negative
    
    benchmark_vec_with_alg_input("count_target_subseq_sum",           
                                 gen_random_vec<std::int32_t>, 
                                 alg::count_target_subseq_sum,           
                                 alg::count_target_subseq_sum_bmk,  
                                 num_trial, 30, -20, +20, 20, print_each_test_case); 

    benchmark_vec_with_alg_input("count_target_divisible_subseq_sum", 
                                 gen_random_vec<std::int32_t>, 
                                 alg::count_target_divisible_subseq_sum, 
                                 alg::count_target_divisible_subseq_sum_bmk, 
                                 num_trial, 30, -20, 20, 8, print_each_test_case); 

    benchmark_vec_with_alg_input("longest_target_subseq_sum", 
                                 gen_random_vec<std::int32_t>, 
                                 alg::longest_target_subseq_sum, 
                                 alg::longest_target_subseq_sum_bmk, 
                                 num_trial, 30, -20, 20, 20, print_each_test_case); 

    benchmark_vec_with_alg_input("count_less_than_target_subseq_sum", 
                                 gen_random_vec<std::uint32_t>, 
                                 alg::count_less_than_target_subseq_sum, 
                                 alg::count_less_than_target_subseq_sum_bmk, 
                                 num_trial, 30, 1, 60, 100, print_each_test_case); 

    benchmark_vec_with_alg_input("count_less_than_target_subseq_prd", 
                                 gen_random_vec<std::uint32_t>, 
                                 alg::count_less_than_target_subseq_prd, 
                                 alg::count_less_than_target_subseq_prd_bmk, 
                                 num_trial, 10, 1, 12, 1234, print_each_test_case); 
    
}


