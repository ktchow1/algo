#include<iostream>
#include<iomanip>
#include<cassert>
#include<tree.h>
#include<utility.h>




void test_avl_tree_checking()
{
}

void test_avl_tree_compare()
{
    std::uint32_t trial = 10000;
    std::uint32_t error0 = 0;
    std::uint32_t error1 = 0;
    std::uint32_t error2 = 0;

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        alg::avl::tree<std::uint32_t> tree;

        std::uint32_t N = 50 + rand()%250;
        for(std::uint32_t n=0; n!=N; ++n)
        {
            std::uint32_t x = rand()%500;
            auto* node = tree.insert(x);
            assert(node->m_value == x);
        }

        alg::traversal_cache<std::uint32_t> f0;
        alg::traversal_cache<std::uint32_t> f1;
        alg::traversal_cache<std::uint32_t> f2;
        alg::traversal_cache<std::uint32_t> f3;
        alg::traversal_cache<std::uint32_t> f4;
        alg::traversal_cache<std::uint32_t> f5;
        tree.traverse(f0, alg::avl::mode::dfs_in_order_recursive);  
        tree.traverse(f1, alg::avl::mode::dfs_pre_order_recursive); 
        tree.traverse(f2, alg::avl::mode::dfs_post_order_recursive);  // as long as no crash, its fine
        tree.traverse(f3, alg::avl::mode::dfs_pre_order_iterative); 
        tree.traverse(f4, alg::avl::mode::dfs_in_order_iterative);  
        tree.traverse(f5, alg::avl::mode::bfs_iterative);             // as long as no crash, its fine

        if (!alg::is_sorted(f0.m_values))             ++error0;
        if (!alg::is_equal(f0.m_values, f4.m_values)) ++error1;
        if (!alg::is_equal(f1.m_values, f3.m_values)) ++error2;
    }
    print_summary("test_avl_tree_compare in_order sorted", error0, trial);
    print_summary("test_avl_tree_compare in_order",        error1, trial);
    print_summary("test_avl_tree_compare pre_order",       error2, trial);
}

void test_tree()
{
    test_avl_tree_checking();
    test_avl_tree_compare();
}

