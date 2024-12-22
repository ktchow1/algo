#include<iostream>
#include<iomanip>
#include<cassert>
#include<cmath>
#include<utility.h>
#include<bisection.h>

constexpr double pi      = 3.141592654;
constexpr double epsilon = 0.000000001;


bool equal(double answer, double truth)
{
    return answer > truth - epsilon &&
           answer < truth + epsilon;
}

bool equal(std::uint32_t answer, std::uint32_t truth)
{
    return answer == truth;
}

template<typename T>
bool compare_and_print(const std::optional<T>& answer, 
                       const std::optional<T>& truth)
{
    if (std::is_same_v<T,double>)
    {
        std::cout << std::fixed << std::setprecision(6);
    }
    if (truth)
    {
        if (answer) 
        {
            if (equal(*answer, *truth))
            {
                std::cout << "\n[OK] answer = " << *answer << ", truth = " << *truth << std::flush;
                return true;
            }
            else
            {
                std::cout << "\n[ERROR] answer = " << *answer << ", truth = " << *truth << std::flush;
                return false;
            }
        }
        else
        {
            std::cout << "\n[ERROR] answer = nullopt" << ", truth = " << *truth << std::flush;
            return false;
        }
    }
    else
    {
        if (answer) 
        {
            std::cout << "\n[ERROR] answer = " << *answer << ", truth = nullopt" << std::flush;
            return false;
        }
        else
        {
            std::cout << "\n[OK] answer = nullopt, truth = nullopt" << std::flush;
            return true;
        }
    }
}

void test_bisection()
{
    std::cout << "\n\nBisection of double function";
    {
        auto ans = alg::bisection([](double x) { return sin(x); }, -pi/2, +pi/2);        
        assert(compare_and_print(ans, std::optional<double>{0.0}));
    }
    {
        auto ans = alg::bisection([](double x) { return sin(x); }, +pi/2, 3*pi/2);
        assert(compare_and_print(ans, std::optional<double>{+pi}));
    }   
    {
        auto ans = alg::bisection([](double x) { return sin(x)-0.5; }, -pi/2, +pi/2);
        assert(compare_and_print(ans, std::optional<double>{+pi/6}));
    }
    {
        auto ans = alg::bisection([](double x) { return sin(x)-sqrt(3)/2; }, -pi/2, +pi/2);
        assert(compare_and_print(ans, std::optional<double>{+pi/3}));
    }
    {
        auto ans = alg::bisection([](double x) { return sin(x)-cos(x); }, -pi/2, +pi/2);
        assert(compare_and_print(ans, std::optional<double>{+pi/4}));
    }
    {
        auto ans = alg::bisection([](double x) { return sin(x)-cos(x); }, 2*pi/3, +pi/2);
        assert(compare_and_print(ans, std::optional<double>{}));
    }

    std::cout << "\n\nBisection of int vector";
    {
        std::vector<std::int32_t> vec{10}; // size = 1
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{0}));
    }
    {
        std::vector<std::int32_t> vec{1};
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{}));
    }
    {
        std::vector<std::int32_t> vec{10,29}; // size = 2
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{0}));
    }
    {
        std::vector<std::int32_t> vec{1,10};
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{1}));
    }
    {
        std::vector<std::int32_t> vec{1,20};
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{}));
    }
    {
        std::vector<std::int32_t> vec{10,20,30}; // size = 3
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{0}));
    }
    {
        std::vector<std::int32_t> vec{1,10,20};
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{1}));
    }
    {
        std::vector<std::int32_t> vec{1,2,10};
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{2}));
    }
    {
        std::vector<std::int32_t> vec{1,5,20};
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{}));
    }
    {
        std::vector<std::int32_t> vec{10,11,12,15,18,20,24,28,30,32,36,40,45}; // size > 10
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{0}));
    }
    {
        std::vector<std::int32_t> vec{-2,-1,0,1,2,4,5,8,10,11,12,15,18,20};
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{8}));
    }
    {
        std::vector<std::int32_t> vec{-5,-4,-3,-2,-1,0,1,2,3,4,5,8,10};
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{12}));
    }
    {
        std::vector<std::int32_t> vec{-5,-4,-3,-2,-1,0,1,2,3,4,5,8,20,24,28,30,32};
        auto ans = alg::bisection(vec, 10);
        assert(compare_and_print(ans, std::optional<std::uint32_t>{}));
    }

    std::cout << "\n\nPeak bisection";

}

