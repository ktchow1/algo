#pragma once

// **************************************** //
// *** Preliminary design of shared_ptr *** //
// **************************************** //
template<typename T> 
class shared_ptr
{	
private:
    struct manager
    {
        manager(std::uint32_t n) : count(n)
        {
        }

        std::uint32_t count;
    };

public: 
    explicit shared_ptr(T* ptr = nullptr) : manager_ptr(nullptr), resource_ptr(nullptr)
    {
        if (ptr)
        {
            manager_ptr = new manager(1);
            resource_ptr = ptr;
        }
    }												
										
    ~shared_ptr()
    {
        decrement();
    }
                                    
    shared_ptr(shared_ptr<T>& rhs) 	
    {
        manager_ptr = rhs.manager_ptr;
        resource_ptr = rhs.resource_ptr;
        increment();
    }

    shared_ptr<T>& operator=(shared_ptr<T>& rhs)
    {
        decrement();
        manager_ptr = rhs.manager_ptr;
        resource_ptr = rhs.resource_ptr;
        increment();
        return *this;
    }

    shared_ptr(shared_ptr<T>&& rhs) 
    {
        manager_ptr = rhs.manager_ptr;
        resource_ptr = rhs.resource_ptr;
        rhs.manager_ptr = nullptr;
        rhs.resource_ptr = nullptr;
    }	

    shared_ptr<T>& operator=(shared_ptr<T>&& rhs)
    {
        decrement();
        manager_ptr = rhs.manager_ptr;
        resource_ptr = rhs.resource_ptr;
        rhs.manager_ptr = nullptr;
        rhs.resource_ptr = nullptr;
        return *this;
    }

    std::uint32_t ref_count() const { return  (manager_ptr? manager_ptr->count:0); }
    operator bool()           const { return (resource_ptr!=nullptr); }
    const T& operator *()     const { return *resource_ptr;  }
    const T* operator->()     const { return  resource_ptr;  }
    T& operator *()                 { return *resource_ptr;  }
    T* operator->()                 { return  resource_ptr;  }

private:    
    void increment()
    {
        if (manager_ptr)
        {
            ++(manager_ptr->count); 
        }
    }

    void decrement()
    {	
        if (manager_ptr && manager_ptr->count > 0)
        {
            --(manager_ptr->count);
            if (manager_ptr->count == 0)
            {
                delete manager_ptr;	
                delete resource_ptr;	
                manager_ptr = nullptr;
                resource_ptr = nullptr;
            }		
        }
    }

private:    
    manager* manager_ptr;
    T*       resource_ptr;
};
 
