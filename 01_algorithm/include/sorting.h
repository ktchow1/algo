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
    }







}
