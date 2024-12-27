#include<iostream>
#include<thread.h>
#include<synchronization.h>
#include<timer.h>


// *********************************************************** //
// Main differences between this test and sync_test are :
// * this test is for semaphore
// * this test is spmc
// * supports customized notification schedule
// * objective is to check how semaphore work, not for timing
// *********************************************************** //
void test_semaphore()
{
    std::cout << "[Test how semaphore works]";

    // *** Sync mechanism *** //
    std::mutex mutex_cout;  
    alg::sync_psemaphore s;
    std::atomic<std::uint32_t> ready = 0;


    std::vector<std::thread> threads;
    for(std::uint32_t m=0; m!=3; ++m)
    {
        threads.push_back(std::thread([&](std::uint32_t thread_id)
        {
            // *** Measurement *** //
            alg::timer timer;

            for(std::uint32_t n=0; n!=10; ++n)
            {
                // *** Print *** //
                {
                    std::lock_guard<std::mutex> lock(mutex_cout);
                    std::cout << "\nconsumer " << thread_id << " waiting" << std::flush;
                }

                ready.fetch_add(1); // *** sync point 
                timer.click();
                s.wait();           // *** consumer is blocked
                timer.click();

                // *** Print *** //
                {
                    std::lock_guard<std::mutex> lock(mutex_cout);
                    std::cout << "\nconsumer " << thread_id 
                              << " notified by product " << n 
                              << ", semaphore_peek = " << s.peek_value() 
                              << ", time_in_waiting = " << timer.time_elapsed_in_nsec() << std::flush;
                }

                // *** Do some work *** //
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }

            // *** Print *** //
            {
                std::lock_guard<std::mutex> lock(mutex_cout);
                std::cout << "\nconsumer " << thread_id << " done";
            }
        }, m));
    }

    // *** Producer (any pproduction schedule) *** //
    while(ready.load()!=3) std::this_thread::yield();

    for(std::uint32_t n=0; n!=3; ++n) s.notify();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for(std::uint32_t n=0; n!=2; ++n) s.notify();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for(std::uint32_t n=0; n!=2; ++n) s.notify();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for(std::uint32_t n=0; n!=2; ++n) s.notify();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for(std::uint32_t n=0; n!=2; ++n) s.notify();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for(std::uint32_t n=0; n!=2; ++n) s.notify();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for(std::uint32_t n=0; n!=4; ++n) s.notify();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    for(std::uint32_t n=0; n!=10; ++n) s.notify();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    s.notify();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    s.notify();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    s.notify();


    // *** Test maximum number of notifications *** //
    for(std::uint32_t n=0; n!=100000; ++n) s.notify();
    std::cout << "\nsemaphore_peek after 100000 notify-call = " << s.peek_value() << std::flush;
    for(std::uint32_t n=0; n!=100000; ++n) s.wait();
    std::cout << "\nsemaphore_peek after 100000 wait-call = " << s.peek_value() << std::flush;

    for(auto& x:threads) x.join(); 
    std::cout << "\n";
}

