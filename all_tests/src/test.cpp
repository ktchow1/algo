#include<iostream>
#include<interview.h>

void test_arithmetics();
void test_tree();
void test_tree_variant();

void test_subseq_sum_count();
void test_substr();
void test_stack_trick();
void test_sorting_trick();

void test_singly_link_subproblem();
void test_multi_link_subproblem();
void test_time_state_diagram();

void test_polymorphism();

void test_template();
void test_template_index_sequence();
void test_template_index_sequence_Maven();
void test_template_tuple();
void test_template_variadic();


int main()
{
    test_arithmetics();
    test_tree();
    test_tree_variant();
    
    test_subseq_sum_count();
    test_substr();
    test_stack_trick();
    test_sorting_trick();

    test_singly_link_subproblem();
    test_multi_link_subproblem();
    test_time_state_diagram();

    test_polymorphism();
  
    test_template(); 
    test_template_index_sequence();
    test_template_index_sequence_Maven();
    test_template_tuple();
    test_template_variadic();
  
    interview::test_alphagrep_max_min_diff_in_subset();
    interview::test_alphagrep_count_k_odd_subseq(); 
    interview::test_alphagrep_count_decreasing_triplet();
    interview::test_alphagrep_count_step_function();
    interview::test_apt3();
    interview::test_apt4();
    interview::test_apt5();
    interview::test_archax_min_missing_pos_num();
    interview::test_citadel_matrix_spiral();
    interview::test_citadel_K_merge();
    interview::test_citadel_max_rook_sum();
//  interview::test_epoch_state();
    interview::test_maven_runtime_template();
    interview::test_optiver_tree_checker();
    interview::test_smarket_subseq_product();
    interview::test_smarket_point_puzzle();
    interview::test_wintermute_shared_ptr(); 

    std::cout << "\n\n\n";
    return 0;
}
