#include<iostream>
#include<iomanip>
#include<cstdint>
#include<vector>
#include<algorithm>
#include<timer.h>


// ************************* //
// *** Random generators *** //
// ************************* //
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

template<typename T>
inline std::vector<T> gen_random_vec(std::uint32_t size, T min, T max)
{
    std::vector<T> ans;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        T x = min + std::rand() % (max-min);
        ans.push_back(x);
    }
    return ans;
}

template<typename T>
inline std::vector<T> gen_random_sorted_vec(std::uint32_t size, T min, T max)
{
    auto ans = gen_random_vec(size, min, max);
    std::sort(ans.begin(), ans.end());
    return ans;
}

template<typename T>
inline std::vector<T> gen_random_partial_sorted_vec(std::uint32_t size, T min, T max)
{
    auto ans = gen_random_sorted_vec(size, min, max);

    std::uint32_t N0 =        rand() % (size/2);
    std::uint32_t N1 = size - rand() % (size/2);
    for(std::uint32_t t=0; t!=N1-N0; ++t)
    {
        std::uint32_t n0 = rand() & (N1-N0) + N0; 
        std::uint32_t n1 = rand() & (N1-N0) + N0; 
        std::swap(ans[n0], ans[n1]);
    }
    return ans;
}

// Generate swapped vector [0,1,2,3,...,N-1]
inline std::vector<std::uint32_t> gen_random_swapped_order(std::uint32_t size)
{
    std::vector<std::uint32_t> ans;
    for(std::uint32_t n=0; n!=size; ++n)
    {
        ans.push_back(n);
    }

    std::uint32_t N = rand()%size;
    for(std::uint32_t n=0; n!=N; ++n)
    {
        std::uint32_t x = rand()%3;
        if (x==1 && n>=1)
        {
            std::swap(ans[n], ans[n-1]);
        }
        else if (x==2 && n>=2)
        {
            std::swap(ans[n], ans[n-1]);
            std::swap(ans[n-1], ans[n-2]);
        }
    }
    return ans;
}


// ****************** //
// *** Statistics *** //
// ****************** //
// Todo : 
// * add namespace
// * add percentile
// * move to c++ folder
//
template<typename T>
class statistics
{
public:
    explicit statistics(const T& x) : m_sum(x), m_count(0)
    {
    }

    void add(const T& x)
    {
        m_sum += x;
        ++m_count;
    }
    
    T mean() const noexcept
    {
        if (m_count > 0) 
             return m_sum / m_count;
        else return m_sum;             
    }

private:
    T m_sum;
    std::uint32_t m_count;
};


// **************** //
// *** Printing *** //
// **************** //
template<typename OUTPUT>
void print_one_case(const std::string& test_name, 
                    OUTPUT ans0, 
                    OUTPUT ans1, 
                    std::uint32_t error,
                    std::uint32_t trial, 
                    bool flag, 
                    const std::string& str)
{
    std::cout << "\n" << test_name
              << " : ans0 = " << ans0 
              <<  ", ans1 = " << ans1 
              <<  ", error rate = " << error
              <<  "/" << trial
              <<  " " << (flag? "OK":"ERROR") << str;
}

template<typename OUTPUT>
void print_one_case(const std::string& test_name, 
                    OUTPUT ans0, 
                    OUTPUT ans1, 
                    std::uint32_t error,
                    std::uint32_t trial, 
                    bool flag) 
{
    std::cout << "\n" << test_name
              << " : ans0 = " << ans0 
              <<  ", ans1 = " << ans1 
              <<  ", error rate = " << error
              <<  "/" << trial
              <<  " " << (flag? "OK":"ERROR");
}

inline void print_summary(const std::string& test_name, const std::string& status)
{
    std::cout << "\n" << std::setw(60) << std::left << test_name << " " << status;
}

inline void print_summary(const std::string& test_name, std::uint32_t error, std::uint32_t trial)
{
    std::cout << "\n" << std::setw(60) << std::left << test_name 
                      << " error rate = " << error << "/" << trial;
}

