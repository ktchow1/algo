// *********************** // 
// *** Max 2-point sum *** //
// *********************** // 
namespace hackerrank
{
    //   max vec[n]+vec[m]+(m-n) where n<=m (equality makes this question easier)
    // = max vec[n]+n + max vec[m]-m
    // = max vec[n]+n for n>k
    // + max vec[m]-m for m<k, where k = arg max vec 
    template<typename ITER>
    auto max_2_sum_distance(ITER begin, ITER end) 
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        auto max_iter = std::max_element(begin, end);
        auto max_iter_dist = std::distance(begin, max_iter);
        auto max0 = *begin + 0;
        auto max1 = *max_iter - max_iter_dist;

        int n=0;
        for(auto i=begin; i!=max_iter+1; ++i, ++n)
        {
            if (max0 < *i + n)
                max0 = *i + n;
        }

        n = max_iter_dist;
        for(auto i=max_iter; i!=end; ++i, ++n)
        {
            if (max1 < *i - n)
                max1 = *i - n;
        }
        return max0 + max1;
    }


    int sum_of_digits(int x)
    {
        int sum = 0;
        while(x > 0)
        {
            sum += x % 10;
            x = x/10;
        }
        return sum;
    }

    template<typename ITER>
    auto max_2_sum_with_same_digit_sum(ITER begin, ITER end) // Assume all numbers are positive
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::unordered_map<int, std::pair<int,int>> hist; 
        // K    = sum of digits
        // V    = 1st and 2nd largest number with that sum of digits
        // i    = input iterator
        // iter = hist  iterator

        for(ITER i=begin; i!=end; ++i)
        {
            auto sum = sum_of_digits(*i); 
            auto iter = hist.find(sum); bool flag;
            if (iter == hist.end()) 
            {
                std::tie(iter, flag) = hist.insert({ sum, std::make_pair(-1,-1) });
            }

            // case 1 : no 1st no 2nd number
            if (iter->second.first == -1)
            {
                iter->second.first = *i;
            }
            // case 2 : no 2nd number
            else if (iter->second.second == -1)
            {
                iter->second.second = *i;
            }
            // case 3 : both numbers exist
            else
            {
                if (*i > iter->second.first)
                {
                    iter->second.second = iter->second.first;
                    iter->second.first = *i;
                }
                else if (*i > iter->second.second)
                {
                    iter->second.second = *i;
                }
            //  else ignored
            }
        }
        
        VALUE max = -1; // if no 2 numbers have same sum-of-digits, return -1
        for(auto iter=hist.begin(); iter!=hist.end(); ++iter)
        {
            if (iter->second.first  != -1 &&
                iter->second.second != -1)
            {
                auto temp = iter->second.first + iter->second.second;
                if (max < temp)
                    max = temp;
            }
        }
        return max;
    }

    template<typename MATRIX>
    auto max_2_sum_with_non_attacking_rooks(const MATRIX& mat) 
    {
        int ans = 0;
        std::vector<std::pair<int,int>> row_max;
        std::vector<std::pair<int,int>> col_max; 

        // step 1 : find max and 2nd max in each row
        for(int y=0; y!=mat.size_y(); ++y)
        {
            int max0 = -1; int x0 = -1; //     max-value and     max-index of a row
            int max1 = -1; int x1 = -1; // 2nd max-value and 2nd max-index of a row
            for(int x=0; x!=mat.size_x(); ++x)
            {
                if      (max0 == -1) { max0 = mat[y][x]; x0 = x; }
                else if (max1 == -1) { max1 = mat[y][x]; x1 = x; }
                else 
                {
                    if   (max0 < mat[y][x]) {  max1 = max0; max0 = mat[y][x]; x1 = x0; x0 = x; }
                    else (max1 < mat[y][x]) {               max1 = mat[y][x];          x1 = x; } 
                }
            }
            row_max.push_back(std::make_pair(x0, x1));
        }

        // step 2 : find max and 2nd max in each col
        // todo   : repeat step 1
        
        // step 3 : assume 1st rook is the max of a row
        // rook0 = {y0,x0}
        // rook1 = {y1,x1}
        for(int y0=0; y0!=mat.size_y(); ++y0)  
        {
            int x0 = row_max[y0].first;
            for(int x1=0; x1!=mat.size_x(); ++x1)
            {
                if (x1 != x0)
                {
                    if (col_max[x1].first != y0) ans = std::max(ans, mat[y0][x0] + mat[col_max[x1].first ][x1]);
                    else                         ans = std::max(ans, mat[y0][x0] + mat[col_max[x1].second][x1]);
                }
            }
        }

        // step 4 : assume 1st rook is the 2nd max of a row
        // todo   : repeat step 3
        return ans;
    }
}

