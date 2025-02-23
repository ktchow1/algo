#pragma once
#include<locked_queue.h>
#include<lockfree_queue.h>
#include<synchronization.h>
#include<thread.h>


// ************************************* //
// T = task 
// Q = multi-thread safe queue
// S = synchronization primitive
// ************************************* //
// S = sync_futex        work, 1.7us 
// S = sync_pmutex       deadlock
// S = sync_HansBarz     work, 1.7us 
// S = sync_psemaphore   work, 1.7us
// S = sync_condvar      work, 2.2us
// ************************************* //
namespace alg
{
    template<std::invocable<std::uint32_t> T,                     // <---- invocable with thread_id as unary arg 
             template<typename> typename Q = mutex_locked_queue,  
             sync_primitive S = sync_psemaphore>       

    class threadpool_sync
    {
    public:
        threadpool_sync() = delete;
       ~threadpool_sync()
        { 
            for(std::uint32_t n=0; n!=threads.size(); ++n) 
            {
                threads[n].join();
            }
        }

        threadpool_sync(const threadpool_sync&) = delete;
        threadpool_sync(threadpool_sync&&) = default;
        threadpool_sync& operator=(const threadpool_sync&) = delete;
        threadpool_sync& operator=(threadpool_sync&&) = default;

        explicit threadpool_sync(std::uint32_t num_threads) : run(true), threads(), task_queue(), sync() 
        {
            for(std::uint32_t n=0; n!=num_threads; ++n)
            {
                threads.emplace_back(std::thread(&threadpool_sync<T,Q>::thread_fct, this, n));
            }
        }

        // ****************************************** //
        // NOT USED FOR HOME-DESKTOP. THIS IS FOR :
        // * RELEASE MODE
        // * sudo TO ALLOW CHANGE IN SCHEDULE POLICY
        // ****************************************** //
        threadpool_sync(std::uint32_t num_threads, const std::vector<std::uint32_t>& affinity) : threadpool_sync(num_threads)
        {   
            for(auto& x:threads) 
            {
                set_thread_affinity(x.native_handle(), affinity);
                set_thread_policy  (x.native_handle(), SCHED_RR);
            } 
        }

    public: 
        void stop()
        {
            run.store(false);
            for(std::uint32_t n=0; n!=threads.size(); ++n) 
            {
                sync.notify();
            }
        }

    public: 
        // ************************* //    
        // *** Producer of tasks *** //
        // ************************* //    
        template<typename... ARGS>
        bool emplace_task(ARGS&&... args)
        {
            if (task_queue.emplace(std::forward<ARGS>(args)...))
            {
                sync.notify(); 
                return true;
            }
            else
            {
                return false;
            }
        }

        template<typename... ARGS>
        void emplace_task_until_success(ARGS&&... args)
        {
            while(!task_queue.emplace(std::forward<ARGS>(args)...));
            sync.notify(); 
        }

    private:
        // ************************* //    
        // *** Consumer of tasks *** //
        // ************************* //    
        void thread_fct(std::uint32_t thread_id)
        {
            // Decouple two checking :
            // 1. is threadpool_sync running 
            // 2. is task queue empty 
            //
            // Adventages of decoupling :
            // 1. avoid repeated lock & unlock on peeking queue size
            // 2. avoid lost notification on stopping threadpool_sync

            while(run.load())
            {
                sync.wait();
                auto task = task_queue.pop();
                if (task)
                {
                    (*task)(thread_id);
                }
            }

            // All threads are now spinning (no more waiting).
            while(task_queue.peek_size() > 0)
            {
                auto task = task_queue.pop();
                if (task)
                {
                    (*task)(thread_id);
                }
            }
        }

    private:
        std::atomic<bool> run;
        std::vector<std::thread> threads;
        Q<T> task_queue;
        S sync;
    };
}
