#include<iostream>
#include<unordered_set>
#include<thread.h>
#include<timer.h>
#include<statistics.h>

// ***************************** //
// *** Posix thread affinity *** //
// ***************************** //
struct thread_data
{
    std::uint32_t id;                         //  input
    std::uint64_t aff_ns;                     // output (time for changing affinity) 
    std::uint64_t pri_ns;                     // output (time for changing priority)
    std::unordered_set<std::uint32_t> cores0; // output (core used by this thread before setting affinity)
    std::unordered_set<std::uint32_t> cores1; // output (core used by this thread after setting affinity)

    inline void reset(std::uint32_t n)
    {
        id = n;
        aff_ns = 0;
        pri_ns = 0;
        cores0.clear();
        cores1.clear();
    }
};

void* thread_fct(void* arg)
{
    thread_data* data_ptr = reinterpret_cast<thread_data*>(arg);

    // *** Sample core used for this thread *** //
    for(std::uint32_t n=0; n!=20; ++n)
    {
        std::this_thread::yield();
        data_ptr->cores0.insert(sched_getcpu());
    }

    // *** Chanage affinity *** //
    alg::timer timer;

    timer.click();
    alg::set_this_thread_affinity(data_ptr->id);
    timer.click();
    data_ptr->aff_ns = timer.time_elapsed_in_nsec();

    timer.click();
    alg::set_this_thread_policy(SCHED_RR);
    timer.click();
    data_ptr->pri_ns = timer.time_elapsed_in_nsec();

    // *** Sample core used for this thread *** //
    for(std::uint32_t n=0; n!=20; ++n)
    {
        std::this_thread::yield();
        data_ptr->cores1.insert(sched_getcpu());
    }
    return NULL;
}

void test_pthread()
{
    constexpr std::uint32_t num_threads = 3;
    alg::statistics<std::uint64_t> aff_stat;               // time for changing affinity 
    alg::statistics<std::uint64_t> pri_stat;               // time for changing priority      
    std::unordered_set<std::uint32_t> cores0[num_threads]; // core-id before setting affinity
    std::unordered_set<std::uint32_t> cores1[num_threads]; // core-id after  setting affinity

    std::cout << "\nNext test progress ";
    for(std::uint32_t n=0; n!=1000; ++n)
    {
        if(n%20==0) std::cout << "." << std::flush;        
        pthread_t threads[num_threads];
        thread_data data[num_threads];

        // *** Create *** //
        for(std::uint32_t n=0; n!=num_threads; ++n) 
        {
            data[n].reset(n);
            pthread_create(threads+n, NULL, thread_fct, data+n);
            // arg[1] = pthread_attr_t which is NULL for default
            // arg[2] = function taking void* returning void*
            // arg[3] = function argument
        }

        // *** Join *** //
        for(std::uint32_t n=0; n!=num_threads; ++n) 
        {
            pthread_join(threads[n], NULL);        
        }

        // *** Measurement *** //
        for(std::uint32_t n=0; n!=num_threads; ++n)            
        {
            aff_stat.add(data[n].aff_ns);
            pri_stat.add(data[n].pri_ns);
            cores0[n].insert(data[n].cores0.begin(), data[n].cores0.end());
            cores1[n].insert(data[n].cores1.begin(), data[n].cores1.end());
        }
    }

    std::cout << "\n\n[Core used by different thread]";
    for(std::uint32_t n=0; n!=num_threads; ++n)            
    {
        std::cout << "\nthread " << n << " before assigning affinity : "; for(const auto&x : cores0[n]) std::cout << x << " ";
        std::cout << "\nthread " << n << " after  assigning affinity : "; for(const auto&x : cores1[n]) std::cout << x << " ";
    }
    std::cout << "\n\n[Time for setting affinity]" << aff_stat.get_string();
    std::cout << "\n\n[Time for setting priority]" << pri_stat.get_string() << "\n";
}

