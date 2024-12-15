#include<iostream>
#include<iomanip>
#include<cstdint>
#include<vector>
#include<algorithm>


inline std::string gen_random_str(std::uint32_t size, std::uint32_t alphabet_set) // 1-26
{
    std::uint32_t alphabet_set_size = 26;
    if (alphabet_set >= 1 && alphabet_set <= 26) 
    {
        alphabet_set_size = alphabet_set;
    }

    std::string ans;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        char c = 'a' + std::rand() % alphabet_set_size;
        ans.push_back(c);
    }
    return ans;
}

inline void gen_mirror_image(std::string& str, std::uint32_t centre, std::uint32_t radius)
{
    for(std::uint32_t r=0; r!=radius; ++r)
    {
        if (centre >= r && centre+r < str.size())
        {
            str[centre-r] = str[centre+r];
        }
        else break;
    }
}

inline std::string gen_random_palindrome(std::uint32_t size, std::uint32_t alphabet_set)
{
    std::uint32_t num_image = 5;
    std::uint32_t max_radius = size<3 ?  size : size/3;

    auto str = gen_random_str(size, alphabet_set);
    for(std::uint32_t n=0; n!=num_image; ++n)
    {
        std::uint32_t centre = rand() % str.size();
        std::uint32_t radius = rand() % max_radius + 1;
        gen_mirror_image(str, centre, radius);
    }
    return str;
}

inline std::vector<std::int32_t> gen_random_signed_vec(std::uint32_t size, std::int32_t min, std::int32_t max)
{
    std::vector<std::int32_t> ans;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        std::int32_t x = min + std::rand() % (max-min);
        ans.push_back(x);
    }
    return ans;
}

inline std::vector<std::uint32_t> gen_random_unsigned_vec(std::uint32_t size, std::uint32_t min, std::uint32_t max)
{
    std::vector<std::uint32_t> ans;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        std::uint32_t x = min + std::rand() % (max-min);
        ans.push_back(x);
    }
    return ans;
}

inline std::vector<std::int32_t> gen_random_signed_sorted_vec(std::uint32_t size, std::int32_t min, std::int32_t max)
{
    auto ans = gen_random_signed_vec(size, min, max);
    std::sort(ans.begin(), ans.end());
    return ans;
}

inline std::vector<std::uint32_t> gen_random_unsigned_sorted_vec(std::uint32_t size, std::uint32_t min, std::uint32_t max)
{
    auto ans = gen_random_unsigned_vec(size, min, max);
    std::sort(ans.begin(), ans.end());
    return ans;
}



// *************************** //
// *** Test loop functions *** //
// *************************** //
template<typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION>
void benchmark_str(const std::string&  test_name,
                   const GEN_FUNCTION& gen_function, 
                   const ALG_FUNCTION& alg_function, 
                   const BMK_FUNCTION& bmk_function, 
                   std::uint32_t       trial, 
                   std::uint32_t       size,
                   std::uint32_t       alphabet_set,
                   bool                print_each_test_case)
{
    std::uint32_t error = 0;
    if (print_each_test_case) std::cout << "\n";

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto str  = gen_function(size, alphabet_set);
        auto ans0 = alg_function(str);
        auto ans1 = bmk_function(str); 
        bool flag = (ans0 == ans1);
        if (!flag) ++error;

        if (print_each_test_case)
        {
            std::cout << "\n" << test_name
                      << " : ans0 = " << ans0 
                      <<  ", ans1 = " << ans1 
                      <<  ", error rate = " << error
                      <<  "/" << trial
                      <<  " " << (flag? "OK":"ERROR") << " " << str;
        }
    }
    if (!print_each_test_case)
    {
        std::cout << "\n" << std::setw(40) << test_name << ", error rate = " << error << "/" << trial;
    }
}

