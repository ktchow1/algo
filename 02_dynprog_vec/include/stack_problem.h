#include<iostream>
#include<stack>
#include<vector>


namespace alg
{
    std::uint32_t count_stroke_in_histogram(const std::vector<std::uint32_t>& vec)
    {
        std::stack<std::uint32_t> stack;
        std::uint32_t ans = 0;

        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {
            if (stack.empty())
            {
                if (vec[n] > 0)
                {
                    ans += vec[n];
                    stack.push(vec[n]);
                }
                else
                {
                    // do nothing for vec[n] == 0
                }
            }
            else
            {
                if (stack.top() < vec[n]) 
                {
                    ans += vec[n]-stack.top();
                    stack.push(vec[n]);
                }
                else
                {
                    while(!stack.empty() && stack.top() >= vec[n]) 
                    {
                        stack.pop();
                    }
                    stack.push(vec[n]);
                }
            }
        }
        return ans;
    }
}


// ******************************** //
// *** Benchmark implementation *** //
// ******************************** //
namespace alg
{
    std::uint32_t count_stroke_in_histogram_bmk(const std::vector<std::uint32_t>& vec)
    {
        std::uint32_t ans = 0;
        std::uint32_t lvl = 1;

        while(true)
        {
            bool is_empty = true;
            bool is_stroke = false;

            for(std::uint32_t n=0; n!=vec.size(); ++n)
            {
                if (!is_stroke)
                {
                    if (vec[n] >= lvl)
                    {
                        is_empty = false;
                        is_stroke = true;
                        ++ans;
                    }
                    else
                    {
                        // do nothing
                    }
                }
                else
                {
                    if (vec[n] >= lvl)
                    {
                        is_empty = false;
                    }
                    else
                    {
                        is_stroke = false;
                    }
                }
            }
            if (is_empty) break;
            ++lvl;
        }
        return ans;
    }
}
