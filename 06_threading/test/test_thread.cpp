
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
    std::cout << "\nTimer resolution : " << stat.get_string();
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
    std::cout << "\nThread creation time : " << stat.get_string();
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
    std::cout << "\nThreead creation by async call time : " << stat.get_string();
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
    std::cout << "\nMutex lock time : " << stat0.get_string() << "\n";
    std::cout << "\nMutex unlock time : " << stat1.get_string() << "\n";
}

auto synchronization_with_atomic(std::uint32_t cpu0, std::uint32_t cpu1)
{
    // *** Sync mechanism *** //
    std::atomic<std::uint32_t> trigger = 0;
    std::atomic<std::uint32_t> response = 0;
    constexpr std::uint32_t N = 1000;

    // *** Measurement *** //
    timespec ts0;
    timespec ts1;
    timespec tsM;
    alg::statistics<std::uint64_t> stat01;
    alg::statistics<std::uint64_t> stat0M;
    alg::statistics<std::uint64_t> statM1;

    // *************** //
    // *** Reactor *** //
    // *************** //
    std::thread reactor([&]()
    {
        alg::set_this_thread_affinity(cpu0);        
        alg::set_this_thread_policy(SCHED_FIFO);        
        for(std::uint32_t n=0; n!=N; ++n)
        {
            while(trigger.load()!=n+1) 
            {
                __builtin_ia32_pause();
            //  std::this_thread::yield(); // very slow response time
            }
            clock_gettime(CLOCK_MONOTONIC, &tsM);
            response.fetch_add(1);
        }
    });

    // ***************** //
    // *** Initiator *** //
    // ***************** //
    alg::set_this_thread_affinity(cpu1);        
    alg::set_this_thread_policy(SCHED_FIFO);        
    for(std::uint32_t n=0; n!=N; ++n)
    {
        clock_gettime(CLOCK_MONOTONIC, &ts0);
        trigger.fetch_add(1);
        while(response.load()!=n+1) 
        {
            __builtin_ia32_pause();
        //  std::this_thread::yield(); // very slow response time 
        }
        clock_gettime(CLOCK_MONOTONIC, &ts1);


        // *** Print measurement *** //
        std::uint64_t ns0 = to_nanosec(ts0);
        std::uint64_t ns1 = to_nanosec(ts1);
        std::uint64_t nsM = to_nanosec(tsM);
        stat01.add(ns1 - ns0);
        stat0M.add(nsM - ns0);
        statM1.add(ns1 - nsM);

        // *************************************************************************** //
        // There is no way to block initiator before each round, thus we have to wait. //
        // *************************************************************************** //
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    reactor.join();
    return std::make_tuple(stat01, stat0M, statM1);
}

void time_synchronization_with_atomic()
{
    alg::statistics<std::uint64_t> stat01;
    alg::statistics<std::uint64_t> stat0M;
    alg::statistics<std::uint64_t> statM1;

    for(std::uint32_t n=0; n!=7; ++n)
    {
        for(std::uint32_t m=n+1; m!=8; ++m)
        {
            auto [a,b,c] = synchronization_with_atomic(n,m);     
            auto [d,e,f] = synchronization_with_atomic(m,n);     
            stat01 += a += d; 
            stat0M += b += e; 
            statM1 += c += f; 
        }
    }
    std::cout << "\nSynchonization with atomic : all-in-one";
    std::cout << "\n[stat01]" << stat01.get_string() << "\n";
    std::cout << "\n[stat0M]" << stat0M.get_string() << "\n";
    std::cout << "\n[statM1]" << statM1.get_string() << "\n";
}


void test_thread()
{
    test_timer_resolution();
    time_thread_creation();
    time_thread_creation_by_async_call();
    time_mutex_lock_and_unlock();
}


