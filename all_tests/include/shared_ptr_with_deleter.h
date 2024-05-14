#pragma once
#include<concepts>

// ******************************* //
// *** Re-design of shared_ptr *** //
// ******************************* //
// Wintermute 2nd round technical interview : 
// - Resource pointer should be indirectly under class shared_ptr<T>.
// - Manager should not contain resource pointer, as it needs 2 redirections.
// - Manager should     contain deleter.
// - Deleter can be any type, we need type-erasure pattern. Stackoverflow 6324694.
//
// 3 main changes are made, as stated in step 1-3 below.
// 3 new / delete operations are involved (as opposed to 2 in original design) :
// - new resource
// - new deleter
// - new manager
// 
// Type T and U are : 
// - T is pointer type 
// - U is physical type, i.e. the type erased
// - T and U must fulfill concept std::assignment_from<T,U>
// - T and U are not necessary derived from same base, but have common interface, which is destructor in this case
//
// Manager must be non-template, which housekeeps a non-template deleter_base pointer.
// Manager must have a template constructor, which allows construction from different types.
// ***************************************************************************************** //

template<typename T> 
class shared_ptr_with_deleter
{	
private:
    // ******************************************************************************************** //
    // Step 1 : Introduce deleter_base, deleter<T> and deleter_base*, called "type-erasure" pattern //
    // ******************************************************************************************** //
    struct deleter_base
    {
        virtual ~deleter_base() {}
        virtual void operator()(void*) = 0;
    };

    template<typename U> requires std::derived_from<U,T>
    struct deleter : public deleter_base
    {
        virtual void operator()(void* ptr)
        {
            delete static_cast<U*>(ptr);
        }
    };

    struct manager
    {

        template<typename U>
        manager(std::uint32_t n, [[maybe_unused]] U* ptr) : count(n), deleter_ptr(new deleter<U>{})
        {
        }

        std::uint32_t count;
        deleter_base* deleter_ptr;
    };

public:
    // ********************************************************************************************** //
    // Step 2 : Make constructor as template with parameter U, put the right type inside type-erasure //
    // ********************************************************************************************** //
    template<typename U>
    explicit shared_ptr_with_deleter(U* ptr = nullptr) : manager_ptr(nullptr), resource_ptr(nullptr)
    {
        if (ptr)
        {
            manager_ptr = new manager(1, ptr);
            resource_ptr = ptr;
        }
    }												
										
    ~shared_ptr_with_deleter()
    {
        decrement();
    }
                                    
    shared_ptr_with_deleter(shared_ptr_with_deleter<T>& rhs) 	
    {
        manager_ptr = rhs.manager_ptr;
        resource_ptr = rhs.resource_ptr;
        increment();
    }

    shared_ptr_with_deleter<T>& operator=(shared_ptr_with_deleter<T>& rhs)
    {
        decrement();
        manager_ptr = rhs.manager_ptr;
        resource_ptr = rhs.resource_ptr;
        increment();
        return *this;
    }

    shared_ptr_with_deleter(shared_ptr_with_deleter<T>&& rhs) 
    {
        manager_ptr = rhs.manager_ptr;
        resource_ptr = rhs.resource_ptr;
        rhs.manager_ptr = nullptr;
        rhs.resource_ptr = nullptr;
    }	

    shared_ptr_with_deleter<T>& operator=(shared_ptr_with_deleter<T>&& rhs)
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
                // ************************************************************* //
                // Step 3 : Delete resource using deleter, instead of default ~T //
                // ************************************************************* //
                (*(manager_ptr->deleter_ptr))(resource_ptr);
                delete manager_ptr->deleter_ptr;
                delete manager_ptr;	
                manager_ptr = nullptr;
                resource_ptr = nullptr;
            }		
        }
    }

private:    
    manager* manager_ptr;
    T*       resource_ptr;
};
 
