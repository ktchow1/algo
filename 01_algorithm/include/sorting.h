#include<iostream>
#include<functional>
#include<vector>
#include<algorithm>


namespace alg
{
    template<typename T>
    struct less_comparator : std::function<bool(const T&, const T&)>
    {
        bool operator()(const T& lhs, const T& rhs)
        {
            return lhs < rhs;
        }
    };

    template<typename ITER, typename CMP = less_comparator<typename std::iterator_traits<ITER>::value_type>>
    void select_sort(ITER begin, ITER end) // forward iterator
    {
        for(ITER i=begin; i!=end; ++i)
        {
            ITER min_iter=i;
            ITER j=i;
            for(++j; j!=end; ++j)
            {
                if (CMP{}(*j, *min_iter))
                {
                    min_iter = j;
                }
            }
            std::swap(*i, *min_iter);
        }
    }

    template<typename ITER, typename CMP = less_comparator<typename std::iterator_traits<ITER>::value_type>>
    void bubble_sort(ITER begin, ITER end) // bidirectional iterator
    {
        for(ITER i=begin; i!=end; ++i)
        {
            ITER j=end;
            for(--j; j!=i; --j)
            {
                if (CMP{}(*j, *(j-1)))
                {
                    std::swap(*j, *(j-1));
                }
            }
        }
    }

    template<typename ITER, typename CMP = less_comparator<typename std::iterator_traits<ITER>::value_type>>
    void insert_sort(ITER begin, ITER end) // bidirectional iterator
    {
        for(ITER i=begin; i!=end; ++i)
        {
            for(ITER j=i; j!=begin; --j)
            {
                if (CMP{}(*j, *(j-1)))
                {
                    std::swap(*j, *(j-1));
                }
                else break;
            }
        }
    }

    template<typename ITER, typename CMP = less_comparator<typename std::iterator_traits<ITER>::value_type>>
    void quick_sort(ITER begin, ITER end) // bidirectional iterator
    {
        if (begin==end) return; // BUG : without this checking, result in infinite recursion for size 0 input
        ITER i = begin;
        ITER j = end;
        --j;

        if (begin==j)   return; // BUG : without this checking, result in infinite recursion for size 1 input
        while(i!=j)
        {
            if (CMP{}(*i, *j))
            {
                --j;
            }
            else
            {
                auto x = *i;
                *i = *j;
                *j = *(i+1);
                *(i+1) = x;
                ++i;
            }
        }

        // recursion
        quick_sort(begin,i);
        quick_sort(i+1,end);
    //  quick_sort(i  ,end); // BUG : with this line, result in infinite recursion for size 2 input 
    }

    template<typename ITER, typename OITER, typename CMP = less_comparator<typename std::iterator_traits<ITER>::value_type>>
    void merge(ITER begin0, ITER end0, ITER begin1, ITER end1, OITER oiter) // forward iterator
    {
        while(begin0!=end0 && begin1!=end1)
        {
            if (CMP{}(*begin0, *begin1))
            {
                *oiter = *begin0;
                ++begin0;
                ++oiter;
            }
            else
            {
                *oiter = *begin1;
                ++begin1;
                ++oiter;
            }
        }
        while(begin0!=end0)
        {
            *oiter = *begin0;
            ++begin0;
            ++oiter;
        }
        while(begin1!=end1)
        {
            *oiter = *begin1;
            ++begin1;
            ++oiter;
        }
    }

    template<typename ITER, typename CMP = less_comparator<typename std::iterator_traits<ITER>::value_type>>
    void merge_sort(ITER begin, ITER end) // random access iterator
    {
        if (begin==end)   return; // BUG : without this checking, result in infinite recursion for size 0 input
        if (begin==end-1) return; // BUG : without this checking, result in infinite recursion for size 1 input

        ITER mid = begin;
        std::advance(mid, std::distance(begin, end)/2);

        merge_sort(begin, mid); // BUG : you forget this recursion
        merge_sort(mid, end);   // BUG : you forget this recursion

        std::vector<typename std::iterator_traits<ITER>::value_type> vec;
        merge(begin, mid, mid, end, std::back_inserter(vec));
        std::copy(vec.begin(), vec.end(), begin);
    }
}