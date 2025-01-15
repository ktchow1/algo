#pragma once
#include<iostream>
#include<cstdint>

// from alg
#include<matrix.h>
#include<utility.h>


// ***************************** //
// *** Longest common subseq *** //
// ***************************** //
// matrix in "dp_matrix_and_graph.h"  = subproblem size vs state
// matrix in "dp_matrix_only.h"       = subproblem size vs subproblem size
//
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
            if (str0[m]==str1[0]) mat(0,m) = 1;
            else                  mat(0,m) = mat(0,m-1);
        }

        for(std::uint32_t n=1; n!=str1.size(); ++n)
        {
            if (str0[0]==str1[n]) mat(n,0) = 1;
            else                  mat(n,0) = mat(n-1,0);
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


// ********************* //
// *** Edit distance *** //
// ********************* //
// Recursion of edit distance is slower than recursion of LCS because of 1-to-3 in recursion equation.
//
namespace alg
{
    std::uint32_t edit_distance_recursive(const std::string& str0, const std::string& str1)
    {
        if (str0.size()==0 && str1.size()==0) return 0;
        if (str0.size()!=0 && str1.size()==0) return str0.size();
        if (str0.size()==0 && str1.size()!=0) return str1.size();

        std::string trim_str0(str0); 
        std::string trim_str1(str1);
        trim_str0.erase(trim_str0.begin());
        trim_str1.erase(trim_str1.begin());

        if (str0[0] == str1[0]) 
        {
            return edit_distance_recursive(trim_str0, trim_str1);
        }
        else
        {
            return std::min(edit_distance_recursive(trim_str0,      str1) + 1, // trim_str0 becomes str1, plus DEL
                   std::min(edit_distance_recursive(     str0, trim_str1) + 1, // str0 becomes trim_str1, plus ADD
                            edit_distance_recursive(trim_str0, trim_str1) + 1));
        }
    }

    std::uint32_t edit_distance_iterative(const std::string& str0, const std::string& str1)
    {
        alg::matrix<std::uint32_t> mat(str1.size(), str0.size(), alg::inf<std::uint32_t>);
        if (str0[0]==str1[0]) mat(0,0) = 0;
        else                  mat(0,0) = 1;

        for(std::uint32_t m=1; m!=str0.size(); ++m)
        {
            if (str0[m]==str1[0]) mat(0,m) = m;
            else                  mat(0,m) = mat(0,m-1) + 1;
        }

        for(std::uint32_t n=1; n!=str1.size(); ++n)
        {
            if (str0[0]==str1[n]) mat(n,0) = n;
            else                  mat(n,0) = mat(n-1,0) + 1;
        }

        for(std::uint32_t n=1; n!=str1.size(); ++n)
        {
            for(std::uint32_t m=1; m!=str0.size(); ++m)
            {
                if (str0[m]==str1[n])
                {
                    mat(n,m) = mat(n-1,m-1);
                }
                else
                {
                    mat(n,m) = std::min(mat(n-1,m) + 1, 
                               std::min(mat(n,m-1) + 1,
                                        mat(n-1,m-1) + 1));
                }
            }
        }
        return mat(mat.size_y()-1, mat.size_x()-1);
    }
}
