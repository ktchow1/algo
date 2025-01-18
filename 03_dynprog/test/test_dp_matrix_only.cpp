#include<iostream>
#include<iomanip>
#include<cassert>

// from alg
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

void print(const auto& tree_perms)
{
    std::uint32_t n=0;
    for(const auto& perm:tree_perms)
    {
        std::cout << "\nperm_" << n << " "; ++n;
        for(const auto& x:perm) std::cout << x << ",";
    }
}


void test_boolean_parenthesis()
{
    assert(alg::tree_permutations(1).size() ==  1);
    assert(alg::tree_permutations(2).size() ==  2);
    assert(alg::tree_permutations(3).size() ==  5);
    assert(alg::tree_permutations(4).size() == 14);
    assert(alg::tree_permutations(5).size() == 42);

    std::uint32_t num_trial  = 100;
    std::uint32_t input_size = 9;
    benchmark<1>("bool_parenthesis -------- iterative vs exhaustive",
                 std::bind(gen_random_bool_expression, input_size), 
                 std::bind(alg::bool_parenthesis_iterative,  _1),
                 std::bind(alg::bool_parenthesis_exhaustive, _1),      
                 num_trial); 
}


void test_coin_game()
{
    std::uint32_t num_trial  = 100;
    std::uint32_t input_size = 20;
    benchmark<1>("coin_game --------------- recursive vs iterative",
                 std::bind(gen_random_vec<std::uint32_t>, input_size, 1, 20), 
                 std::bind(alg::coin_game_recursive, _1),
                 std::bind(alg::coin_game_iterative, _1),      
                 num_trial, true); 
}


void test_dp_matrix_only()
{
//  test_longest_common_subseq();
//  test_edit_distance();
//  test_boolean_parenthesis();
    test_coin_game();
}
