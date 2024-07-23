#include<iostream>
#include<future>
#include<thread.h>
#include<timer.h>
#include<statistics.h> 


void test_timer_resolution()
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

void test_thread()
{
    test_timer_resolution();
    time_thread_creation();
    time_thread_creation_by_async_call();
    time_mutex_lock_and_unlock();
}


