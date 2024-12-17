#include<iostream>
#include<cstdint>
#include<vector>
#include<stack>
#include<optional>
#include<algorithm>

/*
    Given hist[0:N-1]
    1. subproblem[n] means biggest rect with RHS constrained at n, where n<N
    2. subproblem[n] = std::max(rect[0:n], rect[1,n], ..., rect[2,n], rect[n,n])                      in general, subproblem is solved by O(N) scan
    3.    problem    = std::max(subproblem[0], subproblem[1], ..., subproblem[N-2], subproblem[N-1])  in general,    problem is solved by O(N^2) scan
          problem    = subproblem[N-1]                                                                if hist is increasing
    4. if hist is decreasing :
       subproblem[n] = std::max(subproblemA(n), subproblemB[n])

       where subproblem[A] = biggest rect with RHS constrained at n AND height constrained at hist[n] -> we solve it on the run
             subproblem[B] = biggest rect with RHS constrained at n AND height below hist[n]          -> we move it to the subproblem on increasing hist

    5. subproblemA[n] = find min m, such that hist[k] >= hist[n] for all k in [m,n]
    6. we solve subproblemA in Part1 of our implementation 
       we solve subproblemB together with subproblem on increasing hist in Part2 of our implementation 

    i.e.    biggest_rect_in_hist([1,3,4,2,5,3,1])
    == max( biggest_rect_in_hist([0,3,4,0,0,0,0]),
            biggest_rect_in_hist([1,2,2,2,5,3,1]) )
    == max( biggest_rect_in_hist([0,3,4,0,0,0,0]),
            biggest_rect_in_hist([0,0,0,0,5,3,0]),
            biggest_rect_in_hist([1,2,2,2,3,3,1]) ) 
    == max( biggest_rect_in_hist([0,3,4,0,0,0,0]),
            biggest_rect_in_hist([0,0,0,0,5,3,0]),
            biggest_rect_in_hist([0,2,2,2,3,3,0]),
            biggest_rect_in_hist([1,1,1,1,1,1,1]) ) 

    Thus we use a stack to collect a set of ongoing subproblems in LIFO manner. Why LIFO?
    For example, given a histogram like LHS, the processing order is in RHS :

      xx            11
     xxx  xx       222  44
     xxxx xxx      3333 555
     xxxxxxxx      66666666
    xxxxxxxxxxx   77777777777    which is basically a stack
*/
std::uint32_t biggest_rect_in_histogram(const std::vector<std::uint32_t>& vec)
{
    std::uint32_t ans=0;

    std::stack<std::pair<std::uint32_t, std::uint32_t>> s; // height and location
    for(std::uint32_t n=0; n!=vec.size(); ++n)             // n is the RHS boundary of RECT
    {
        // *** Part1 *** //
        std::optional<std::uint32_t> popped_index = std::nullopt;
        while(!s.empty() && vec[n] <= s.top().first) // BUG : Don't use <, use <=
        {
            {
                auto area = ((n-1) - (s.top().second-1)) * s.top().first;
                ans = std::max(ans, area);
            }
            popped_index = std::make_optional(s.top().second);
            s.pop();
        }
        if (popped_index)
        {
            s.push(std::make_pair(vec[n], *popped_index));
            s.push(std::make_pair(vec[n], n));
        }
        else
        {
            s.push(std::make_pair(vec[n], n));
        }
    }

    // *** Part2 *** //
    while(!s.empty())
    {
        std::uint32_t n=vec.size()-1;
        {
            auto area = (n - (s.top().second-1)) * s.top().first;
            ans = std::max(ans, area);
        }
        s.pop();
    }
    return ans;
}

