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
    class threadpool
    {
    public:
        explicit threadpool(std::uint32_t num_threads) : run(true)
        {
            for(std::uint32_t n=0; n!=num_threads; ++n)
            {
                threads.emplace_back(std::thread(&threadpool::thread_fct, this, n));
            }
        }

        ~threadpool()
        {
            stop();
        }

    public:
        void stop()
        {
            run.store(false);
            condvar.notify_all();
            for(auto& x:threads)
            {
                // BUG : Need to check joinable to avoid multi-join, otherwise it throws
                if (x.joinable()) x.join();
            }
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
        // [Remark 1]
        // BUG : Missing "run.load()" leads to wakeup-miss on stop-request

        void thread_fct(std::uint32_t thread_id)
        {
            try 
            {
                while(run.load())
                {
                    std::optional<std::function<void(std::uint32_t)>> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condvar.wait(lock, [this]() { return !task_queue.empty() || !run.load(); }); // <--- Remark 1
                        task = pop_task();
                    }
                    if (task) 
                    {
                        (*task)(thread_id);
                    }
                }

                // STOP REQUESTED, ALL THREADS RUNNING, NO WAITING.
                while(!task_queue.empty())                           // <--- one queue-empty-check before mutex
                {
                    std::optional<std::function<void(std::uint32_t)>> task;
                    {
                        std::lock_guard<std::mutex> lock(mutex);
                        task = pop_task();                           // <--- one queue-empty-check after mutex
                    }
                    if (task) 
                    {
                        (*task)(thread_id);
                    }
                }
            }
            catch(std::exception& e) // BUG : Need to handle exception thrown from task
            {
                std::cout << "\nException thrown from alg::threadpool, thread_id " << thread_id << ", e = " << e.what() << std::flush;
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

    private:
        std::atomic<bool> run;
        std::vector<std::thread> threads;
        std::queue<std::function<void(std::uint32_t)>> task_queue;

    private:
        // *** Substitute for sync_primitive in alg::threadpool_sync *** //
        mutable std::mutex mutex;
        std::condition_variable condvar;
    };
}

