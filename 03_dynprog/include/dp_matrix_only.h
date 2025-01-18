#pragma once
#include<iostream>
#include<cstdint>
#include<vector>
#include<list>
#include<algorithm>

// from alg
#include<matrix.h>
#include<utility.h>


// ***************************** //
// *** Longest common subseq *** //
// ***************************** //
// 1. Various matrix representation
// matrix in "dp_matrix_and_graph.h"  = subproblem size  vs state
// matrix in "dp_matrix_only.h"       = subproblem size  vs subproblem size
// matrix in LCS    and edit dist     = subproblem str0  vs subproblem str1
// matrix in bool() and coin game     = subproblem begin vs subproblem end of the same str
//
// 2. Various matrix scanning
// matrix in LCS    and edit dist : horizontal scan, from top to bottom
// matric in bool() and coin game :   diagonal scan, from middle to upper-right
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


// *************************** //
// *** Boolean parenthesis *** //
// *************************** //
// Expression = input[0].first input[0].second 
//              input[1].first input[1].second
//              ...
//              input[N-2].first input[N-2].second
//              input[N-1].first                   <--- The last logic is don't care.
//
namespace alg
{
    std::uint32_t bool_parenthesis_iterative(const std::vector<bool_symbol>& input) 
    {
        std::uint32_t N = input.size();
        alg::matrix<std::uint32_t> f(N,N,0); // for  true expression
        alg::matrix<std::uint32_t> g(N,N,0); // for false expression

        // Main diagonal
        for(std::uint32_t n=0; n!=N; ++n)
        {
            f(n,n) =  input[n].m_value;
            g(n,n) = !input[n].m_value;
        }

        // Iterate from main diagonal to UR
        for(std::uint32_t diag=1; diag!=N; ++diag) // shift of diagonal = diag
        {
            for(std::uint32_t n=0; n!=N-diag; ++n) // length of diagonal = N-diag
            {
                std::uint32_t m = n+diag;

                // hori_dist between diagonal & (n,m) = m-n
                // vert_dist between diagonal & (n,m) = m-n
                for(std::uint32_t k=0 ; k!=m-n ; ++k)
                {
                    if (input[k].m_logic == logic::OR)
                    {
                        f(n,m) += f(n,n+k) * f(n+k+1,m) +
                                  f(n,n+k) * g(n+k+1,m) +
                                  g(n,n+k) * f(n+k+1,m);
                        g(n,m) += g(n,n+k) * g(n+k+1,m); 
                    }
                    else
                    {
                        f(n,m) += f(n,n+k) * f(n+k+1,m);
                        g(n,m) += g(n,n+k) * g(n+k+1,m) + 
                                  g(n,n+k) * f(n+k+1,m) +
                                  f(n,n+k) * g(n+k+1,m);
                    }
                }
            }
        }
        // most UR corner
        return f(0,N-1); 
    }

    // ************************* //
    // *** Exhaustive search *** //
    // ************************* //
    std::vector<std::vector<std::uint32_t>> permutations(std::uint32_t N)
    {
        std::vector<std::vector<std::uint32_t>> ans;
        if (N == 0) return ans;

        // for N = 1
        for(std::uint32_t n=0; n!=N; ++n)
        {
            std::vector<std::uint32_t> tmp;
            tmp.push_back(n);
            ans.push_back(tmp);
        }
    
        // for N > 1
        for(std::uint32_t n=1; n!=N; ++n)
        {
            std::vector<std::vector<std::uint32_t>> ans0;
            for(auto tmp:ans)
            {
                for(std::uint32_t m=0; m!=N; ++m)
                {
                    tmp.push_back(m);
                    ans0.push_back(tmp);
                }
            }
            ans = std::move(ans0);
        }
        return ans;
    }

    bool evaluate(const std::vector<bool_symbol>& input, const std::vector<std::uint32_t>& perm)
    {
        if (input.size()-1 != perm.size()) 
        {
            throw std::runtime_error("evaluate boolean parenthesis - input and perm size not matched");
        }

        // ************************************************* //
        // *** Convert input into list and list-iterator *** //
        // ************************************************* //
        std::list<bool_symbol> list;
        std::vector<std::list<bool_symbol>::iterator> list_iters;

        for(const auto& x:input)
        {
            list.push_back(x);
        }
        for(auto& x:perm)
        {
            auto iter = list.begin();
            std::advance(iter, x);
            list_iters.push_back(iter);
        }

        // ****************** //
        // *** Evaluation *** //
        // ****************** //
        for(const auto& this_iter : list_iters)
        {
            auto next_iter = this_iter;
            ++next_iter;

            if (this_iter->m_logic == logic::OR)
                 next_iter->m_value = this_iter->m_value || next_iter->m_value;
            else next_iter->m_value = this_iter->m_value && next_iter->m_value;
            list.erase(this_iter);
        }

        // ************** //
        // *** Result *** //
        // ************** //
        if (list.size() != 1)
        {
            throw std::runtime_error("evaluate boolean parenthesis - final list size not equal to 1");
        }
        return list.front().m_value;
    }

    std::uint32_t bool_parenthesis_exhaustive(const std::vector<bool_symbol>& input) 
    {
        auto perms = permutations(input.size()-1); // last OP logic is not used

        std::uint32_t ans = 0;
        for(const auto& perm:perms)
        {
            auto flag = evaluate(input, perm);
            if (flag) ++ans;
        }
        return ans;
    }
}


// ***************** //
// *** Coin game *** //
// ***************** //

namespace alg
{
}

