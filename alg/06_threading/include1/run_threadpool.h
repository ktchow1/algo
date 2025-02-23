#pragma once
#include<iostream>
#include<functional>

// *** alg *** //
#include<run_mpmcq.h>
#include<threadpool_sync.h>
#include<utility.h>


// ******************************** //
// *** Test for threadpool_sync *** //
// ******************************** //
namespace alg
{
    template<template<typename> typename QUEUE>
    void run_threadpool_sync(const std::string& test_name, 
                             std::uint32_t num_threads, 
                             std::uint32_t num_tasks, 
                             std::uint32_t delay_between_tasks_in_us)
    {
        std::vector<task_output> task_outputs(num_tasks);
        std::vector<std::uint32_t> task_counts (num_threads, 0); 


        // ******************* //
        // *** Main thread *** //
        // ******************* //
        {
            alg::threadpool_sync<task_spec, QUEUE> pool(num_threads);
            for(std::uint32_t n=0; n!=num_tasks; ++n)
            {
                while(!pool.emplace_task(task_outputs[n])) 
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(10));
                }
                std::this_thread::sleep_for(std::chrono::microseconds(delay_between_tasks_in_us));
            }
            pool.stop();
        }
        // threadpool_sync is destructed, joins all threads, task_outputs are ready
          

        // **************** //
        // *** Checking *** //
        // **************** //
        std::string comment = task_check(num_threads, task_outputs);
        print_summary(test_name, "succeeded, " + comment);
    }
}


// ************************************************************ //
// *** Test for threadpool / threadpool_j / threadpool_jcrt *** //
// ************************************************************ //
