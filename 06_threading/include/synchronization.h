#pragma once
#include<mutex>
#include<condition_variable>
#include<future>
#include<array>

// For posix-thread lib
#include<stdint.h>    
#include<unistd.h>    
#include<semaphore.h> 
#include<linux/futex.h> 
#include<sys/syscall.h> // for SYS_futex
#include<sys/time.h> 


namespace alg
{
    template<typename T> 
    concept sync_primitive = requires(T x)
    {
        x.wait();
        x.notify();
    };


    // ********************************* //
    // *** Implementation with futex *** //
    // ********************************* //
    class sync_futex
    {
    public:
        // This thread is blocked when futex == blocking_value.
        sync_futex() : blocking_value(0)
        {
        }

       ~sync_futex() = default;
        sync_futex(const sync_futex&) = delete;
        sync_futex(sync_futex&&) = default;
        sync_futex& operator=(const sync_futex&) = delete;
        sync_futex& operator=(sync_futex&&) = default;

    public: 
        void wait()
        {
            syscall(SYS_futex, &futex, FUTEX_WAIT, blocking_value, NULL, NULL, 0); 
            ++blocking_value;
        }

        void notify()
        {
            futex.fetch_add(1);
            syscall(SYS_futex, &futex, FUTEX_WAKE, 1, NULL, NULL, 0); 
        }

    private:
        std::atomic<std::int32_t> futex;
        std::int32_t blocking_value;
    };


    // ******************************************************** //
    // *** Implementation with std::mutex (single consumer) *** //
    // ******************************************************** //
    class sync_mutex
    {
    public:
        sync_mutex() : mutex()
        {
            mutex.lock();
        }

       ~sync_mutex() 
        {
            mutex.unlock();
        }

        sync_mutex(const sync_mutex&) = delete;
        sync_mutex(sync_mutex&&) = default;
        sync_mutex& operator=(const sync_mutex&) = delete;
        sync_mutex& operator=(sync_mutex&&) = default;

    public:
        inline void wait()
        {
            mutex.lock(); 
        }

        inline void notify()
        {
            mutex.unlock(); 
        }

    private:
        std::mutex mutex;
    };


    // *********************************************************** //
    // *** Implementation with pthread mutex (single consumer) *** //
    // *********************************************************** //
    class sync_pmutex 
    {
    public:
        sync_pmutex()
        {
            pthread_mutex_init(&mutex, NULL);
            pthread_mutex_lock(&mutex);
        }

       ~sync_pmutex() 
        {
            pthread_mutex_unlock(&mutex);
            pthread_mutex_destroy(&mutex);
        }

        sync_pmutex(const sync_pmutex&) = delete;
        sync_pmutex(sync_pmutex&&) = default;
        sync_pmutex& operator=(const sync_pmutex&) = delete;
        sync_pmutex& operator=(sync_pmutex&&) = default;

    public:
        inline void wait()
        {
            pthread_mutex_lock(&mutex);
        }

        inline void notify()
        {
            pthread_mutex_unlock(&mutex);
        }

    private:
        pthread_mutex_t mutex;
    };


    // ******************************************************** //
    // *** Implementation with pthread mutex (Hans W. Barz) *** //
    // ******************************************************* //
    // 1. count is protected by cs_mutex, hence inc/dec are atomic
    // 2. waiting threads on P are waked either by :
    // -- other threads completing P and count N->N-1, where N>1
    // -- other threads completing V and count 0->1
    // ******************************************************* //
    class sync_HansBarz  
    {
    public:
        sync_HansBarz() : count(0)
        {
            pthread_mutex_init(&cs_mutex, NULL);
            pthread_mutex_init(&pv_mutex, NULL);
            pthread_mutex_lock(&pv_mutex);
        }

       ~sync_HansBarz() 
        {
            pthread_mutex_unlock (&pv_mutex);
            pthread_mutex_destroy(&pv_mutex);
            pthread_mutex_destroy(&cs_mutex);
        }

        sync_HansBarz(const sync_HansBarz&) = delete;
        sync_HansBarz(sync_HansBarz&&) = default;
        sync_HansBarz& operator=(const sync_HansBarz&) = delete;
        sync_HansBarz& operator=(sync_HansBarz&&) = default;