// ******************************** // 
// *** Find / count 2-point sum *** //
// ******************************** // 
namespace hackerrank
{
    template<typename ITER>
    auto find_target_2_sum_in_1_sorted_array(ITER begin, ITER end, const auto& target)
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        if (begin   == end) return std::make_pair(end,end);
        if (begin+1 == end) return std::make_pair(end,end);
        
        ITER i = begin;
        ITER j = end; --j;
        while(i!=j)
        {
            auto temp = *i + *j;
            if      (temp < target) ++i;
            else if (temp > target) --j; 
            else return std::make_pair(i,j);
        }
        return std::make_pair(end,end);
    }

    template<typename ITER>
    auto find_target_2_sum_in_2_sorted_array(ITER begin0, ITER end0, 
                                             ITER begin1, ITER end1,
                                             const auto& target)
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        if (begin0 == end0) return std::make_pair(end,end);
        if (begin1 == end1) return std::make_pair(end,end);
        
        ITER i = begin0;
        ITER j = end1-1;
        while(i!=end0-1 && j!=begin1)
        {
            auto temp = *i + *j;
            if      (temp < target) ++i;
            else if (temp > target) --j; 
            else return std::make_pair(i,j);
        }
        while(i!=end0-1)
        {
            auto temp = *i + *j;
            if      (temp < target) ++i;
            else if (temp > target) return std::make_pair(end,end); 
            else return std::make_pair(i,j);
        }
        while(j!=begin1)
        {
            auto temp = *i + *j;
            if      (temp < target) return std::make_pair(end,end);
            else if (temp > target) --j; 
            else return std::make_pair(i,j);
        }
        return std::make_pair(end,end);
    }

    template<typename ITER>
    int count_target_2_sum_in_unsorted_array(ITER begin, ITER end, const auto& target)
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::unordered_map<VALUE,int> hist;

        int ans = 0;
        for(auto i=begin; i!=end; ++i) // O(n)
        {
            auto iter = hist.find(target - *i);
            if (iter != hist.end()) ans += iter->second;            

            iter = hist.find(*i);
            if (iter != hist.end()) ++iter->second;
            else hist[*i] = 1;
        }
        return ans;
    }

    template<typename ITER>
    int count_target_3_sum_in_unsorted_array(ITER begin, ITER end, const auto& target)
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::unordered_map<VALUE,int> hist;

        int ans = 0;
        for(auto i=begin; i!=end; ++i) // O(n^2) : hist's element's index < i < j
        {
            for(auto j=i+1; j!=end; ++j) 
            {
                auto iter = hist.find(target - *i - *j);
                if (iter != hist.end()) ans += iter->second;            

                iter = hist.find(*i); // hist records visited element with index < i < j
                if (iter != hist.end()) ++iter->second;
                else hist[*i] = 1;
            }
        }
        return ans;
    }

    template<typename ITER>
    int count_target_4_sum_in_unsorted_array(ITER begin, ITER end, const auto& target)
    {
        using VALUE = typename std::iterator_traits<ITER>::value_type;
        std::unordered_map<VALUE, std::unordered_set<std::pair<ITER,ITER>>> hist;

        // step 1 : preprocessing
        for(auto i=begin; i!=end; ++i) 
        {
            for(auto j=i+1; j!=end; ++j) 
            {
                auto temp = *i + *j;
                auto iter = hist.find(temp);
                if (iter != hist.end()) 
                    iter->second.insert(std::make_pair(i,j)); // always fulfill i < j (strictly)
                else  hist[temp].insert(std::make_pair(i,j));
            }
        }

        // step 2 : counting
        int ans = 0;
        for(auto i=begin; i!=end; ++i) 
        {
            for(auto j=i+1; j!=end; ++j) 
            {
                auto iter = hist.find(target - *i - *j);
                if (iter != hist.end()) 
                {
                    for(const auto& x:iter->second)
                    {
                        if (i!=x.first && i!=x.second &&
                            j!=x.first && j!=x.second) ++ans;            
                    }
                }
            }
        }
        return ans;
    }
}
