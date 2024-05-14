#include<iostream>
#include<vector>


// Bisection as outer loop, using index x0,x1.
// Quicksort as inner loop, using index i,j.
//
// Time complexity = O(N)+O(N/2)+O(N/4)+... = O(2N) = O(N)
//
std::uint32_t get_K_order(const std::vector<std::uint32_t>& vec, std::uint32_t K) // K = [0,N-1]
{
    auto v = vec;
    std::uint32_t bisect_x0 = 0;
    std::uint32_t bisect_x1 = v.size()-1;


    while(bisect_x0 != bisect_x1)
    {
        // *** Quciksort *** //
        std::uint32_t i=bisect_x0;
        std::uint32_t j=bisect_x1;
        while(i!=j)
        {
            if (v[i] <= v[j])
            {
                --j;
            }
            else
            {
                auto temp = v[i];
                v[i] = v[j];
                v[j] = v[i+1];
                v[i+1] = temp;
                ++i;
            }
        }

        // *** Bisection *** // 
        if (i==K)
        {
            return v[i]; // luckily, we get the answer before narrowing down search range to 1 
        }
        else if (i<K)
        {
            bisect_x0 = i+1;
        }
        else
        {
            bisect_x1 = i-1;
        }
    }
    return v[bisect_x0]; // normally, need to iterate through bisection until search range is 1 
}

std::uint32_t get_K_order_by_sorting(const std::vector<std::uint32_t>& vec, std::uint32_t K)
{
    auto v = vec;
    std::sort(v.begin(), v.end());
    return v[K];
}

// Since : 
// n >= vec[n]-2
// m >= vec[m]-2 
//
// if we need to check : vec[n] < vec[m], then m must fulfill
// 
// m >= vec[m]-2 > vec[n]-2
//
std::int32_t num_of_bribes(const std::vector<std::uint32_t>& vec_original_position)
{
    // Step1 : check n >= vec_original_position[n] - 2
    for(std::uint32_t n=0; n!=vec_original_position.size(); ++n) 
    {
        if (n+2 < vec_original_position[n]) return -1; // instead of checking n < vec_...[n]-2, as the latter may underflow 
    } 

    // Step2 : count number of swap
    std::int32_t ans=0;
    for(std::uint32_t n=0; n!=vec_original_position.size(); ++n)
    {
        std::uint32_t m0 = 0;
        if (vec_original_position[n] > 2)
        {
            m0 = vec_original_position[n] - 2;
        }
        for(std::uint32_t m=m0; m!=n; ++m) // for m<n
        {
            if (vec_original_position[n]<vec_original_position[m]) ++ans;
        }
    }
    return ans;
}

std::int32_t num_of_bribes_not_optimized0(const std::vector<std::uint32_t>& vec_original_position)
{
    // Step2 : count number of swap
    std::int32_t ans=0;
    for(std::uint32_t n=0; n!=vec_original_position.size(); ++n)
    {
        for(std::uint32_t m=0; m!=n; ++m) // for m<n
        {
            if (vec_original_position[n]<vec_original_position[m]) ++ans;
        }
    }
    return ans;
}

std::int32_t num_of_bribes_not_optimized1(const std::vector<std::uint32_t>& vec_original_position)
{
    // Step2 : count number of swap
    std::int32_t ans=0;
    for(std::uint32_t n=0; n!=vec_original_position.size(); ++n)
    {
        for(std::uint32_t m=n+1; m!=vec_original_position.size(); ++m) // for m>n
        {
            if (vec_original_position[n]>vec_original_position[m]) ++ans;
        }
    }
    return ans;
}

// ************ //
// *** Test *** //
// ************ //
std::vector<std::uint32_t> gen_random_vector(std::uint32_t length)
{
    std::vector<std::uint32_t> ans;
    for(std::uint32_t n=0; n!=length; ++n)
    {
        ans.push_back(rand()%500);
    }
    return ans;
}

std::vector<std::uint32_t> gen_random_bribes(std::uint32_t length)
{
    std::vector<std::uint32_t> ans;
    for(std::uint32_t n=0; n!=length; ++n)
    {
        ans.push_back(n);
    }

    std::uint32_t N=rand()%length;
    for(std::uint32_t n=0; n!=N; ++n)
    {
        auto temp = rand()%3;
        if (temp==1 && n>=1)
        {
            std::swap(ans[n],ans[n-1]);
        }
        else if (temp==2 && n>=2)
        {
            std::swap(ans[n],ans[n-1]);
            std::swap(ans[n-1],ans[n-2]);
        }
    }
    return ans;
}

void test_get_K_order()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        auto vec = gen_random_vector(100); std::uint32_t K = 40;
        auto ans0 = get_K_order_by_sorting(vec, K);
        auto ans1 = get_K_order(vec, K);

        std::cout << "\n" << ans0 << ", " << ans1 << ", vec = ";
        for(const auto& x:vec) std::cout << x << " ";
        if (ans0 != ans1) std::cout << " [ERROR]";
    }
}

void test_num_of_bribes()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        auto vec = gen_random_bribes(100); 
        auto ans0 = num_of_bribes(vec);
        auto ans1 = num_of_bribes_not_optimized0(vec);
        auto ans2 = num_of_bribes_not_optimized1(vec);

        std::cout << "\n" << ans0 << ", " << ans1 << ", " << ans2 << ", vec = ";
        for(const auto& x:vec) std::cout << x << " ";
        if (ans0 != ans1 || ans1 != ans2) std::cout << " [ERROR]";
    }
}

void test_sorting_trick()
{
//  test_get_K_order();
    test_num_of_bribes();
}
