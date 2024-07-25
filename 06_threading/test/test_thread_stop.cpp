#include<iostream>
#include<vector>
#include<thread>
#include<functional> // for std::placeholders


void test_std_thread_stop()
{
    std::cout << "\n[Stop source for std::thread]";

    std::stop_source s_source;
    std::vector<std::thread> threads;
    for(std::uint32_t id=0; id!=5; ++id)
    {
        threads.push_back(std::thread([s_token = s_source.get_token()](std::uint32_t thread_id)
        {
            std::uint32_t n = 0;
            while(!s_token.stop_requested())
            {
                std::cout << "\nthread " << thread_id << ", loop " << n << std::flush; 
                std::this_thread::sleep_for(std::chrono::milliseconds(400 + 40 * thread_id));
                ++n;
            }
            std::cout << "\nthread " << thread_id << ", done" << std::flush; 
        }, id));
    }
    std::cout << "\nmain thread sleep for 3 secs"; 
    std::this_thread::sleep_for(std::chrono::seconds(3));

    s_source.request_stop();
    for(auto& x:threads) x.join();
    std::cout << "\n";
}
  
// ***************************** //
// Jthread is for RAII, as it :
// 1. stop on destructor
// 2. join on destructor
// ***************************** //
void test_std_jthread_stop()
{
    std::cout << "\n[Stop source for std::jthread]";

//  std::stop_source s_source; // Change 1
    std::vector<std::jthread> threads;
    for(std::uint32_t id=0; id!=5; ++id)
    {
        // Change 2 : captured token becomes arg
        threads.push_back(std::jthread([](std::stop_token s_token, std::uint32_t thread_id)
        {
            std::uint32_t n = 0;
            while(!s_token.stop_requested())
            {
                std::cout << "\njthread " << thread_id << ", loop " << n << std::flush; 
                std::this_thread::sleep_for(std::chrono::milliseconds(400 + 40 * thread_id));
                ++n;
            }
            std::cout << "\njthread " << thread_id << ", done" << std::flush; 
        }, id)); 
    //  }, std::placeholders::_1, id)); // Why no placeholders needed?
    }
    std::cout << "\nmain thread sleep for 3 secs"; 
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Change 3 : Stop and join are auto
//  s_source.request_stop();
//  for(auto& x:threads) x.join(); 
    std::cout << "\n";
} 

void test_thread_stop()
{
    test_std_thread_stop();
    test_std_jthread_stop();
    std::cout << "\n";
}
