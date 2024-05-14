#include<iostream>
#include<vector>


//               fast    medium  slow    inplace  iter stable
// ----------------------------------------------------------
// select sort   O(N^2)  O(N^2)  O(N^2)  yes   forward     no
// bubble sort   O(N)    O(N^2)  O(N^2)  yes  bidirect    yes
// insert sort   O(N)    O(N^2)  O(N^2)  yes  bidirect    yes
// quick sort    O(lnN)  O(lnN)  O(N^2)  yes  bidirect     no
// merge sort    O(lnN)  O(lnN)  O(lnN)   no    random    yes
// heap sort     O(lnN)  O(lnN)  O(lnN)  yes    random     no
// ----------------------------------------------------------

template<typename ITER> void select_sort(ITER& begin, ITER& end)
{
    for(ITER i=begin; i!=end; ++i)
    {
        ITER j=i; 
        ITER max_iter = i;
        for(++j; j!=end; ++j)
        {
            if (*max_iter < *j)
                 max_iter =  j;
        }
        std::swap(*i, *max_iter);
    }
}

/*
template<typename ITER> void bubble_sort(ITER& begin, ITER& end)
{
}*/


