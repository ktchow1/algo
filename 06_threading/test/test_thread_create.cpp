#include<iostream>
#include<mutex>
#include<thread.h>
#include<timer.h>
#include<statistics.h>


struct task_arg
{
    std::uint32_t              x;    
    std::string                y;
    std::vector<std::uint32_t> z;
};

void* task_pfct(void* arg)
{
    task_arg* p_arg = reinterpret_cast<task_arg*>(arg);
    // do something
    return nullptr;
}

void task_fct(std::uint32_t x, const std::string& y, const std::vector<std::uint32_t>& z)
{
    // do something
}

class task
{
public:
    void fct(std::uint32_t x, const std::string& y, const std::vector<std::uint32_t>& z)
    {
        // do something
    }

    void operator()(std::uint32_t x, const std::string& y, const std::vector<std::uint32_t>& z)
    {
        // do something
    }
};


// ************ //
// *** Test *** //
// ************ //
void create_thread()
{
    std::vector<std::thread> threads;
    task_arg arg;

    // *** Create *** //
    threads.emplace_back(task_fct, arg.x, std::cref(arg.y), std::cref(arg.z));

    // *** Join *** //
    for(auto& x:threads) 
    {
        x.join();        
    }
}

void create_jthread()
{
    std::vector<std::jthread> jthreads;
    task_arg arg;

    // *** Create *** //
    jthreads.emplace_back(task_fct, arg.x, std::cref(arg.y), std::cref(arg.z));

    // *** Auto join *** //
}

void create_pthread()
{
    constexpr std::uint32_t num_threads = 3;
    pthread_t threads[num_threads];
    task_arg arg;

    // *** Create *** //
    for(std::uint32_t n=0; n!=num_threads; ++n) 
    {
        pthread_create(threads+n, NULL, task_pfct, &arg);
        // arg[1] = pthread_attr_t which is NULL for default
        // arg[2] = function taking void* returning void*
        // arg[3] = function argument
    }

    // *** Join *** //
    for(std::uint32_t n=0; n!=num_threads; ++n) 
    {
        pthread_join(threads[n], NULL);        
    }
}

void test_thread_create()
{
    create_thread();
    create_jthread();
    create_pthread();
}
