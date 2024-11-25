#include<iostream>
#include<subseq.h>


std::vector<std::int32_t> gen_random_signed_vec(std::uint32_t size, std::int32_t min, std::int32_t max)
{
    std::vector<std::int32_t> ans;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        std::int32_t x = min + std::rand() % (max-min);
        ans.push_back(x);
    }
    return ans;
}

std::vector<std::uint32_t> gen_random_unsigned_vec(std::uint32_t size, std::uint32_t min, std::uint32_t max)
{
    std::vector<std::uint32_t> ans;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        std::uint32_t x = min + std::rand() % (max-min);
        ans.push_back(x);
    }
    return ans;
}

template<typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION, typename INPUT_TYPE>
void test_max_subseq(const std::string&  test_name,
                     const GEN_FUNCTION& gen_function, 
                     const ALG_FUNCTION& alg_function, 
                     const BMK_FUNCTION& bmk_function, 
                     std::uint32_t       trial, 
                     std::uint32_t       size,
                     INPUT_TYPE          min_value,
                     INPUT_TYPE          max_value)
{
    std::uint32_t error = 0;
    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto vec  = gen_function(size, min_value, max_value);
        auto ans0 = alg_function(vec);
        auto ans1 = bmk_function(vec); 
        bool flag = (ans0 == ans1);
        if (!flag) ++error;

        std::cout << "\n" << test_name
                  << " : ans0 = " << ans0 
                  <<  ", ans1 = " << ans1 
                  <<  ", rate = " << error
                  <<  "/" << trial
                  <<  " " << (flag? "OK":"ERROR");
    }
}

template<typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION, typename INPUT_TYPE, typename TARGET_TYPE>
void test_count_subseq(const std::string&  test_name,
                       const GEN_FUNCTION& gen_function, 
                       const ALG_FUNCTION& alg_function, 
                       const BMK_FUNCTION& bmk_function, 
                       std::uint32_t       trial, 
                       std::uint32_t       size,
                       INPUT_TYPE          min_value,
                       INPUT_TYPE          max_value,
                       TARGET_TYPE         target)
{
    std::uint32_t error = 0;
    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto vec  = gen_function(size, min_value, max_value);
        auto ans0 = alg_function(vec, target);
        auto ans1 = bmk_function(vec, target); 
        bool flag = (ans0 == ans1);
        if (!flag) ++error;

        std::cout << "\n" << test_name
                  << " : ans0 = " << ans0 
                  <<  ", ans1 = " << ans1 
                  <<  ", rate = " << error
                  <<  "/" << trial
                  <<  " " << (flag? "OK":"ERROR");
    }
}

void test_subseq_sum()
{
    /*
    test_max_subseq("max_subseq_sum",           
                      gen_random_signed_vec, 
                      alg::max_subseq_sum,           
                      alg::max_subseq_sum_bmk,  
                      300, 50, -30, +30); 

    test_max_subseq("max_subseq_prd",           
                      gen_random_signed_vec, 
                      alg::max_subseq_prd,           
                      alg::max_subseq_prd_bmk,  
                      300, 10, -12, 12); */ 

    test_max_subseq("max_non_contiguous_subseq_sum",           
                      gen_random_signed_vec, 
                      alg::max_non_contiguous_subseq_sum,           
                      alg::max_non_contiguous_subseq_sum_bmk,  
                      300, 50, -30, +30); 

    test_max_subseq("max_non_contiguous_subseq_sum",           
                      gen_random_signed_vec, 
                      alg::max_non_contiguous_subseq_sum,           
                      alg::max_non_contiguous_subseq_sum_bmk,  
                      10, 50, -30, +0); // all negative
    /*
    test_count_subseq("count_target_subseq_sum",           
                      gen_random_signed_vec, 
                      alg::count_target_subseq_sum,           
                      alg::count_target_subseq_sum_bmk,  
                      300, 30, -20, +20, 20); 

    test_count_subseq("count_target_divisible_subseq_sum", 
                      gen_random_signed_vec, 
                      alg::count_target_divisible_subseq_sum, 
                      alg::count_target_divisible_subseq_sum_bmk, 
                      300, 30, -20, 20, 8); 

    test_count_subseq("longest_target_subseq_sum", 
                      gen_random_signed_vec, 
                      alg::longest_target_subseq_sum, 
                      alg::longest_target_subseq_sum_bmk, 
                      300, 30, -20, 20, 20); 

    test_count_subseq("count_less_than_target_subseq_sum", 
                      gen_random_unsigned_vec, 
                      alg::count_less_than_target_subseq_sum, 
                      alg::count_less_than_target_subseq_sum_bmk, 

    test_count_subseq("count_less_than_target_subseq_prd", 
                      gen_random_unsigned_vec, 
                      alg::count_less_than_target_subseq_prd, 
                      alg::count_less_than_target_subseq_prd_bmk, 
                      30000, 10, 1, 12, 1234); 
    */
}


