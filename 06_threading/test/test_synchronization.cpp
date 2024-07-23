#include<iostream>
#include<atomic>
#include<thread.h>
#include<synchronization.h>
#include<timer.h>
#include<statistics.h>


template<typename SYNC>
void sync_test(const std::string& test_name, std::uint32_t N, std::uint32_t us)
{
    // *** Sync mechanism *** //
    SYNC sync;
    std::atomic<std::uint32_t> ready = 0;

    // *** Measurement *** //
    alg::statistics<std::uint64_t> stat{};
    alg::timer timer;

    std::thread consumer
    (
        [&]()
        {
            alg::set_this_thread_affinity(2);
            alg::set_this_thread_priority();
            for(std::uint32_t n=0; n!=N; ++n)
            {
                ready.fetch_add(1); // <--- sync point
                sync.wait();        // <--- consumer is blocked

                // stop timer
                timer.click();
                stat.add(timer.time_elapsed_in_nsec());
            }
        }
    );

    // *** Producer *** //
    {
        alg::set_this_thread_affinity(4);
        alg::set_this_thread_priority();
        for(std::uint32_t n=0; n!=N; ++n)
        {
            while(ready.load()!=n+1) 
            {
                std::this_thread::yield(); // <--- sync point (yield allows consumer to fetch-add)
            }
            std::this_thread::sleep_for(std::chrono::microseconds(us)); // <--- wait until consumer is blocked

            // start timer
            timer.click();
            sync.notify();
        }
    }

    consumer.join();
    std::cout << "\n[" << test_name << "] N=" << N << " us=" << us;
    std::cout << stat.get_string();
    std::cout << "\n";
}

void test_synchronization()
{
    constexpr std::uint32_t N  = 1000; // must be compile time const for sync_promfut
    constexpr std::uint32_t us = 1000;

    sync_test<alg::sync_futex>     ("Synchronization with sync_futex",      N, us);
    sync_test<alg::sync_mutex>     ("Synchronization with sync_mutex",      N, us);
    sync_test<alg::sync_pmutex>    ("Synchronization with sync_pmutex",     N, us);
    sync_test<alg::sync_HansBarz>  ("Synchronization with sync_HansBarz",   N, us);
    sync_test<alg::sync_semaphore> ("Synchronization with sync_semaphore",  N, us);
    sync_test<alg::sync_psemaphore>("Synchronization with sync_psemaphore", N, us);
    sync_test<alg::sync_condvar>   ("Synchronization with sync_condvar",    N, us);
    sync_test<alg::sync_promfut<N>>("Synchronization with sync_promfut",    N, us);
    sync_test<alg::sync_atomic>    ("Synchronization with sync_atomic",     N, us);
}
