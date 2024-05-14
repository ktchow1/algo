#include<iostream>


template<typename T>
class array // copyable and movable
{
public:
    array(std::uint32_t cap) : ptr(new char[cap*sizeof(T)]), size(0), capacity(cap)
    {
    }

    array(const array& rhs) : ptr(new char[rhs.capacity*size(T)]), size(rhs.size), capacity(rhs.capacity)
    {
        std::memcpy(ptr, rhs.ptr, capacity*size(T));
    }

    array<T>& operator=(const array& rhs) 
    {
        // step 1
        destruct_T();

        // step 2
        ptr = new char[rhs.capacity*size(T)];
        size = rhs.size;
        capacity = rhs.capacity;
        std::memcpy(ptr, rhs.ptr, capacity*size(T));

        // step 3

        // step 4
        return *this;
    }

    array(array<T>&& rhs) : ptr(rhs.ptr), size(rhs.size), capacity(rhs.capacity)
    {
        rhs.ptr = nullptr;
        rhs.size = 0;
        rhs.capacity = 0;
    }

    array<T>& operator=(array<T>&& rhs) 
    {
        // step 1 
        destruct_T();
        
        // step 2
        ptr = rhs.ptr;
        size = rhs.size; 
        capacity = rhs.capacity;

        // step 3
        rhs.ptr = nullptr;
        rhs.size = 0;
        rhs.capacity = 0;

        // step 4
        return *this;
    }

    ~array()
    {
        destruct_T();
        size = 0;
        capacity = 0;
    }

public:
    template<typename...Ts>
    void push_back(Ts&&...args) 
    {
        new (&reinterpret_cast<T*>(ptr)[size]) T(std::forward<Ts>(args)...);
    }

    const T& operator[](std::uint32_t n) const noexcept
    {
        return reinterpret_cast<const T*>(ptr)[n];
    }

    T& operator[](std::uint32_t n) noexcept
    {
        return reinterpret_cast<const T*>(ptr)[n];
    }

private:
    void destruct_T() noexcept
    {
        if (ptr) 
        {
            for(std::uint32_t n=0; n!=size; ++n)
            {
                reinterpret_cast<T*>(ptr)[n].~T();
            }
            delete [] ptr;
            ptr = nullptr;
        }
    }

private:
    char* ptr;
    std::uint32_t size;
    std::uint32_t capacity;
};



template<typename T>
class unique_ptr
{
public:
    unique_ptr() : ptr(nullptr)
    {
    }

    explicit unique_ptr(T* p) : ptr(p)
    {
    }

    unique_ptr(const unique_ptr<T>& rhs) = delete;
    unique_ptr(unique_ptr<T>&& rhs) : ptr(rhs.ptr)
    {
        rhs.ptr = nullptr;
    }
    
    unique_ptr<T>& operator=(const unique_ptr<T>& rhs) = delete;
    unique_ptr<T>& operator=(unique_ptr<T>&& rhs)
    {   
        if (ptr) delete ptr;
        ptr = rhs.ptr;
        rhs.ptr = nullptr;
        return *this;
    }
    
    ~unique_ptr()
    {
        if (ptr) delete ptr;
    }

public:
    const T& operator*() const noexcept
    {
        return *ptr;
    }

    T& operator*() noexcept
    {
        return *ptr;
    }

    const T* operator->() const noexcept
    {
        return ptr;
    }

    T* operator->() noexcept
    {
        return ptr;
    }

private:
    T* ptr;
};

template<typename T, typename...ARGS>
unique_ptr<T> make_unique(ARGS&&...args)
{
    return unique_ptr<T>(std::forward<ARGS>(args)...);
}



template<typename T>
struct shared_ptr_manager // POD
{
    std::uint32_t ref_count;
    T* ptr;
};
/*
template<typename T>
class shared_ptr
{
public:
    shared_ptr() : mgr(nullptr)
    {
    }

    shared_ptr(T* p) : mgr(new shared_ptr_manager<T>(1, p))
    {
    }

    shared_ptr(shared_ptr<T>& rhs) : mgr(rhs.mgr)
    {
        increment();
    }
    
    shared_ptr<T>& operator=(shared_ptr<T>& rhs) 
    {
        decrement();
        mgr = rhs.mgr;
        increment();
        return *this;
    }

    shared_ptr(shared_ptr<T>&& rhs) : mgr(rhs.mgr)
    {
        rhs.mgr = nullptr; 
    }
    
    shared_ptr<T>& operator=(shared_ptr<T>& rhs) 
    {
        decrement();
        mgr = rhs.mgr;
        rhs.mgr = nullptr; 
        return *this;
    }

    ~shared_ptr()
    {
        decrement();
    }

public:
    const T& operator*() const noexcept
    {
        return *(mgr->ptr);
    }

    T& operator*() noexcept
    {
        return *(mgr->ptr);
    }

    const T* operator->() const noexcept
    {
        return mgr->ptr;
    }

    T* operator->() noexcept
    {
        return mgr->ptr;
    }

private:
    void increment()
    {
        if (mgr)
        {
            ++mgr->ref_count;
        }
    }

    void decrement()
    {
        if (mgr)
        {
            --mgr->ref_count;
            if (mgr->ref_count == 0)
            {
                delete mgr->ptr;
                delete mgr;
            }
        }
    }

private:
    shared_ptr_manager<T>* mgr;
};

template<typename T, typename...ARGS>
shared_ptr<T> make_shared(ARGS&&...args)
{
    return shared_ptr<T>(std::forward<ARGS>(args)...);
}

*/
