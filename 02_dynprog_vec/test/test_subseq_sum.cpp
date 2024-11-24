#include<iostream>
#include<subseq_sum.h>











std::vector<std::int32_t> gen_random_signed_vector(std::uint32_t size, std::int32_t min, std::int32_t max)
{
    std::vector<std::int32_t> ans;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        std::int32_t x = min + std::rand() % (max-min);
        ans.push_back(x);
    }
    return ans;
}

std::vector<std::uint32_t> gen_random_unsigned_vector(std::uint32_t size, std::uint32_t min, std::uint32_t max)
{
    std::vector<std::uint32_t> ans;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        std::uint32_t x = min + std::rand() % (max-min);
        ans.push_back(x);
    }
    return ans;
}

void test_count_target_subseq_sum()
{
    std::uint32_t trial = 100;
    std::uint32_t error = 0;
    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto vec  = gen_random_signed_vector(50, -10, 10);
        auto ans0 = alg::count_target_subseq_sum(vec, 30);
        auto ans1 = alg::count_target_subseq_sum_benchmark(vec, 30); 
        bool flag = (ans0 == ans1);
        if (!flag) ++error;

        std::cout << "\ncount_target_subseq_sum "
                  << ": ans0 = " << ans0 
                  << ", ans1 = " << ans1 
                  << ", rate = " << error
                  << "/" << trial
                  << " " << (flag? "ERROR":"OK");
    }
}

void test_count_target_divisible_subseq_sum()
{
}

void test_longest_target_subseq_sum()
{
}

void test_count_less_than_target_subseq_sum()
{
}

void test_count_less_than_target_subseq_product()
{
}

void test_subseq_sum()
{
    test_count_target_subseq_sum();
    test_count_target_divisible_subseq_sum();
    test_longest_target_subseq_sum();
    test_count_less_than_target_subseq_sum();
    test_count_less_than_target_subseq_product();
}


