// ****************************** //
// *** Max 2-point difference *** //
// ****************************** //
namespace hackerrank
{
    template<typename ITER>
    typename std::iterator_traits<ITER>::value_type max_profit_exhaustive(ITER begin, ITER end)
    {	
        using VALUE = typename std::iterator_traits<ITER>::value_type;

        auto ans = std::numerics::limits<VALUE>::min;
        for(auto i=begin; i!=end-1; ++i)
        {
            for(auto j=i+1; j!=end; ++j)
            {
                sub = *j - *i;
                ans = std::max(ans, sub);
            }
        }
        return ans;
    }

    template<typename ITER>
    typename std::iterator_traits<ITER>::value_type max_profit_divide_conquer(ITER begin, ITER end)
    {	
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        if (begin   == end) return {};
        if (begin+1 == end) return {};

        auto mid = begin + std::distance(begin, end)/2;
        auto ans = std::max_elemnent(mid,end) - std::min_element(begin,mid);
        return std::max(ans, max_profit(begin,mid), max_profit(mid,end));
    }

    template<typename ITER>
    typename std::iterator_traits<ITER>::value_type max_profit(ITER begin, ITER end)
    {	
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        if (begin   == end) return {};
        if (begin+1 == end) return {};

        auto sub = *(begin+1) - *begin;
        auto ans = sub;
        for(auto i=begin+2; i!=end; ++i)
        {
            sub = std::max(sub + *i - *(i-1), *i - *(i-1));
            ans = std::max(ans, sub);
        }
        return ans;
    }
}

// ******************************** //
// *** Count 2-point difference *** //
// ******************************** //
namespace hackerrank
{
    template<typename ITER>
    int count_target_profit(ITER begin, ITER end, const auto& target)
    {	
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::unordered_map<VALUE,int> hist;

        int ans = 0;
        for(auto i=begin; i!=end; ++i)
        {
            auto iter = hist.find(*i - target);
            if (iter != hist.end()) ans += iter->second;            

            iter = hist.find(*i);
            if (iter != hist.end()) ++iter->second;
            else hist[*i] = 1;
        }
        return ans;
    }

    template<typename ITER>
    int count_abs_target_profit(ITER begin, ITER end, const auto& target)
    {	
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::unordered_map<VALUE,int> hist;
       
        int ans = 0;
        for(auto i=begin; i!=end; ++i)
        {
            auto iter = hist.find(*i - target);
            if (iter != hist.end()) ans += iter->second;            

            iter = hist.find(*i + target);
            if (iter != hist.end()) ans += iter->second;            

            iter = hist.find(*i);
            if (iter != hist.end()) ++iter->second;
            else hist[*i] = 1;
        }
        return ans;
    }
}