    public:
        inline void wait()
        {
            pthread_mutex_lock(&pv_mutex); // wait ... 
            pthread_mutex_lock(&cs_mutex);
            --count;
            if (count > 0)
            {
                pthread_mutex_unlock(&pv_mutex);
            }
            pthread_mutex_unlock(&cs_mutex);
        }

        inline void notify()
        {
            pthread_mutex_lock(&cs_mutex);
            ++count;
            if (count == 1)
            {
                pthread_mutex_unlock(&pv_mutex); // notify ... 
            }
            pthread_mutex_unlock(&cs_mutex);
        }

    private:
        std::int32_t count;
        pthread_mutex_t cs_mutex; // for critical session protection
        pthread_mutex_t pv_mutex; // for signaling, i.e. p and v
    }; 
      

    // ***************************************** //
    // *** Implementation with std semaphore *** //
    // ***************************************** //
    class sync_semaphore
    {
    public:
        sync_semaphore() : semaphore(0) // initial count is zero, i.e. consumer must wait 
        {
        }

       ~sync_semaphore() = default;
        sync_semaphore(const sync_semaphore&) = delete;
        sync_semaphore(sync_semaphore&&) = default;
        sync_semaphore& operator=(const sync_semaphore&) = delete;
        sync_semaphore& operator=(sync_semaphore&&) = default;

    public:
        inline void wait()
        {
            semaphore.acquire();
        }

        inline void notify()
        {
            semaphore.release();
        }

    private:
        std::counting_semaphore<1> semaphore;
    };   


    // ********************************************* //
    // *** Implementation with pthread semaphore *** //
    // ********************************************* //
    class sync_psemaphore
    {
    public:
        sync_psemaphore()
        {
            sem_init(&semaphore, 0, 0); 
            // arg[1] : 0 for multi-thread, 1 for multi-process
            // arg[2] : initial value
        }

       ~sync_psemaphore() 
        {
            sem_destroy(&semaphore);
        }

        sync_psemaphore(const sync_psemaphore&) = delete;
        sync_psemaphore(sync_psemaphore&&) = default;
        sync_psemaphore& operator=(const sync_psemaphore&) = delete;
        sync_psemaphore& operator=(sync_psemaphore&&) = default;

    public:
        inline void wait()
        {
            sem_wait(&semaphore);
        }

        inline void notify()
        {
           sem_post(&semaphore);
        }

        inline auto peek_value() 
        {
            std::int32_t x;
            sem_getvalue(&semaphore, &x);
            return x;
        }

    private:
        sem_t semaphore;
    };


    // ********************************************** //
    // *** Implementation with condition variable *** //
    // ********************************************** //
    class sync_condvar
    {
    public:
        sync_condvar() : count(0)
        {
        }

       ~sync_condvar() = default;
        sync_condvar(const sync_condvar&) = delete;
        sync_condvar(sync_condvar&&) = default;
        sync_condvar& operator=(const sync_condvar&) = delete;
        sync_condvar& operator=(sync_condvar&&) = default;

    public: 
        void wait()
        {
            std::unique_lock<std::mutex> lock(mutex);
            while(count == 0) cv.wait(lock); 
            --count;
        }

        void notify()
        {
            {
                std::lock_guard<std::mutex> lock(mutex);
                ++count;
            }
            cv.notify_one();
        }

    private:
        std::mutex mutex;
        std::condition_variable cv;
        std::uint32_t count; // emulate a semaphore
    };


    // ********************************************** //
    // *** Implementation with promise and future *** //
    // ********************************************** //
    template<std::uint32_t N>
    class sync_promfut
    {
    public:
        sync_promfut() : p_index(0), f_index(0), promises{}, futures{}
        {
            for(std::uint32_t n=0; n!=N ; ++n)
            {
                futures[n] = promises[n].get_future();
            }
        }

       ~sync_promfut() = default;
        sync_promfut(const sync_promfut&) = delete; 
        sync_promfut(sync_promfut&&) = default; 
        sync_promfut& operator=(const sync_promfut&) = delete; 
        sync_promfut& operator=(sync_promfut&&) = default; 

    public:
        void wait()
        {
            auto n = f_index.fetch_add(1);
            if (n < N) 
            {
                futures[n].get();
            }
        }

