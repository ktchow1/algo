#include<iostream>
#include<cassert>
#include<sorting.h>
#include<utility.h>


template<typename CONTAINER>
bool compare(const CONTAINER& c0, const CONTAINER& c1)
{
    if (c0.size() != c1.size()) return false;

    auto i0 = c0.begin();
    auto i1 = c1.begin();
    for(; i0!=c0.end(); ++i0, ++i1)
    {
        if (*i0 != *i1) return false;
    }
    return true;
}

template<typename CONTAINER>
void print(const std::string& header, const CONTAINER& c)
{
    std::uint32_t n=0;
    std::uint32_t N=c.size();

    std::cout << header << "["; 
    for(const auto& x:c)
    {
        if (n!=N) std::cout << x << ",";
        else      std::cout << x << "]";
        ++n;
    }
}


void test_sorting()
{
    std::uint32_t trial = 10000;
    std::uint32_t error0 = 0;
    std::uint32_t error1 = 0;
    std::uint32_t error2 = 0;
    std::uint32_t error3 = 0;
    std::uint32_t error4 = 0;
    std::uint32_t error5 = 0;

    print_summary("[test_sorting]", "running");
    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto vec = gen_random_vec<std::uint32_t>(200, 0, 200);
        auto ans{vec};
        std::sort(ans.begin(), ans.end());

        auto vec0{vec};
        alg::select_sort(vec0.begin(), vec0.end());
        bool flag0 = compare(ans, vec0);
        if (!flag0) ++error0;

        auto vec1{vec};
        alg::bubble_sort(vec1.begin(), vec1.end());
        bool flag1 = compare(ans, vec1);
        if (!flag1) ++error1;

        auto vec2{vec};
        alg::insert_sort(vec2.begin(), vec2.end());
        bool flag2 = compare(ans, vec2);
        if (!flag2) ++error2;

        auto vec3{vec};
        alg::quick_sort(vec3.begin(), vec3.end());
        bool flag3 = compare(ans, vec3);
        if (!flag3) ++error3;

        auto vec4{vec};
        alg::merge_sort(vec4.begin(), vec4.end());
        bool flag4 = compare(ans, vec4);
        if (!flag4) ++error4;

        auto vec5{vec};
        alg::heap_sort(vec5.begin(), vec5.end());
        bool flag5 = compare(ans, vec5);
        if (!flag5) ++error5;

        if (t % (trial/20) == 0) std::cout << "." << std::flush;
    }

    print_summary("select_sort", error0, trial);
    print_summary("bubble_sort", error1, trial);
    print_summary("insert_sort", error2, trial);
    print_summary("quick_sort" , error3, trial);
    print_summary("merge_sort" , error4, trial);
    print_summary("heap_sort"  , error5, trial);
}


