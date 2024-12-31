#include<iostream>
#include<iomanip>
#include<cassert>
#include<tree_variant.h>
#include<utility.h>


void test_heap()
{
    std::uint32_t trial  = 10000;
    std::uint32_t error0 = 0;
    std::uint32_t error1 = 0;

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        alg::heap<std::uint32_t> heap;
        std::vector<std::uint32_t> ans;
        std::vector<std::uint32_t> vec0;
        std::vector<std::uint32_t> vec1;

        // ************* //
        // *** Input *** //
        // ************* //
        std::uint32_t N = rand()%500;
        for(std::uint32_t n=0; n!=N; ++n)
        {
            std::uint32_t x = rand()%500;
            ans .push_back(x);
            vec0.push_back(x);
            vec1.push_back(x);
        }

        // ************** //
        // *** Output *** //
        // ************** //
        std::sort(ans.begin(), ans.end()); 
          
        // 0. by heap
        for(const auto& x:vec0)
        {
            heap.push(x);
        }
        vec0.clear();
        while(!heap.empty())
        {
            vec0.push_back(heap.top()); 
            heap.pop();
        }

        // 1. by heap_inplace
        alg::heap_inplace<std::uint32_t, std::greater<std::uint32_t>> temp(vec1.begin(), vec1.end());

        if (!alg::is_equal(ans, vec0)) ++error0;
        if (!alg::is_equal(ans, vec1)) ++error1;
    }
    print_summary("test_heap - normal", error0, trial);
    print_summary("test_heap - inplace", error1, trial);
}

void test_disjoint_set()
{
}

void test_tree_variant()
{
    test_heap();
    test_disjoint_set();
}

