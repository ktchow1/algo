#include<iostream>
#include<map>
#include<thread.h>
#include<spinlock.h>


template<typename LOCK>
void test_lock(const std::string& test_name, std::uint32_t num_threads)
{
    std::map<std::uint32_t, std::uint32_t> hist; // common resource
    std::vector<std::thread> threads;

    for(std::uint32_t n=0; n!=num_threads; ++n)
    {
    }

    for(auto& x:threads)
    {
        x.join();
    } 

    // checking
}

void test_spinlock_mutex()
{
    test_lock<alg::spinlock>("[Test spinlock]", 3);
    test_lock<std::mutex>("[Test mutex]", 3);
}
