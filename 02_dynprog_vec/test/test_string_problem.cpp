#include<iostream>
#include<utility.h>
#include<string_problem.h>

  
void test_string_problem()
{
    std::uint32_t num_trial = 100000;
    bool print_each_test_case = true;
/*
    benchmark_str("longest_non_duplicate_substr",           
                  gen_random_str, 
                  alg::longest_non_duplicated_substr,           
                  alg::longest_non_duplicated_substr_bmk,
                  num_trial, 20, 26, print_each_test_case); */

    benchmark_str("longest_odd_palindrome_substr",           
                  gen_random_palindrome, 
                  alg::longest_odd_palindrome_substr,           
                  alg::longest_odd_palindrome_substr_bmk,
                  num_trial, 200, 5, print_each_test_case); 


    // **************************** //
    // *** For repeat and debug *** //
    // **************************** //
/*    
    std::string str = "cbabacccbcccccccbcabc";
    std::uint32_t ans = alg::longest_odd_palindrome_substr_optimized(str);
    std::cout << "\n[REPEAT CASE] ans = " << ans; */
}
  
