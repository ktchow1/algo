#include<iostream>
#include<unordered_set>
#include<thread.h>
#include<timer.h>
#include<statistics.h>


struct thread_result
{
    std::uint32_t id;                         //  input
    alg::statistics<std::uint64_t> aff_stat;  // output (time for changing affinity) 
    alg::statistics<std::uint64_t> pri_stat;  // output (time for changing priority)
    std::unordered_set<std::uint32_t> cores0; // output (core used by this thread before setting affinity)
    std::unordered_set<std::uint32_t> cores1; // output (core used by this thread after setting affinity)
};

void* thread_fct(void* arg)
{
    thread_result* result_ptr = reinterpret_cast<thread_result*>(arg);

    // *** Sample core used for this thread *** //
    for(std::uint32_t n=0; n!=10; ++n)
    {
        std::this_thread::yield();
        result_ptr->cores0.insert(sched_getcpu());
    }

    // *** Chanage affinity *** //
    alg::timer timer;

    timer.click();
    alg::set_this_thread_affinity(result_ptr->id);
    timer.click();
    result_ptr->aff_stat.add(timer.time_elapsed_in_nsec());

    timer.click();
    alg::set_this_thread_policy(SCHED_RR);
    timer.click();
    result_ptr->pri_stat.add(timer.time_elapsed_in_nsec());

    // *** Sample core used for this thread *** //
    for(std::uint32_t n=0; n!=10; ++n)
    {
        std::this_thread::yield();
        result_ptr->cores1.insert(sched_getcpu());
    }
    return NULL;
}

void test_pthread()
{
    constexpr std::uint32_t num_threads = 3;
    thread_result results[num_threads];
    for (std::uint32_t n=0; n!=num_threads; ++n)
    {
        results[n].id = n;
    }

    std::cout << "\nNext test progress ";
    for(std::uint32_t n=0; n!=1000; ++n)
    {
        if (n%20==0) std::cout << "." << std::flush;        
        pthread_t threads[num_threads];

        // *** Create *** //
        for(std::uint32_t n=0; n!=num_threads; ++n) 
        {
            pthread_create(threads+n, NULL, thread_fct, results+n);
            // arg[1] = pthread_attr_t which is NULL for default
            // arg[2] = function taking void* returning void*
            // arg[3] = function argument
        }

        // *** Join *** //
        for(std::uint32_t n=0; n!=num_threads; ++n) 
        {
            pthread_join(threads[n], NULL);        
        }
    }

    std::cout << "\n\n[Core used by different thread]";
    for(std::uint32_t n=0; n!=num_threads; ++n)            
    {
        std::cout << "\nthread " << n << " before assigning affinity : "; for(const auto&x : results[n].cores0) std::cout << x << " ";
        std::cout << "\nthread " << n << " after  assigning affinity : "; for(const auto&x : results[n].cores1) std::cout << x << " ";
    }
    std::cout << "\n\n[Time for setting affinity]" << results[0].aff_stat.get_string();
    std::cout << "\n\n[Time for setting priority]" << results[0].pri_stat.get_string() << "\n";
}

