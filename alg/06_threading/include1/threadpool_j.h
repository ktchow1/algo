#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<functional>
#include<thread>
#include<mutex>
#include<condition_variable>


namespace alg
{
    class threadpool_j
    {
    public:
        explicit threadpool_j(std::uint32_t num_threads) 
        {
            for(std::uint32_t n=0; n!=num_threads; ++n)
            {
                jthreads.emplace_back(std::jthread(&threadpool_j::thread_fct, this, s_source.get_token(), n));
            }
        }

        ~threadpool_j()
        {
            stop();
        }

        void stop()
        {
            s_source.request_stop();

            // Unlike the threadpool::stop() :
            // * there is no condvar.notify_all(), because token is passed to condvar::wait()
            // * there is no need to join threads, because jthreads can auto-join
        }

    public: 
        // ************************* //    
        // *** Producer of tasks *** //
        // ************************* //    
        void add_task(const std::function<void(std::uint32_t)>& task)
        {
            {
                std::lock_guard<std::mutex> lock(mutex);
                task_queue.push(task);
            }
            condvar.notify_one();
        }

    private:
        // ************************* //    
        // *** Consumer of tasks *** //
        // ************************* //    
        // [Remark 1] Condition variable wait() will proceed when either :
        // 1. task_queue is non empty, predicate returns true  ---> there must be next task, execute the task 
        // 2. stop requested in token, predicate returns false ---> break while loop, do not execute any task

        void thread_fct(std::stop_token s_token, std::uint32_t thread_id)
        {
            try
            {
                while(!s_source.get_token().stop_requested())
                {
                    std::function<void(std::uint32_t)> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        if (!condvar.wait(lock, s_token, [this]() { return !task_queue.empty(); })) // <--- Remark 1
                        {
                            break; 
                        } 
                        task = pop_task_without_checking();
                    }
                    task(thread_id);
                }

                // STOP REQUESTED, ALL THREADS RUNNING, NO WAITING.
                while(!task_queue.empty())
                {
                    std::optional<std::function<void(std::uint32_t)>> task;
                    {
                        std::lock_guard<std::mutex> lock(mutex);
                        task = pop_task();    
                    }
                    if (task) 
                    {
                        (*task)(thread_id);
                    }
                }
            }
            catch(std::exception& e)
            {
                std::cout << "\nException thrown from alg::threadpool_j, thread_id " << thread_id << ", e = " << e.what() << std::flush;
            }
        }

        // ************************************** //
        // *** Combine std::queue<T>::front() *** //
        // ***    with std::queue<T>::pop()   *** //
        // ************************************** //
        std::optional<std::function<void(std::uint32_t)>> pop_task()
        {
            if (!task_queue.empty())               
            {
                auto task = std::move(task_queue.front());
                task_queue.pop();
                return task;
            }
            return std::nullopt;
        }

        std::function<void(std::uint32_t)> pop_task_without_checking()
        {
            auto task = std::move(task_queue.front());
            task_queue.pop();
            return task;
        }

    private:
        std::stop_source s_source;           
        std::vector<std::jthread> jthreads;
        std::queue<std::function<void(std::uint32_t)>> task_queue;

    private:
        mutable std::mutex mutex;
        std::condition_variable_any condvar;
    };
}
