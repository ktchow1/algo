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
        bool push0 = true;

        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {
            const auto& x = vec[n];
            while(!s0.empty() && x < s0.top())
            {
                push0 = false;
                s0.pop();
            }
            if (push0)
            {
                s0.push(x);
            }
        }
        if (s0.size()==vec.size()) return 0;

        // **************** //
        // *** Backward *** //
        // **************** //
        std::stack<std::uint32_t> s1;
        bool push1 = true;

        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {
            const auto& x = vec[vec.size()-n-1];
            while(!s1.empty() && x > s1.top())
            {
                push1 = false;
                s1.pop();
            }
            if (push1)
            {
                s1.push(x);
            }
        }
        if (s1.size()==vec.size()) return 0; // <---- should never happen

        return vec.size() - s0.size() - s1.size();
    }

    std::uint32_t count_stroke_in_histogram(const std::vector<std::uint32_t>& vec)
    {
        std::stack<std::uint32_t> s;
        std::uint32_t ans = 0;

        for(std::uint32_t n=0; n!=vec.size(); ++n)
        {
            const auto& x = vec[n];

            // Tried to re-order following if-case like the above algo, unfortunately does not work.
            if (s.empty())
            {
                if (x > 0) 
                {
                    ans += x;
                    s.push(x);
                }
            }
            else if (x > s.top())
            {
                if (x > 0) 
                {
                    ans += x-s.top();
                    s.push(x);
                }
            }
            else
            {
                while(!s.empty() && x <= s.top()) 
                {
                    s.pop();
                }
                if (x > 0) 
                {
                    s.push(x);
                }
            }
        }
        return ans;
    }

    std::uint32_t biggest_rectangle_in_histogram(const std::vector<std::uint32_t>& vec)
    {
        std::uint32_t ans;
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

    std::uint32_t biggest_rectangle_in_histogram_bmk(const std::vector<std::uint32_t>& vec)
    {
        std::uint32_t ans;
        return ans;
    }
}
