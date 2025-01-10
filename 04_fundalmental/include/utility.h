#include<iostream>
#include<iomanip>
#include<cstdint>
#include<vector>
#include<algorithm>
#include<functional>
#include<timer.h>

using namespace std::placeholders;


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
std::vector<T> gen_random_vec(std::uint32_t size, T min, T max)
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
std::vector<T> gen_random_sorted_vec(std::uint32_t size, T min, T max)
{
    auto ans = gen_random_vec(size, min, max);
    std::sort(ans.begin(), ans.end());
    return ans;
}

template<typename T>
std::vector<T> gen_random_partial_sorted_vec(std::uint32_t size, T min, T max)
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

// Assume size-1 <= max-min+1
inline std::vector<std::uint32_t> gen_random_coins(std::uint32_t size, std::uint32_t min, std::uint32_t max)
{
    std::vector<std::uint32_t> ans;
    ans.push_back(1);

    for(std::uint32_t n=min; n!=max; ++n)
    {
        ans.push_back(n);
    }
    for(std::uint32_t n=1; n!=size; ++n)
    {
        std::uint32_t m = 1+rand()%(ans.size()-1);
        std::swap(ans[n], ans[m]);
    }

    ans.resize(size);
    std::sort(ans.begin(), ans.end());
    return ans;
}


// *************** //
// *** Compare *** //
// *************** //
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


// **************** //
// *** Printing *** //
// **************** //
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

template<typename OUTPUT>
void print_one_case(const std::string& test_name, 
                    OUTPUT ans0, 
                    OUTPUT ans1, 
                    std::uint32_t error,
                    std::uint32_t trial, 
                    bool flag, 
                    const std::string& str = "")
{
    std::cout << "\n" << test_name
              << " : ans0 = " << ans0 
              <<  ", ans1 = " << ans1 
              <<  ", error rate = " << error
              <<  "/" << trial
              <<  " " << (flag? "OK":"ERROR") << str << std::flush;
}

inline void print_summary(const std::string& test_name, const std::string& status)
{
    std::cout << "\n" << std::setw(60) << std::left << test_name 
                      << " " << status
                      << std::flush;
}

inline void print_summary(const std::string& test_name, std::uint32_t error, std::uint32_t trial)
{
    std::cout << "\n" << std::setw(60) << std::left << test_name 
                      << " error rate = " << error << "/" << trial
                      << std::flush;
}

inline void print_summary(const std::string& test_name, std::uint32_t error, std::uint32_t trial, std::uint64_t time0, std::uint64_t time1)
{
    std::cout << "\n" << std::setw(60) << std::left << test_name 
                      << " error rate = " << error << "/" << trial 
                      << ", time = " << time0 << "/" << time1 
                      << std::flush;
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


// *************************** //
// *** Test loop functions *** //
// *************************** //
template<std::uint32_t GEN_NUM, typename GEN_FUNCTION, typename ALG_FUNCTION, typename BMK_FUNCTION>
void benchmark(const std::string&  test_name,
               const GEN_FUNCTION& gen_function, // nullary
               const ALG_FUNCTION& alg_function, // nary = GEN_NUM
               const BMK_FUNCTION& bmk_function, // nary = GEN_NUM
               std::uint32_t       trial, 
               bool                print_each_test_case)
{
    alg::timer timer0;
    alg::timer timer1;
    statistics<std::uint64_t> stat0(0);
    statistics<std::uint64_t> stat1(0);
    std::uint32_t error = 0;

    if (print_each_test_case) std::cout << "\n";
    for(std::uint32_t t=0; t!=trial; ++t)
    {
        if constexpr (GEN_NUM == 1)
        {
            auto data = gen_function();

            timer0.click();
            auto ans0 = alg_function(data);
            timer0.click();

            timer1.click();
            auto ans1 = bmk_function(data); 
            timer1.click();

            if (ans0 != ans1) ++error;
            if (print_each_test_case) print_one_case(test_name, ans0, ans1, error, trial, ans0 == ans1);
        }
        else
        {
            auto data0 = gen_function();
            auto data1 = gen_function();

            timer0.click();
            auto ans0 = alg_function(data0, data1);
            timer0.click();

            timer1.click();
            auto ans1 = bmk_function(data0, data1); 
            timer1.click();

            if (ans0 != ans1) ++error;
            if (print_each_test_case) print_one_case(test_name, ans0, ans1, error, trial, ans0 == ans1);
        }

        stat0.add(timer0.time_elapsed_in_nsec());
        stat1.add(timer1.time_elapsed_in_nsec());
    }
    print_summary(test_name, error, trial, stat0.mean(), stat1.mean());
}
