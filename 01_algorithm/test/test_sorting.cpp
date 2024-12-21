#include<iostream>
#include<iomanip>
#include<cassert>
#include<cmath>
#include<utility.h>
#include<bisection.h>

constexpr double pi      = 3.141592654;
constexpr double epsilon = 0.000000001;


inline bool equal(std::optional<double> answer, double truth)
{
    if (!answer) return false;
    if (*answer > truth - epsilon &&
        *answer < truth + epsilon)
    {
        std::cout << "\n[bisection OK] answer = " << std::fixed << std::setprecision(6) << *answer << ", truth = " << truth << std::flush;
        return true;
    }
    else
    {
        std::cout << "\n[bisection ERROR] answer = " << std::fixed << std::setprecision(6) << *answer << ", truth = " << truth << std::flush;
        return false;
    }
}

void test_bisection()
{
    {
        auto ans = alg::bisection([](double x) { return sin(x); }, -pi/2, +pi/2);        
        assert(equal(ans, 0));
    }
    {
        auto ans = alg::bisection([](double x) { return sin(x); }, +pi/2, 3*pi/2);
        assert(equal(ans, +pi));
    }   
    {
        auto ans = alg::bisection([](double x) { return sin(x)-0.5; }, -pi/2, +pi/2);
        assert(equal(ans, +pi/6));
    }
    {
        auto ans = alg::bisection([](double x) { return sin(x)-sqrt(3)/2; }, -pi/2, +pi/2);
        assert(equal(ans, +pi/3));
    }
    {
        auto ans = alg::bisection([](double x) { return sin(x)-cos(x); }, -pi/2, +pi/2);
        assert(equal(ans, +pi/4));
    }
}
