#include<iostream>
#include<cassert>
#include<vector>
#include<list>
#include<sstream>
#include<utility.h>


namespace alg
{
    std::vector<std::string> allocator_invoked;

    template<typename T> 
    class custom_allocator
    {
    public: 
        // STL requires the following typedef
        typedef size_t    size_type;
        typedef ptrdiff_t difference_type;
        typedef T*        pointer;
        typedef const T*  const_pointer;
        typedef T&        reference;
        typedef const T&  const_reference;
        typedef T         value_type;

    public:
        T* allocate(size_t n, const void* hints=0) // n = num of T-instances (not bytes)
        {
            std::stringstream ss;
            ss << "custom allocate " << n << " objects.";
            allocator_invoked.push_back(ss.str());

            T* p = new T[n];
            return p;
        }

        void deallocate(void* p, size_t n)
        {
            std::stringstream ss;
            ss << "custom deallocate " << n << " objects.";
            allocator_invoked.push_back(ss.str());

            delete[] reinterpret_cast<T*>(p);
        }
    };
}


// ******************************************* //
// *** Test custom allocator for container *** //
// ******************************************* //
void test_custom_allocator()
{
    struct A
    {
        std::uint64_t m_x;
        std::uint64_t m_y;
        std::uint64_t m_z;
    };

    {
        std::vector<A, alg::custom_allocator<A>> vec;
        std::list  <A, alg::custom_allocator<A>> list;

        for(int n=0; n!=100; ++n) vec.push_back(A{});
        for(int n=0; n!=5;   ++n) vec.pop_back();
        for(int n=0; n!=5;   ++n) list.push_back(A{});
    }

    std::uint32_t n = 0;
    assert(alg::allocator_invoked[n] == "custom allocate 1 objects.");        ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 2 objects.");        ++n; // <--- std::vector   allocates first
    assert(alg::allocator_invoked[n] == "custom deallocate 1 objects.");      ++n; // <--- std::vector deallocates after
    assert(alg::allocator_invoked[n] == "custom allocate 4 objects.");        ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 2 objects.");      ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 8 objects.");        ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 4 objects.");      ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 16 objects.");       ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 8 objects.");      ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 32 objects.");       ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 16 objects.");     ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 64 objects.");       ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 32 objects.");     ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 128 objects.");      ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 64 objects.");     ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 1 objects.");        ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 1 objects.");        ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 1 objects.");        ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 1 objects.");        ++n;
    assert(alg::allocator_invoked[n] == "custom allocate 1 objects.");        ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 1 objects.");      ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 1 objects.");      ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 1 objects.");      ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 1 objects.");      ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 1 objects.");      ++n;
    assert(alg::allocator_invoked[n] == "custom deallocate 128 objects.");     ++n;
    print_summary("custom allocator for container", "succeeded");
}

