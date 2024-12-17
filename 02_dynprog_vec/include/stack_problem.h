#include<iostream>
#include<stack>
#include<vector>
#include<algorithm>


namespace alg
{
    std::uint32_t shortest_unsorted_subseq(const std::vector<std::uint32_t>& vec)
    {
        // *************** //
        // *** Forward *** //
        // *************** //
        std::stack<std::uint32_t> s0;
        bool flag0 = true;

        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {

            const auto& x = vec[n];
            if (s0.empty())
            {
                if (flag0) s0.push(x);
            }
            else if (x >= s0.top())
            {
                if (flag0) s0.push(x);
            }
            else
            {
                while(!s0.empty() && x < s0.top())
                {
                    s0.pop();
                }
                flag0 = false;
            }
        }

        // **************** //
        // *** Backward *** //
        // **************** //
        std::stack<std::uint32_t> s1;
        bool flag1 = true;

        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {
            const auto& x = vec[vec.size()-n-1];
            if (s1.empty())
            {
                if (flag1) s1.push(x);
            }
            else if (x <= s1.top())
            {
                if (flag1) s1.push(x);
            }
            else
            {
                while(!s1.empty() && x > s1.top())
                {
                    s1.pop();
                }
                flag1 = false;
            }
        }
        return vec.size() - s0.size() - s1.size();
    }

    std::uint32_t count_stroke_in_histogram(const std::vector<std::uint32_t>& vec)
    {
        std::stack<std::uint32_t> s;
        std::uint32_t ans = 0;

        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {
            if (s.empty())
            {
                if (vec[n] > 0)
                {
                    ans += vec[n];
                    s.push(vec[n]);
                }
            }
            else if (vec[n] > s.top()) 
            {
                ans += vec[n]-s.top();
                s.push(vec[n]);
            }
            else
            {
                while(!s.empty() && vec[n] <= s.top()) 
                {
                    s.pop();
                }
                s.push(vec[n]);
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
    std::uint32_t shortest_unsorted_subseq_bmk(const std::vector<std::uint32_t>& vec)
    {
        std::vector<std::uint32_t> sorted_vec(vec);
        std::sort(sorted_vec.begin(), sorted_vec.end());

        // *************** //
        // *** Forward *** //
        // *************** //
        std::uint32_t unsorted_begin = vec.size();
        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {
            if (vec[n] != sorted_vec[n])
            {
                unsorted_begin = n;
                break;
            }
        }
        if (unsorted_begin == vec.size()) return 0;

        // **************** //
        // *** Backward *** //
        // **************** //
        std::uint32_t unsorted_last = vec.size();
        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {
            if (vec[vec.size()-n-1] != sorted_vec[vec.size()-n-1])
            {
                unsorted_last = vec.size()-n-1;
                break;
            }
        }
        if (unsorted_last == vec.size()) return 0; // <---- should never happen

        return unsorted_last - unsorted_begin + 1;
    }

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
