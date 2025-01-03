#include<iostream>
#include<utility.h>
#include<two_point_sum.h>


void test_two_point_sum()
{
    std::uint32_t num_trial = 1000;
    bool print_each_test_case = false;

    benchmark_vec("max_2_point_sum_distance",           
                  gen_random_vec<std::int32_t>, 
                  alg::max_2_point_sum_distance,           
                  alg::max_2_point_sum_distance_bmk,
                  num_trial, 100, -100, +100, print_each_test_case);

    benchmark_vec("max_2_point_sum_with_equal_digit_sum",           
                  gen_random_vec<std::uint32_t>, 
                  alg::max_2_point_sum_with_equal_digit_sum,           
                  alg::max_2_point_sum_with_equal_digit_sum_bmk,
                  num_trial, 50, 0, +1000, print_each_test_case); 

    // Todo
    // max_2_point_sum_with_non_attacking_rooks
    
    benchmark_vec_with_alg_input("check_target_2_point_sum_in_1_sorted_vec", 
                                 gen_random_sorted_vec<std::int32_t>, 
                                 alg::check_target_2_point_sum_in_1_sorted_vec,           
                                 alg::check_target_2_point_sum_in_1_sorted_vec_bmk,           
                                 num_trial, 100, -6000, 6000, 40, print_each_test_case); 

    benchmark_2_vec_with_alg_input("check_target_2_point_sum_in_2_sorted_vec", 
                                   gen_random_sorted_vec<std::int32_t>, 
                                   alg::check_target_2_point_sum_in_2_sorted_vec,           
                                   alg::check_target_2_point_sum_in_2_sorted_vec_bmk,           
                                   num_trial, 100, -6000, 6000, 40, print_each_test_case);    
  
    benchmark_vec_with_alg_input("count_target_2_point_sum", 
                                 gen_random_vec<std::int32_t>, 
                                 alg::count_target_2_point_sum,           
                                 alg::count_target_2_point_sum_bmk,           
                                 num_trial, 50, -40, 40, 50, print_each_test_case); 
    
    benchmark_vec_with_alg_input("count_target_3_point_sum", 
                                 gen_random_vec<std::int32_t>, 
                                 alg::count_target_3_point_sum,           
                                 alg::count_target_3_point_sum_bmk,           
                                 num_trial, 50, -40, 40, 75, print_each_test_case); 

    benchmark_vec_with_alg_input("count_target_4_point_sum", 
                                 gen_random_vec<std::int32_t>, 
                                 alg::count_target_4_point_sum,           
                                 alg::count_target_4_point_sum_bmk,           
                                 num_trial, 50, -40, 40, 100, print_each_test_case);  
}
