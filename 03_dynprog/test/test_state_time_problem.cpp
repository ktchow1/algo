#include<iostream>
#include<iomanip>
#include<cassert>
#include<state_time_problem.h>
#include<utility.h>


void test_min_coin_change()
{
    for(std::uint32_t target=10; target!=20; ++target)
    {
        std::vector<std::uint32_t> coins{1,2,5};
        auto ans0 = alg::min_coin_change_iterative_in_time(coins, target);
        auto ans1 = alg::min_coin_change_recursive(coins, target);
        assert(ans0 == ans1);
    }
    print_summary("min_coin_change iterative vs recursion", "all succeeded");


    std::uint32_t num_trial = 1000;
    bool print_each_test_case = false;
  
    benchmark<1>("min_coin_change iterative0 vs iterative1",           
                 std::bind(gen_random_coins, 10, 1, 50), 
                 std::bind(alg::min_coin_change_iterative_in_time,    _1, 250),      
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 250),
                 num_trial, print_each_test_case);
}


void test_count_coin_change()
{
    {
        std::vector<std::uint32_t> coins{1,2,5};
        assert(alg::count_coin_change_iterative_in_subprob(coins,1) == 1);  
        assert(alg::count_coin_change_iterative_in_subprob(coins,2) == 2); 
        assert(alg::count_coin_change_iterative_in_subprob(coins,3) == 2); 
        assert(alg::count_coin_change_iterative_in_subprob(coins,4) == 3); 
    }
    print_summary("count_coin_change iterative vs hardcoded answer", "all succeeded");
}


void test_state_time_problem()
{
//  test_min_coin_change();
    test_count_coin_change();
}
