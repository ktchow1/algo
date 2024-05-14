#include<iostream>
#include<vector>
#include<string>
#include<stack>
#include<optional>
#include<unordered_map>

std::uint32_t longest_non_duplicated_substr(const std::string& str)
{
    std::unordered_map<char, std::uint32_t> index; // can be replaced by std::array 

    std::uint32_t ans=0;
    for(std::uint32_t n=0; n!=str.size(); ++n) 
    {
        if (auto iter=index.find(str[n]); iter!=index.end())
        {
            ans = std::max(ans, n-iter->second);
        }
        index[str[n]] = n;
    }
    return ans;
}

// This is nearly a exhaustive search, but probably a fast one. There is no promising dynprog.
std::uint32_t longest_palindrome_substr(const std::string& str) 
{
    std::uint32_t ans=1;
    for(std::uint32_t n=1; n!=str.size()-1; ++n) 
    {
        // case 1 : when str[n] is the mirror
        std::uint32_t m=1;
        while(true)
        {
            if (n<m || n+m>=str.size()) 
            {
                break;
            }
            else if (str[n-m]==str[n+m])
            {
                ans = std::max(ans, 2*m+1);
                ++m;
            }
            else
            {
                break;
            }
        }

        // case 2 : when str[n-1] str[n] is the mirror
        // please implement ... 
    }
    return ans;
}

std::uint32_t length_of_minimum_sorting(const std::vector<std::uint32_t>& vec)
{
    // *** Forward *** //
    bool push = true;

    std::stack<std::uint32_t> s0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        while(!s0.empty() && vec[n] < s0.top()) 
        {
            push = false;
            s0.pop(); 
        }
        if (push) 
        {
            s0.push(vec[n]);
        }
    }

    // *** Backward *** //
    push = true;

    std::stack<std::uint32_t> s1;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        while(!s1.empty() && vec[vec.size()-1-n] > s1.top())
        {
            push = false;
            s1.pop();
        }
        if (push)
        {
            s1.push(vec[vec.size()-1-n]);
        }
    }
    return vec.size()-s0.size()-s1.size();
}

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

std::uint32_t max_depth_of_puddle(const std::vector<std::uint32_t>& vec)
{
    std::vector<std::uint32_t> LHS_profile(vec.size(), 0);
    std::vector<std::uint32_t> RHS_profile(vec.size(), 0);
    std::uint32_t LHS_max;
    std::uint32_t RHS_max;

    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        LHS_max = std::max(LHS_max, vec[n]);
        LHS_profile[n] = LHS_max;
    }
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        RHS_max = std::max(RHS_max, vec[vec.size()-n-1]);
        RHS_profile[vec.size()-n-1] = RHS_max;
    }

    std::uint32_t ans=0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        auto temp = std::min(LHS_profile[n], RHS_profile[n]) - vec[n];
        ans = std::max(ans, temp);
    }
    return ans;
}

// ****************************** //
// *** Benchmarking algorithm *** //
// ****************************** //
std::uint32_t length_of_minimum_sorting_exhaustive(const std::vector<std::uint32_t>& vec)
{
    std::vector<std::uint32_t> vec0 = vec;
    std::sort(vec0.begin(), vec0.end());

    std::uint32_t N0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        if (vec[n]!=vec0[n]) { N0 = n; break; }
    }
    std::uint32_t N1;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        if (vec[vec.size()-1-n]!=vec0[vec0.size()-1-n]) { N1 = n; break; }
    }
    return vec.size()-N0-N1;
}

// Exhaustive search for biggest rect[m,n] in 2D loop :
// n = index of RHS boundary of rect
// m = index of LHS boundary of rect, where m < n
//
std::uint32_t biggest_rect_in_histogram_exhaustive(const std::vector<std::uint32_t>& vec)
{
    std::uint32_t min_height;
    std::uint32_t max_area = 0;
    for(std::uint32_t n=0; n!=vec.size(); ++n)
    {
        min_height = vec[n];
        max_area   = std::max(max_area, vec[n]); // BUG : Don't write max_area = vec[n]
        for(std::int32_t m=static_cast<std::int32_t>(n)-1; m>=0; --m)
        {
            min_height = std::min(min_height, vec[m]);
            max_area   = std::max(max_area, (n-m+1)*min_height);
        }
    }
    return max_area;
}

