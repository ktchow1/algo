#pragma once
#include<vector>
#include<thread>         //  std::thread
#include<pthread.h>      // posix thread
#include<sched.h>        // affinity and realtime
#include<unistd.h>       // priority
#include<sys/resource.h> // priority
#include<sys/syscall.h>  // tid

//  Remark :
//
// 1. pthread_self() == std::this_thread::native_handle()
// 2. policy         =  SCHED_RR / SCHED_FIFO 


// **************** //
// *** Affinity *** //
// **************** //
namespace alg
{
    inline bool set_thread_affinity(auto thread_handle, const std::vector<std::uint32_t>& affinity)
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        for(const auto& x:affinity)
        {
            CPU_SET(x, &cpuset);  
        }
        auto rc = pthread_setaffinity_np(thread_handle, sizeof(cpuset), &cpuset); 
        if (rc != 0)
        {
            return false;
        }
        return true;
    }

    inline bool set_this_thread_affinity(const std::vector<std::uint32_t>& affinity)
    {
        return set_thread_affinity(pthread_self(), affinity);
    }

    inline bool set_thread_affinity(auto thread_handle, std::uint32_t affinity)
    {
        return set_thread_affinity(thread_handle, std::vector<std::uint32_t>{affinity});
    }

    inline bool set_this_thread_affinity(std::uint32_t affinity)
    {
        return set_thread_affinity(pthread_self(), affinity);
    }
}


// ***************************** //
// *** Priority (nice value) *** //
// ***************************** //
namespace alg
{
    inline bool set_this_thread_priority() 
    {
        if (setpriority(PRIO_PROCESS, 0, -20) < 0)
        {
            return false;
        }
        return true;
    }
}


// ************** //
// *** Policy *** //
// ************** //
namespace alg
{
    inline bool set_thread_policy(auto thread_handle, auto policy) 
    {
        struct sched_param sp;
        sp.sched_priority = sched_get_priority_max(policy);

        auto rc = pthread_setschedparam(thread_handle, policy, &sp);
        if (rc != 0)
        {
            return false;
        }   
        return true;
    }

    inline bool set_this_thread_policy(auto policy) 
    {
        return set_thread_policy(pthread_self(), policy);
    }
}

