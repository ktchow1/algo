#pragma once
#include<iostream>
#include<cassert>
#include<functional>

// *** alg *** //
#include<threadpool.h>
#include<statistics.h>
#include<utility.h>


struct thdpool_output
{
    std::thread::id tid;
    std::uint64_t ns_response;
    std::uint64_t ns_calculate0;
    std::uint64_t ns_calculate1;
    double exp_x0;
    double exp_x1;
};

struct thdpool_task
{
    inline void operator()() 
    {
        timespec ts_response;
        clock_gettime(CLOCK_MONOTONIC, &ts_response);
        res->tid = std::this_thread::get_id();
        res->ns_response = alg::to_nanosec(ts_response) - alg::to_nanosec(ts_emplace);

        const std::uint32_t N = 1000;
        double s = (1+x/N);

        timespec ts0;
        timespec ts1;
        timespec ts2;
        clock_gettime(CLOCK_MONOTONIC, &ts0);

        res->exp_x0 = 1;
        for(std::uint32_t n=0; n!=N; ++n) 
        {
            res->exp_x0 *= s;    
        }

        clock_gettime(CLOCK_MONOTONIC, &ts1);

        res->exp_x1 = exp(x);

        clock_gettime(CLOCK_MONOTONIC, &ts2);
        res->ns_calculate0 = alg::to_nanosec(ts1) - alg::to_nanosec(ts0);
        res->ns_calculate1 = alg::to_nanosec(ts2) - alg::to_nanosec(ts1);
    }

    timespec ts_emplace;
    double x;
    thdpool_output* res;
};


// *************************** //
// *** Test for threadpool *** //
// *************************** //
namespace alg
{
    template<template<typename> typename QUEUE>
    void run_threadpool(const std::string& test_name, std::uint32_t num_threads, std::uint32_t num_tasks)
    {
        std::uint32_t waiting_in_us = 10;
        std::vector<thdpool_output> results;
        results.resize(num_tasks);

        {
            alg::threadpool<thdpool_task, QUEUE> pool(num_threads);
            for(std::uint32_t n=0; n!=num_tasks; ++n)
            {
                double r = rand() % 1000 / 200.0;
                timespec time;
                clock_gettime(CLOCK_MONOTONIC, &time);

                while(!pool.emplace_task(time, r, &results[n])) 
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(waiting_in_us));
                }

                if (waiting_in_us > 0)
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(waiting_in_us));
                }
            }
            pool.stop();
        }

        alg::statistics<std::uint64_t> stat; 
        for(std::uint32_t n=0; n!=num_tasks; ++n)
        {
            stat.add(results[n].ns_response);
        }



        // **************** //
        // *** Checking *** //
        // **************** //
        print_summary(test_name, "succeeded, time = " + stat.get_str());
    }
}

