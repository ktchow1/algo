#include<iostream>
#include<locked_queue.h>
#include<lockfree_queue.h>
#include<lockfree_queue_test.h>
/*
#include<boost/lockfree/spsc_queue.hpp>
#include<boost/lockfree/queue.hpp> 
*/


// **********************************************************************************************
// Here are the results for 3 producers, 3 consumers, 50000 tasks, 
// with 300ns waiting time between any two productions in producer (unit = nanosec) :
//
// percentile           | 0.1 |   1 |   5 |  10 |  25 |  50 |  75 |   90 |   95 |   99 | 99.9   |  
// ---------------------+-----+-----+-----+-----+-----+-----+-----+------+------+------+--------+
//    my locked   queue | 141 | 142 | 146 | 151 | 176 | 197 | 546 | 1527 | 2481 | 5798 |   12ms | 
//    my lockfree queue |  83 |  88 | 116 | 117 | 119 | 122 | 126 |  128 |  130 |  135 |  151   | 
// boost lockfree spscq |  86 |  97 | 106 | 108 | 112 | 116 | 122 |  127 |  130 |  138 |  157   | 
// boost lockfree queue | 166 | 172 | 178 | 181 | 188 | 195 | 210 |  228 |  252 |  282 |  318   | 
// **********************************************************************************************
// Affinity is a must
// Priority (using nice value)  is a must
// Priority (using FIFO policy) should be avoided, it kills mpmcq
// **********************************************************************************************
  

void test_lockfree_queue()
{
    using QUEUE0 = alg::  locked_queue<mpmcq_task, alg::spinlock>;
    using QUEUE1 = alg::lockfree_queue<mpmcq_task, 1024>;
//  using QUEUE2 = boost::lockfree::spsc_queue<mpmcq_task, boost::lockfree::capacity<1024>>;
//  using QUEUE3 = boost::lockfree::     queue<mpmcq_task>;

    QUEUE0 queue0;
    QUEUE0 queue1;
//  QUEUE0 queue2;
//  QUEUE0 queue3(1024);

    std::uint32_t num_producers = 3;
    std::uint32_t num_consumers = 3;
    std::uint32_t num_tasks = 20000;

    alg::run_mpmcq("test alg::locked_queue   ", queue0, num_producers, num_consumers, num_tasks);
    alg::run_mpmcq("test alg::lockfree_queue ", queue1, num_producers, num_consumers, num_tasks);
//  alg::run_mpmcq("test boost::lockfree_spsc", queue2, 1, 1, num_tasks);
//  alg::run_mpmcq("test boost::lockfree_mpmc", queue3, num_producers, num_consumers, num_tasks);
} 



