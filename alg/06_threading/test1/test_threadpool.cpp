

#include<iostream>
#include<functional>
#include<threadpool.h>
#include<statistics.h>
#include<timer.h>

struct result
{
    std::thread::id tid;
    std::uint64_t ns_response;
    std::uint64_t ns_calculate0;
    std::uint64_t ns_calculate1;
    double exp_x0;
    double exp_x1;
};

std::ostream& operator<<(std::ostream& os, const result& res)
{
    os << "tid = " << res.tid << ", ";
    os << "ns_response = " << res.ns_response   << ", ";
    os << "ns_calculate0 = " << res.ns_calculate0 << ", ";
    os << "ns_calculate1 = " << res.ns_calculate1 << ", ";
    os << "exp_x0 = " << std::setprecision(8) << res.exp_x0 << ", ";
    os << "exp_x1 = " << std::setprecision(8) << res.exp_x1;
    return os;
}

struct task
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
    result* res;
};


template<template<typename> typename QUEUE>
void test_threadpool_impl(const std::string& label, std::uint32_t waiting_in_us)
{
    std::uint32_t num_threads = 4;
    std::uint32_t num_trials = 100;
    std::vector<result> results;
    results.resize(num_trials);

    {
        alg::threadpool<task, QUEUE> pool(num_threads);
        for(std::uint32_t n=0; n!=num_trials; ++n)
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
    for(std::uint32_t n=0; n!=num_trials; ++n)
    {
        stat.add(results[n].ns_response);
    }
    std::cout << "\n[" << label << "] " << stat.get_string() << std::flush;
    std::cout << "\n";
}

void test_threadpool()
{
    test_threadpool_impl<alg::lockfree_queue_long> ("lockfree_queue_long", 10);
    test_threadpool_impl<alg::lockfree_queue_short>("lockfree_queue_short",10);
    test_threadpool_impl<alg::mutex_locked_queue>  ("mutex_locked_queue",  10);
    test_threadpool_impl<alg::spin_locked_queue>   ("spin_locked_queue",   10);
    test_threadpool_impl<alg::lockfree_queue_long> ("lockfree_queue_long", 0);
    test_threadpool_impl<alg::lockfree_queue_short>("lockfree_queue_short",0);
    test_threadpool_impl<alg::mutex_locked_queue>  ("mutex_locked_queue",  0);
    test_threadpool_impl<alg::spin_locked_queue>   ("spin_locked_queue",   0);
}
