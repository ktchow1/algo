#include<iostream>
#include<locked_queue.h>
#include<lockfree_queue.h>
#include<threadpool.h>

// *** test *** //
#include<run_mpmcq.h>
#include<run_threadpool.h>

// *** boost *** // 
// #include<boost/lockfree/spsc_queue.hpp> 
// #include<boost/lockfree/queue.hpp> 



// **********************************************************************************************
// Here are the results for 3 producers, 3 consumers, 50000 tasks (in Yubo machine) 
// with 300ns waiting time between any two productions in producer (unit = nanosec) :
//
// percentile           | 0.1 |   1 |   5 |  10 |  25 |  50 |  75 |   90 |   95 |   99 | 99.9   |  
// ---------------------+-----+-----+-----+-----+-----+-----+-----+------+------+------+--------+
//   alg locked   queue | 141 | 142 | 146 | 151 | 176 | 197 | 546 | 1527 | 2481 | 5798 |   12ms | 
//   alg lockfree queue |  83 |  88 | 116 | 117 | 119 | 122 | 126 |  128 |  130 |  135 |  151   | 
// boost lockfree spscq |  86 |  97 | 106 | 108 | 112 | 116 | 122 |  127 |  130 |  138 |  157   | 
// boost lockfree queue | 166 | 172 | 178 | 181 | 188 | 195 | 210 |  228 |  252 |  282 |  318   | 
// **********************************************************************************************
// Affinity is a must
// Priority (using nice value)  is a must
// Priority (using FIFO policy) should be avoided, it kills mpmcq
// **********************************************************************************************
  

void test_lockfree_queue()
{
    using QUEUE0 = alg::  locked_queue<mpmcq_task, std::mutex>;
    using QUEUE1 = alg::  locked_queue<mpmcq_task, alg::spinlock>;
    using QUEUE2 = alg::lockfree_queue<mpmcq_task, 1024>;

    std::uint32_t num_producers = 3;
    std::uint32_t num_consumers = 3;
    std::uint32_t num_tasks = 20000;

    alg::run_mpmcq<QUEUE0>("test alg::(mutex) locked_queue ", num_producers, num_consumers, num_tasks);
    alg::run_mpmcq<QUEUE1>("test alg::(spin)  locked_queue ", num_producers, num_consumers, num_tasks);
    alg::run_mpmcq<QUEUE2>("test alg::lockfree_queue       ", num_producers, num_consumers, num_tasks);
} 


void test_threadpool()
{
    std::uint32_t num_threads = 3;
    std::uint32_t num_tasks = 20000;

    alg::run_threadpool<alg::lockfree_queue_long> ("test threadpool with lockfree_queue_long  ", num_threads, num_tasks);
    alg::run_threadpool<alg::lockfree_queue_short>("test threadpool with lockfree_queue_short ", num_threads, num_tasks);
    alg::run_threadpool<alg::mutex_locked_queue>  ("test threadpool with mutex_locked_queue   ", num_threads, num_tasks);
    alg::run_threadpool<alg::spin_locked_queue>   ("test threadpool with spin_locked_queue    ", num_threads, num_tasks);
}
