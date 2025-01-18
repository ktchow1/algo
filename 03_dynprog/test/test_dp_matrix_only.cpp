#include<iostream>
#include<iomanip>
#include<cassert>
#include<dp_matrix_only.h>
#include<utility.h>


void test_longest_common_subseq()
{
    std::uint32_t num_trial     = 100;
    std::uint32_t str_size      = 12;
    std::uint32_t num_alphabets = 5;
    benchmark<2>("longest_common_subseq --- recursive vs iterative (matrix)",
                 std::bind(gen_random_str, str_size, num_alphabets), 
                 std::bind(alg::longest_common_subseq_recursive, _1, _2),
                 std::bind(alg::longest_common_subseq_iterative, _1, _2),      
                 num_trial);
}


void test_edit_distance()
{
    std::uint32_t num_trial     = 100;
    std::uint32_t str_size      = 10;
    std::uint32_t num_alphabets = 5;
    benchmark<2>("edit_distance ----------- recursive vs iterative (matrix)",
                 std::bind(gen_random_str, str_size, num_alphabets), 
                 std::bind(alg::edit_distance_recursive, _1, _2),
                 std::bind(alg::edit_distance_iterative, _1, _2),      
                 num_trial);
}


void test_boolean_parenthesis()
{
}


void test_dp_matrix_only()
{
    test_longest_common_subseq();
    test_edit_distance();
    test_boolean_parenthesis();
}
