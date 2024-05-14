#include<iostream>
#include<array>


template<typename T>
struct node
{
    T value; // replace with std::array<char, sizeof(T)> to avoid requiring T to be default constructible
    node<T>* next;
};

template<typename T, std::uint32_t N> class object_pool
{
public:
    object_pool() : head(&impl[0])
    {
        for(std::uint32_t n=0; n!=N-1; ++n)
        {
            impl[n].next = &impl[n+1];
        }          
        impl[N-1].next = nullptr;
    }

    template<typename...ARGS>
    node<T>* request(ARGS&&...args)
    {
        if (head)
        {
            auto this_node = head;
            head = head->next_node;
            new (&this_node->value) T{std::forward<ARGS>(args)...};
            return this_node; 
        }
        return nullptr;
    }

    void release(node<T>* this_node)
    {
        this_node->value.~T();
        this_node->next = head;
        head = this_node;
    }

private:
    node<T>* head;
    std::array<node<T>,N> impl;
};

template<typename T, typename POOL> class stack
{
public:
    stack(POOL& pool) : pool(pool), head(nullptr)
    {
    }
    
    bool is_empty() const noexcept    
    {
        return !head;
    }
    
    const T& top() const noexcept
    {
        head->value;
    }   
    
    T& top() noexcept
    {
        head->value;
    }  
 
    template<typename...ARGS>
    bool push(ARGS&&...args) // return false when out-of-memory
    {
        node<T>* this_node = pool.request(std::forward<ARGS>(args)...);
        if (this_node)
        {
            this_node->next = head;
            head = this_node;
            return true;
        }
        return false;
    }

    bool pop() // return false when stack is empty
    {
        if (head)
        { 
            node<T>* del_node = head; 
            head = head->next;
            pool.release(del_node);
            return true;
        }
        return false;
    }

private:
    POOL& pool;
    node<T>* head;
};

/*
object_pool<T,1000> pool;
stack<T> s0(pool);
stack<T> s1(pool);
stack<T> s2(pool);
for(std::uint32_t n=0; n!=100; ++n) s0.push(randX(), randY(), randZ());
for(std::uint32_t n=0; n!=200; ++n) s1.push(randX(), randY(), randZ());
for(std::uint32_t n=0; n!=300; ++n) s2.push(randX(), randY(), randZ());
*/


