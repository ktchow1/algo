#pragma once
#include<iostream>
#include<cstdint>

// from alg
#include<matrix.h>
#include<utility.h>


// *************************************************************************** //
// matrix in "dp_matrix_and_graph.h"  = subproblem size vs state
// matrix in "dp_matrix_only.h"       = subproblem size vs subproblem size
// *************************************************************************** //
namespace alg
{
    std::uint32_t longest_common_subseq_recursive(const std::string& str0, const std::string& str1)
    {
        if (str0.size()==0 || str1.size()==0) return 0;

        std::string trim_str0(str0); 
        std::string trim_str1(str1);
        trim_str0.erase(trim_str0.begin());
        trim_str1.erase(trim_str1.begin());

        if (str0[0] == str1[0]) 
        {
            return longest_common_subseq_recursive(trim_str0, trim_str1) + 1;
        }
        else
        {
            return std::max(longest_common_subseq_recursive(trim_str0, str1), 
                            longest_common_subseq_recursive(str0, trim_str1));
        }
    }

    std::uint32_t longest_common_subseq_iterative(const std::string& str0, const std::string& str1)
    {
        alg::matrix<std::uint32_t> mat(str1.size(), str0.size(), 0);
        mat(0,0) = (str0[0]==str1[0]);

        for(std::uint32_t m=1; m!=str0.size(); ++m)
        {
            std::uint32_t x = (str0[m]==str1[0]);
            mat(0,m) = std::max(mat(0,m-1), x);
        }

        for(std::uint32_t n=1; n!=str1.size(); ++n)
        {
            std::uint32_t x = (str0[0]==str1[n]);
            mat(n,0) = std::max(mat(n-1,0), x);
        }

        for(std::uint32_t n=1; n!=str1.size(); ++n)
        {
            for(std::uint32_t m=1; m!=str0.size(); ++m)
            {
                if (str0[m]==str1[n])
                {
                    mat(n,m) = mat(n-1,m-1) + 1;
                }
                else
                {
                    mat(n,m) = std::max(mat(n-1,m), mat(n,m-1));
                }
            }
        }
        return mat(mat.size_y()-1, mat.size_x()-1);
    }
}