inline void print_summary(const std::string& test_name, std::uint32_t error, std::uint32_t trial, std::uint64_t time0, std::uint64_t time1)
{
    std::cout << "\n" << std::setw(60) << std::left << test_name 
                      << " error rate = " << error << "/" << trial 
                      << ", time0 = " << time0
                      << ", time1 = " << time1;
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
        auto str = gen_function(size, alphabet_set);       
        auto ans0 = alg_function(str);
        auto ans1 = bmk_function(str);
        bool flag = (ans0 == ans1);

        if (!flag) ++error;
        if (print_each_test_case) print_one_case(test_name, ans0, ans1, error, trial, flag, str);
    }
    print_summary(test_name, error, trial);
}

template<typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION, typename GEN_INPUT>
void benchmark_vec(const std::string&  test_name,
                   const GEN_FUNCTION& gen_function, 
                   const ALG_FUNCTION& alg_function, 
                   const BMK_FUNCTION& bmk_function, 
                   std::uint32_t       trial, 
                   std::uint32_t       size,
                   GEN_INPUT           min_value,
                   GEN_INPUT           max_value, 
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
        if (print_each_test_case) print_one_case(test_name, ans0, ans1, error, trial, flag);
    }
    print_summary(test_name, error, trial);
}

template<typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION>
void benchmark_vec_without_input(const std::string&  test_name,
                                 const GEN_FUNCTION& gen_function, 
                                 const ALG_FUNCTION& alg_function, 
                                 const BMK_FUNCTION& bmk_function, 
                                 std::uint32_t       trial, 
                                 std::uint32_t       size,
                                 bool                print_each_test_case)
{
    std::uint32_t error = 0;
    if (print_each_test_case) std::cout << "\n";

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto vec  = gen_function(size);
        auto ans0 = alg_function(vec);
        auto ans1 = bmk_function(vec); 
        bool flag = (ans0 == ans1);

        if (!flag) ++error;
        if (print_each_test_case) print_one_case(test_name, ans0, ans1, error, trial, flag);
    }
    print_summary(test_name, error, trial);
}

template<typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION, typename GEN_INPUT, typename ALG_INPUT>
void benchmark_vec_with_alg_input(const std::string&  test_name,
                                  const GEN_FUNCTION& gen_function, 
                                  const ALG_FUNCTION& alg_function, 
                                  const BMK_FUNCTION& bmk_function, 
                                  std::uint32_t       trial, 
                                  std::uint32_t       size,
                                  GEN_INPUT           min_value,
                                  GEN_INPUT           max_value,
                                  ALG_INPUT           arg,
                                  bool                print_each_test_case)
{
    std::uint32_t error = 0;
    if (print_each_test_case) std::cout << "\n";

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto vec  = gen_function(size, min_value, max_value);
        auto ans0 = alg_function(vec, arg);
        auto ans1 = bmk_function(vec, arg); 
        bool flag = (ans0 == ans1);

        if (!flag) ++error;
        if (print_each_test_case) print_one_case(test_name, ans0, ans1, error, trial, flag);
    }
    print_summary(test_name, error, trial);
}

template<typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION, typename GEN_INPUT, typename ALG_INPUT>
void benchmark_2_vec_with_alg_input(const std::string&  test_name,
                                    const GEN_FUNCTION& gen_function, 
                                    const ALG_FUNCTION& alg_function, 
                                    const BMK_FUNCTION& bmk_function, 
                                    std::uint32_t       trial, 
                                    std::uint32_t       size,
                                    GEN_INPUT           min_value,
                                    GEN_INPUT           max_value,
                                    ALG_INPUT           arg,
                                    bool                print_each_test_case)
{
    std::uint32_t error = 0;
    if (print_each_test_case) std::cout << "\n";

    for(std::uint32_t t=0; t!=trial; ++t)
    {
        auto vec0 = gen_function(size, min_value, max_value);
        auto vec1 = gen_function(size, min_value, max_value);
        auto ans0 = alg_function(vec0, vec1, arg);
        auto ans1 = bmk_function(vec0, vec1, arg); 
        bool flag = (ans0 == ans1);

        if (!flag) ++error;
        if (print_each_test_case) print_one_case(test_name, ans0, ans1, error, trial, flag);
    }
    print_summary(test_name, error, trial);
}

