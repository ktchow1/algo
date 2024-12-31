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
    for(std::uint32_t n=0; n!=3; ++n)
    {
        for(std::uint32_t m=0; m!=16; ++m)
        {
            alg::find_mode mode;
            if      (n==0) mode = alg::find_mode::iterative;
            else if (n==1) mode = alg::find_mode::recursive;
            else           mode = alg::find_mode::recursive_with_path_compression;

            alg::disjoint_set<std::string> set(mode);
            assert(set.is_same_set("a0", "a1") == false);
            assert(set.is_same_set("a1", "a2") == false);
            assert(set.is_same_set("a2", "a3") == false);
            set.union_of("a0","a1");
            set.union_of("a2","a3");
            set.union_of("a1","a3");
            assert(set.is_same_set("a0", "a1") == true);
            assert(set.is_same_set("a1", "a2") == true);
            assert(set.is_same_set("a2", "a3") == true);
            assert(set.is_same_set("a0", "b0") == false);
            assert(set.is_same_set("a1", "b1") == false);
            assert(set.is_same_set("a2", "b2") == false);
            assert(set.is_same_set("a3", "b3") == false);
            set.union_of("b0","b1");
            set.union_of("b2","b3");
            set.union_of("b0","b2");
            assert(set.is_same_set("a0", "a1") == true);
            assert(set.is_same_set("a1", "a2") == true);
            assert(set.is_same_set("a2", "a3") == true);
            assert(set.is_same_set("b0", "b1") == true);
            assert(set.is_same_set("b1", "b2") == true);
            assert(set.is_same_set("b2", "b3") == true);
            assert(set.is_same_set("a0", "b0") == false);
            assert(set.is_same_set("a1", "b1") == false);
            assert(set.is_same_set("a2", "b2") == false);
            assert(set.is_same_set("a3", "b3") == false);
            set.union_of("c0","c1");
            set.union_of("c2","c3");
            set.union_of("c0","c3");
            assert(set.is_same_set("c0", "c1") == true);
            assert(set.is_same_set("c1", "c2") == true);
            assert(set.is_same_set("c2", "c3") == true);
            assert(set.is_same_set("a0", "b0") == false);
            assert(set.is_same_set("a1", "b1") == false);
            assert(set.is_same_set("a2", "b2") == false);
            assert(set.is_same_set("a3", "b3") == false);
            assert(set.is_same_set("b0", "c0") == false);
            assert(set.is_same_set("b1", "c1") == false);
            assert(set.is_same_set("b2", "c2") == false);
            assert(set.is_same_set("b3", "c3") == false);

            if      (m==0)    set.union_of("b0","c0");
            else if (m==1)    set.union_of("b0","c1");
            else if (m==2)    set.union_of("b0","c2");
            else if (m==3)    set.union_of("b0","c3");
            else if (m==4)    set.union_of("b1","c0");
            else if (m==5)    set.union_of("b1","c1");
            else if (m==6)    set.union_of("b1","c2");
            else if (m==7)    set.union_of("b1","c3");
            else if (m==8)    set.union_of("b2","c0");
            else if (m==9)    set.union_of("b2","c1");
            else if (m==10)   set.union_of("b2","c2");
            else if (m==11)   set.union_of("b2","c3");
            else if (m==12)   set.union_of("b3","c0");
            else if (m==13)   set.union_of("b3","c1");
            else if (m==14)   set.union_of("b3","c2");
            else              set.union_of("b3","c3");

            assert(set.is_same_set("a0", "b0") == false);
            assert(set.is_same_set("a1", "b1") == false);
            assert(set.is_same_set("a2", "b2") == false);
            assert(set.is_same_set("a3", "b3") == false);
            assert(set.is_same_set("b0", "c0") == true);
            assert(set.is_same_set("b1", "c1") == true);
            assert(set.is_same_set("b2", "c2") == true);
            assert(set.is_same_set("b3", "c3") == true);
        }
    }
    print_summary("test_disjoint_set", "all succeeded");
}

void test_prefix_tree()
{
    alg::prefix_tree<std::uint64_t> ptree;
    alg::prefix_tree<std::uint64_t>::fct_type fct = [](const auto& key, const auto& optional_value)
    { 
        if (optional_value) 
             std::cout << "\nkey=" << key << ", value=" << *optional_value; 
        else std::cout << "\nkey=" << key << ", value=null"; 
    };

    ptree.insert("abc", 123);
    ptree.traverse(fct);


}

void test_tree_variant()
{
//  test_heap();
//  test_disjoint_set();
    test_prefix_tree();
}