        void notify()
        {
            auto n = p_index.fetch_add(1);
            if (n < N)
            {
                promises[n].set_value();
            }
        }

    private:
        std::atomic<std::uint32_t> p_index;
        std::atomic<std::uint32_t> f_index;
        std::array<std::promise<void>, N> promises;
        std::array<std::future<void>, N> futures;
    };
    

    // ********************************** //
    // *** Implementation with atomic *** //
    // ********************************** //
    class sync_atomic
    {
    public:
        sync_atomic() : m_flag(false)
        {
        }

       ~sync_atomic() = default;
        sync_atomic(const sync_atomic&) = delete;
        sync_atomic(sync_atomic&&) = default;
        sync_atomic& operator=(const sync_atomic&) = delete;
        sync_atomic& operator=(sync_atomic&&) = default;

    public: 
        void wait()
        {
            while(!m_flag.load(std::memory_order_acquire))
            {
                std::this_thread::yield();
            }
            m_flag.store(false, std::memory_order_release); // Don't forget this
        }

        void notify()
        {
            m_flag.store(true, std::memory_order_release);
        }

    private:
        std::atomic<bool> m_flag;
    };





    /*
    auto synchronization_with_atomic(std::uint32_t cpu0, std::uint32_t cpu1)
    {
        // *** Sync mechanism *** //
        std::atomic<std::uint32_t> trigger = 0;
        std::atomic<std::uint32_t> response = 0;
        constexpr std::uint32_t N = 1000;

        // *** Measurement *** //
        timespec ts0;
        timespec ts1;
        timespec tsM;
        alg::statistics<std::uint64_t> stat0M;
        alg::statistics<std::uint64_t> statM1;

        // *************** //
        // *** Reactor *** //
        // *************** //
        std::thread reactor([&]()
        {
            alg::set_this_thread_affinity(cpu0);        
            alg::set_this_thread_policy(SCHED_FIFO);        
            for(std::uint32_t n=0; n!=N; ++n)
            {
                while(trigger.load()!=n+1) 
                {
                    __builtin_ia32_pause();
                //  std::this_thread::yield(); // very slow response time
                }
                clock_gettime(CLOCK_MONOTONIC, &tsM);
                response.fetch_add(1);
            }
        });

        // ***************** //
        // *** Initiator *** //
        // ***************** //
        alg::set_this_thread_affinity(cpu1);        
        alg::set_this_thread_policy(SCHED_FIFO);        
        for(std::uint32_t n=0; n!=N; ++n)
        {
            clock_gettime(CLOCK_MONOTONIC, &ts0);
            trigger.fetch_add(1);
            while(response.load()!=n+1) 
            {
                __builtin_ia32_pause();
            //  std::this_thread::yield(); // very slow response time 
            }
            clock_gettime(CLOCK_MONOTONIC, &ts1);


            // *** Print measurement *** //
            std::uint64_t ns0 = to_nanosec(ts0);
            std::uint64_t ns1 = to_nanosec(ts1);
            std::uint64_t nsM = to_nanosec(tsM);
            stat01.add(ns1 - ns0);
            stat0M.add(nsM - ns0);
            statM1.add(ns1 - nsM);

            // *************************************************************************** //
            // There is no way to block initiator before each round, thus we have to wait. //
            // *************************************************************************** //
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }

        reactor.join();
        return std::make_tuple(stat01, stat0M, statM1);
    }

    void time_synchronization_with_atomic()
    {
        alg::statistics<std::uint64_t> stat01;
        alg::statistics<std::uint64_t> stat0M;
        alg::statistics<std::uint64_t> statM1;

        for(std::uint32_t n=0; n!=7; ++n)
        {
            for(std::uint32_t m=n+1; m!=8; ++m)
            {
                auto [a,b,c] = synchronization_with_atomic(n,m);     
                auto [d,e,f] = synchronization_with_atomic(m,n);     
                stat01 += a += d; 
                stat0M += b += e; 
                statM1 += c += f; 
            }
        }
        std::cout << "\nSynchonization with atomic : all-in-one";
        std::cout << "\n[stat01]" << stat01.get_string() << "\n";
        std::cout << "\n[stat0M]" << stat0M.get_string() << "\n";
        std::cout << "\n[statM1]" << statM1.get_string() << "\n";
    } */
}
