#include<iostream>
#include<iomanip>
#include<cassert>
#include<tree_variant.h>
#include<utility.h>


void test_heap()
{
    std::uint32_t trial = 10000;
    std::uint32_t error = 0;

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        alg::heap<std::uint32_t> heap;
        std::vector<std::uint32_t> vec0;
        std::vector<std::uint32_t> vec1;

        // ******************** //
        // *** Push to heap *** //
        // ******************** //
        std::uint32_t N = rand()%500;
        for(std::uint32_t n=0; n!=N; ++n)
        {
            std::uint32_t x = rand()%500;
            vec0.push_back(x);
            heap.push(x);
        }
        std::sort(vec0.begin(), vec0.end());

        // *************************** //
        // *** Pop till heap empty *** //
        // *************************** //
        while(!heap.empty())
        {
            vec1.push_back(heap.top());
            heap.pop();
        }

        if (!alg::is_equal(vec0, vec1)) ++error;
    }
    print_summary("test_heap", error, trial);
}

void test_disjoint_set()
{
}

void test_tree_variant()
{
    test_heap();
    test_disjoint_set();
}

