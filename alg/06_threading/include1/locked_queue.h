#pragma once
#include<optional>
#include<queue>
#include<mutex>
#include<spinlock.h>


namespace alg
{
    template<typename T, typename LOCK> 
    class locked_queue final
    {
    public:
        using value_type = T;
        using this_type = locked_queue<T, LOCK>;


    public:
        locked_queue() = default;
       ~locked_queue() = default;
        locked_queue(const this_type&) = delete;
        locked_queue(this_type&&) = default;
        this_type& operator=(const this_type&) = delete;
        this_type& operator=(this_type&&) = default;


    public:
        // ********************** //
        // *** Implementation *** //
        // ********************** //
        template<typename... ARGS> 
        bool emplace(ARGS&&... args) noexcept
        {
            std::lock_guard<LOCK> lock(impl);  
            queue.emplace(std::forward<ARGS>(args)...);
            return true;
        }

        std::optional<T> pop() noexcept
        {
            std::lock_guard<LOCK> lock(impl); 
            if (queue.size() == 0) return std::nullopt;
            
            T output = std::move(queue.front());
            queue.pop();
            return std::make_optional(output);
        }

        // ************************************** //
        // *** Interface compatiable to boost *** //
        // ************************************** //
        bool push(const T& item) noexcept
        {
            return emplace(item);
        }

        bool pop(T& item) noexcept
        {
            auto result = pop();
            if (result)
            {
                item = std::move(*result); 
                return true;
            }
            else return false;
        }


    public:
        std::uint32_t peek_size() const noexcept
        {
            std::lock_guard<LOCK> lock(impl);
            return queue.size();
        }
        

    private:
        std::queue<T> queue;
        mutable LOCK impl; // mutex or spinlock
    };

    template<typename T>
    using mutex_locked_queue = locked_queue<T, std::mutex>;

    template<typename T>
    using spin_locked_queue = locked_queue<T, spinlock>;
}