template<typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION, typename INPUT_TYPE>
void benchmark_vec(const std::string&  test_name,
                   const GEN_FUNCTION& gen_function, 
                   const ALG_FUNCTION& alg_function, 
                   const BMK_FUNCTION& bmk_function, 
                   std::uint32_t       trial, 
                   std::uint32_t       size,
                   INPUT_TYPE          min_value,
                   INPUT_TYPE          max_value, 
                   bool                print_each_test_case)
{
    std::uint32_t error = 0;
    if (print_each_test_case) std::cout << "\n";

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto vec  = gen_function(size, min_value, max_value);
        auto ans0 = alg_function(vec);
        auto ans1 = bmk_function(vec); 
        bool flag = (ans0 == ans1);
        if (!flag) ++error;

        if (print_each_test_case)
        {
            std::cout << "\n" << test_name
                      << " : ans0 = " << ans0 
                      <<  ", ans1 = " << ans1 
                      <<  ", error rate = " << error
                      <<  "/" << trial
                      <<  " " << (flag? "OK":"ERROR");
        }
    }
    if (!print_each_test_case)
    {
        std::cout << "\n" << std::setw(40) << test_name << ", error rate = " << error << "/" << trial;
    }
}

template<typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION, typename INPUT_TYPE, typename TARGET_TYPE>
void benchmark_vec_with_target(const std::string&  test_name,
                               const GEN_FUNCTION& gen_function, 
                               const ALG_FUNCTION& alg_function, 
                               const BMK_FUNCTION& bmk_function, 
                               std::uint32_t       trial, 
                               std::uint32_t       size,
                               INPUT_TYPE          min_value,
                               INPUT_TYPE          max_value,
                               TARGET_TYPE         target,
                               bool                print_each_test_case)
{
    std::uint32_t error = 0;
    if (print_each_test_case) std::cout << "\n";

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto vec  = gen_function(size, min_value, max_value);
        auto ans0 = alg_function(vec, target);
        auto ans1 = bmk_function(vec, target); 
        bool flag = (ans0 == ans1);
        if (!flag) ++error;

        if (print_each_test_case)
        {
            std::cout << "\n" << test_name
                      << " : ans0 = " << ans0 
                      <<  ", ans1 = " << ans1 
                      <<  ", error rate = " << error
                      <<  "/" << trial
                      <<  " " << (flag? "OK":"ERROR");
        }
    }
    if (!print_each_test_case)
    {
        std::cout << "\n" << std::setw(40) << test_name << ", error rate = " << error << "/" << trial;
    }
}

template<typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION, typename INPUT_TYPE, typename TARGET_TYPE>
void benchmark_2_vec_with_target(const std::string&  test_name,
                                 const GEN_FUNCTION& gen_function, 
                                 const ALG_FUNCTION& alg_function, 
                                 const BMK_FUNCTION& bmk_function, 
                                 std::uint32_t       trial, 
                                 std::uint32_t       size,
                                 INPUT_TYPE          min_value,
                                 INPUT_TYPE          max_value,
                                 TARGET_TYPE         target,
                                 bool                print_each_test_case)
{
    std::uint32_t error = 0;
    if (print_each_test_case) std::cout << "\n";

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto vec0 = gen_function(size, min_value, max_value);
        auto vec1 = gen_function(size, min_value, max_value);
        auto ans0 = alg_function(vec0, vec1, target);
        auto ans1 = bmk_function(vec0, vec1, target); 
        bool flag = (ans0 == ans1);
        if (!flag) ++error;

        if (print_each_test_case)
        {
            std::cout << "\n" << test_name
                      << " : ans0 = " << ans0 
                      <<  ", ans1 = " << ans1 
                      <<  ", error rate = " << error
                      <<  "/" << trial
                      <<  " " << (flag? "OK":"ERROR");
        }
    }
    if (!print_each_test_case)
    {
        std::cout << "\n" << std::setw(40) << test_name << ", error rate = " << error << "/" << trial;
    }
}

