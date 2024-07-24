#include<iostream>
#include<cassert>
#include<thread>
#include<future>
#include<promise_future.h>


template<template<typename> typename PROMISE>
void test_promise_future_impl(const std::string& test_name)
{
    std::cout << "\n" << test_name;
    for(std::uint32_t n=0; n!=20; ++n)
    {
        PROMISE<std::uint32_t> p;
        auto f = p.get_future();

        std::thread t0([&]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            p.set_value(n+123);
        });

        std::thread t1([&]()
        {
            assert(f.get() == n+123);
        });

        t0.join(); 
        t1.join(); 
        std::cout << "\nprogress = " << n << "/20";
    }
    std::cout << "\n";
}


void test_promise_future()
{
    test_promise_future_impl<std::promise>("[Promise-future std version]");
    test_promise_future_impl<alg::promise>("[Promise-future alg version]");
}
