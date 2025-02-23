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
                // BUG3 : Need to check joinable to avoid multi-join, otherwise it throws
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
        void thread_fct(std::uint32_t thread_id)
        {
            try // BUG4 : Need to handle exception thrown from task
            {
                while(run.load())
                {
                    std::function<void(std::uint32_t)> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condvar.wait(lock, [this]() // Predicate returns true to continue
                        { 
                            return !task_queue.empty() || 
                                   !run.load(); // BUG1 : missing this results in wakeup-miss on termination
                        }); 

                        if (!run.load()) break; // BUG2 : missing this results in popping empty queue on termination
                        task = std::move(task_queue.front());
                        task_queue.pop();
                    }
                    task(thread_id);
                }

                // All threads are now spinning (no more waiting).
                while(!task_queue.empty())
                {
                    std::function<void(std::uint32_t)> task;
                    {
                        std::lock_guard<std::mutex> lock(mutex);
                        task = std::move(task_queue.front());
                        task_queue.pop();
                    }
                    task(thread_id);
                }
            }
            catch(std::exception& e)
            {
                std::cout << "\nException thrown from alg::threadpool, thread_id " << thread_id << ", e = " << e.what() << std::flush;
            }
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

