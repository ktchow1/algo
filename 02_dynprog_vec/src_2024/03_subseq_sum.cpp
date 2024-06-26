// ********************** //
// *** Max subseq sum *** //
// ********************** //
namespace hackerrank
{
    template<typename ITER>
    typename std::iterator_traits<ITER>::value_type max_subseq_sum_exhaustive(ITER begin, ITER end)
    {	
        using VALUE = typename std::iterator_traits<ITER>::value_type;

        auto ans = std::numerics::limits<VALUE>::min;
        for(auto i=begin; i!=end; ++i) // subseq = i,i+1,...,j
        {
            auto cum = typename std::iterator_traits<ITER>::value_type{};
            for(auto j=i; j!=end; ++j) 
            {
                cum += *j;
                ans = std::max(ans, cum);
            }
        }
        return ans;
    }
    // Remark :
    // we cannot let j be subseq.end()
    // we should let j be subseq.back()
    // since the former violates the inner for loop condition : j!=end

    template<typename ITER>
    typename std::iterator_traits<ITER>::value_type max_subseq_sum(ITER begin, ITER end)
    {	
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        if (begin == end) return {};

        auto sub = *begin;
        auto ans = sub;
        for(auto i=begin+1; i!=end; ++i)
        {
            sub = std::max(sub + *i, *i);
            ans = std::max(ans, sub);
        }
        return ans;
    }

    template<typename ITER>
    typename std::iterator_traits<ITER>::value_type max_subseq_product(ITER begin, ITER end)
    {	
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        if (begin == end) return {};

        auto sub0 = *begin;
        auto sub1 = *begin;
        auto ans  = sub0;
        for(auto i=begin+1; i!=end; ++i)
        {
            auto tmp0 = sub0 * *i;
            auto tmp1 = sub1 * *i;
            sub0 = std::max(tmp0, tmp1, *i);
            sub1 = std::min(tmp0, tmp1, *i);
            ans  = std::max(ans, sub0);
        }
        return ans;
    }

    template<typename ITER>
    typename std::iterator_traits<ITER>::value_type max_noncontiguous_subseq_sum(ITER begin, ITER end)
    {	
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        if (begin == end) return {};

        auto sub0 = *begin;  // with current value
        auto sub1 = VALUE{}; // without current value
        auto ans  = sub0;
        for(auto i=begin+1; i!=end; ++i)
        {
            auto tmp0 = sub0;
            auto tmp1 = sub1;
            sub0 = std::max(tmp0 + *i, tmp1 + *i, *i);
            sub1 = std::max(tmp0, tmp1);
            ans  = std::max(ans, sub0, sub1);
        }
        return ans;
    }

    template<typename ITER>
    typename std::iterator_traits<ITER>::value_type max_strictly_noncontiguous_subseq_sum(ITER begin, ITER end)
    {	
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        if (begin == end) return {};


        auto sub0 = *begin;  // with current value
        auto sub1 = VALUE{}; // without current value
        auto ans  = sub0;
        for(auto i=begin+1; i!=end; ++i)
        {
            auto tmp0 = sub0;
            auto tmp1 = sub1;
            sub0 = std::max(tmp1 + *i, *i);
            sub1 = std::max(tmp0, tmp1);
            ans  = std::max(ans, sub0, sub1);
        }
        return ans;
    }
}

// ************************ //
// *** Count subseq sum *** //
// ************************ //
namespace hackerrank
{
    template<typename ITER>
    int count_target_subseq_sum(ITER begin, ITER end, const auto& target)
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::unordered_map<VALUE,int> hist;

        hist[0] = 1; // Dont miss this case
        auto cum = 0;
        int ans = 0; 
        for(auto i=begin; i!=end; ++i) 
        {
            cum += *i;
            if (auto iter = hist.find(cum-target); iter!= hist.end())
            {
                ans += iter->second;
            }
            if (auto iter = hist.find(cum); iter!= hist.end())
            {
                ++iter->second;
            }
            else hist[cum] = 1;
        }
        return ans;
    }

    int count_target_divisible_subseq_sum(ITER begin, ITER end, const auto& target)
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::unordered_map<VALUE,int> hist;

        hist[0] = 1; // Dont miss this case
        auto cum = 0;
        int ans = 0; 
        for(auto i=begin; i!=end; ++i) 
        {
            cum += *i;
            auto mod = cum % target
            if (auto iter = hist.find(mod); iter!= hist.end())
            {
                ans += iter->second;
                ++iter->second;
            }
            else hist[mod] = 1;
        }
        return ans;
    }

    template<typename ITER>
    int longest_target_subseq_sum(ITER begin, ITER end, const auto& target)
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::unordered_map<VALUE,int> index;

        index[0] = -1; // Dont miss this case
        auto cum = 0;
        int ans = 0; 
        int n = 0;
        for(auto i=begin; i!=end; ++i, ++n) 
        {
            cum += *i;
            if (auto iter = index.find(cum-target); iter!= index.end())
            {
                ans = std::max(ans, n-iter->second);
            }
            if (auto iter = index.find(cum); iter == index.end())
            {
                index[cum] = n;
            }
        }
        return ans;
    }

    // Assuming all inputs are positive, i.e. cum is monotonic increasing.
    // Similar to previous question, but use ordered map instead, search time is O(logN).
    template<typename ITER>
    int count_less_than_or_equal_to_target_subseq_sum(ITER begin, ITER end, const auto& target) 
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::map<VALUE,int> index; 

        index[0] = -1; // Dont miss this case
        auto cum = 0;
        int ans = 0; 
        int n = 0;
        for(auto i=begin; i!=end; ++i) 
        {
            cum += *i;
            if (auto iter = index.lower_bound(cum-target); iter!= index.end())
            {
                ans += n-iter->second;
            }
            if (auto iter = index.find(cum); iter == index.end())
            {
                index[cum] = n;
            }
        }
        return ans;
    }

    // Assuming all inputs are positive, i.e. cum is monotonic increasing.
    // Similar to previous question, but use ordered map instead, search time is O(logN).
    template<typename ITER>
    int count_less_than_or_equal_to_target_subseq_product(ITER begin, ITER end, const auto& target) 
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::map<VALUE,int> index; 

        index[1] = -1; // Dont miss this case
        auto cum = 1;
        int ans = 0; 
        int n = 0;
        for(auto i=begin; i!=end; ++i) 
        {
            cum *= *i;
            int temp = int(std::ceil((double)cum/target));
            if (auto iter = index.lower_bound(temp); iter!= index.end())
            {
                ans += n-iter->second;
            }
            if (auto iter = index.find(cum); iter == index.end())
            {
                index[cum] = n;
            }
        }
        return ans;
    }
}
