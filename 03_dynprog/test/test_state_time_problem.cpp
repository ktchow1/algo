#include<iostream>
#include<iomanip>
#include<cassert>
#include<state_time_problem.h>
#include<utility.h>


void test_state_time_problem()
{
    for(std::uint32_t target=10; target!=20; ++target)
    {
        std::vector<std::uint32_t> coins{1,2,5};
        auto ans0 = alg::min_coin_change_iterative_in_time(coins, target);
        auto ans1 = alg::min_coin_change_recursive(coins, target);
        assert(ans0 == ans1);
    }
    print_summary("min_coin_change test 1", "all succeeded");


    std::uint32_t num_trial = 500;
    bool print_each_test_case = false;
  
    benchmark<1>("min_coin_change test 2",           
                 std::bind(gen_random_coins, 7, 1, 20), 
                 std::bind(alg::min_coin_change_iterative_in_time,    _1, 120),      
                 std::bind(alg::min_coin_change_iterative_in_subprob, _1, 120),
                 num_trial, print_each_test_case);
}