// *********************** //
// *** Test for string *** //
// *********************** //
std::string gen_random_substr(std::uint32_t length)
{
    std::string str;
    for(std::uint32_t n=0; n!=length; ++n)
    {
        str.push_back(static_cast<char>('k' + rand()%10));
    }
    return str;
}

std::string gen_palindrome_substr(std::uint32_t length_of_one_side)
{
    std::string str;
    str.resize(2*length_of_one_side+1);
    for(std::uint32_t n=0; n!=length_of_one_side; ++n)
    {
        str[n] = static_cast<char>('a' + rand()%5);
        str[2*length_of_one_side-n] = str[n];
    }
    str[length_of_one_side] = 'z';
    return str;
}

std::string gen_palindrome_input(std::uint32_t length_of_one_side)
{
    std::string str;
    std::uint32_t N0 = rand()%4;
    std::uint32_t N1 = rand()%4;
    for(std::uint32_t n=0; n!=N0; ++n)
    {
        str.append(gen_random_substr(rand()%10));
        str.append(gen_palindrome_substr(rand()%length_of_one_side));
    }
    str.append(gen_palindrome_substr(length_of_one_side));
    for(std::uint32_t n=0; n!=N1; ++n)
    {
        str.append(gen_random_substr(rand()%10));
        str.append(gen_palindrome_substr(rand()%length_of_one_side));
    }
    return str;
}

void test_longest_palindrome_substr()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        std::uint32_t length_of_one_side = rand()%5 + 5;
        std::string   str = gen_palindrome_input(length_of_one_side);
        std::uint32_t ans = longest_palindrome_substr(str);
        std::cout << "\n" << 2*length_of_one_side+1
                  << ", " << ans
                  << ": " << str 
                  << (ans!=2*length_of_one_side+1? " [ERROR]":"");
    }
}

void test_substr()
{
    test_longest_palindrome_substr();
}

// ********************** //
// *** Test for stack *** //
// ********************** //
std::vector<std::uint32_t> gen_random_vector_for_sorting(std::uint32_t length)
{
    std::vector<std::uint32_t> ans;
    for(std::uint32_t n=0; n!=length; ++n)
    {
        ans.push_back(n);
    }

    std::uint32_t N0 = rand() % (std::uint32_t)(length*0.2) + (std::uint32_t)(length*0.05);
    std::uint32_t N1 = rand() % (std::uint32_t)(length*0.2) + (std::uint32_t)(length*0.05);
    for(std::uint32_t n=0; n!=length; ++n)
    {
        std::uint32_t n0 = N0+rand()%(length-N1-N0);
        std::uint32_t n1 = N0+rand()%(length-N1-N0);
        std::swap(ans[n0], ans[n1]);
    }
    return ans;
}

std::vector<std::uint32_t> gen_random_vector_for_histogram(std::uint32_t length)
{
    std::vector<std::uint32_t> ans;

    std::int32_t height = 12;
    for(std::uint32_t n=0; n!=length; ++n)
    {
        ans.push_back(height);
        height += static_cast<std::int32_t>(rand()%5)-2;
        if (height < 0) height = 0;
    }
    return ans;
}

void test_length_of_minimmum_sorting()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        auto vec = gen_random_vector_for_sorting(80);
        auto ans0 = length_of_minimum_sorting_exhaustive(vec);
        auto ans1 = length_of_minimum_sorting(vec);

        std::cout << "\n" << ans0 << ", " << ans1 << ", vec = ";
        for(const auto& x:vec) std::cout << x << " ";
        if (ans0 != ans1) std::cout << " [ERROR]";
    }
}

void test_biggest_rect_in_histogram()
{
    for(std::uint32_t n=0; n!=300; ++n)
    {
        auto vec = gen_random_vector_for_histogram(100);
        auto ans0 = biggest_rect_in_histogram_exhaustive(vec);
        auto ans1 = biggest_rect_in_histogram(vec);

        std::cout << "\n" << ans0 << ", " << ans1 << ", vec = ";
        for(const auto& x:vec) std::cout << x << " ";
        if (ans0 != ans1) std::cout << " [ERROR]";
    }
}

void test_stack_trick()
{
//  test_length_of_minimmum_sorting();
    test_biggest_rect_in_histogram();
}
