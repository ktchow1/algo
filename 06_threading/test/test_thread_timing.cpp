#include<iostream>
#include<future>
#include<unordered_set>
#include<thread.h>
#include<timer.h>
#include<statistics.h> 


void timer_resolution()
{
    alg::statistics<std::uint64_t> stat;
    alg::timer timer;

    for(std::uint32_t n=0; n!=1000; ++n)
    {
        timer.click(); 
        timer.click(); 
        stat.add(timer.time_elapsed_in_nsec());
    }
    std::cout << "\n[Timer resolution]" << stat.get_string();
    std::cout << "\n";
}

void time_thread_creation()
{
    alg::statistics<std::uint64_t> stat;
    alg::timer timer;

    for(std::uint32_t n=0; n!=1000; ++n)
    {
        timer.click();
        std::thread t([&]()
        {
            timer.click();
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        });

        t.join();
        stat.add(timer.time_elapsed_in_nsec());
    }
    std::cout << "\n[Thread creation time]" << stat.get_string();
    std::cout << "\n";
}

void time_thread_creation_by_async_call()
{
    alg::statistics<std::uint64_t> stat;
    alg::timer timer;

    for(std::uint32_t n=0; n!=1000; ++n)
    {
        timer.click();
        std::future<std::uint64_t> future = std::async([&timer]()
        {
            timer.click();
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            return timer.time_elapsed_in_nsec();
        });

        stat.add(future.get()); // thread join inside
    }
    std::cout << "\n[Threead creation by async call time]" << stat.get_string();
    std::cout << "\n";
}

void time_mutex_lock_and_unlock()
{
    std::mutex mutex;
    alg::statistics<std::uint64_t> stat0;
    alg::statistics<std::uint64_t> stat1;
    alg::timer timer;

    for(std::uint32_t n=0; n!=1000; ++n)
    {
        timer.click();
        mutex.lock();
        timer.click();
        stat0.add(timer.time_elapsed_in_nsec());

        timer.click();
        mutex.unlock();
        timer.click();
        stat1.add(timer.time_elapsed_in_nsec());
    }
    std::cout << "\n[Mutex lock time]" << stat0.get_string() << "\n";
    std::cout << "\n[Mutex unlock time]" << stat1.get_string() << "\n";
}

void time_thread_affinity()
{
    alg::statistics<std::uint64_t> stat;
    std::unordered_set<std::uint32_t> core_pre_affinity;
    std::unordered_set<std::uint32_t> core_post_affinity;
    alg::timer timer;

    for(std::uint32_t n=0; n!=5000; ++n)
    {
        std::thread t([&]()
        {
            for(std::uint32_t n=0; n!=20; ++n)
            {
                std::this_thread::yield();
                core_pre_affinity.insert(sched_getcpu());
            }

            timer.click();
            alg::set_this_thread_affinity(2);
            timer.click();
            stat.add(timer.time_elapsed_in_nsec());

            for(std::uint32_t n=0; n!=20; ++n)
            {
                std::this_thread::yield();
                core_post_affinity.insert(sched_getcpu());
            }
        });

        t.join();
    }
    std::cout << "\n[Thread affinity time]" << stat.get_string();
    std::cout << "\ncores pre  affinity : "; for(const auto&x : core_pre_affinity)  std::cout << x << " ";
    std::cout << "\ncores post affinity : "; for(const auto&x : core_post_affinity) std::cout << x << " ";
    std::cout << "\n";
}

void time_thread_priority()
{
    alg::statistics<std::uint64_t> stat;
    alg::timer timer;

    for(std::uint32_t n=0; n!=1000; ++n)
    {
        std::thread t([&]()
        {
            timer.click();
            alg::set_this_thread_policy(SCHED_RR);
            timer.click();
            stat.add(timer.time_elapsed_in_nsec());
        });

        t.join();
    }
    std::cout << "\n[Thread priority time]" << stat.get_string();
    std::cout << "\n";
}

void test_thread_timing()
{
    timer_resolution();
    time_thread_creation();
    time_thread_creation_by_async_call();
    time_mutex_lock_and_unlock();
    time_thread_affinity();
    time_thread_priority();
}


