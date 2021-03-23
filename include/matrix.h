#include<iostream>
#include<array>
#include<time.h>

// ****************************************************** //
// For all input  : use const const_matrix_view<T>&
// For all output : use             matrix_view<T>  
//
// Why? In order to bind to rvalue, we need either :
// const MAT<T>& or
//       MAT<T>
//
// Besides, we need const_matrix_view to ensure const T*.
// ****************************************************** //
template<typename T>
struct const_matrix_view
{
    const T& operator()(std::uint32_t y, std::uint32_t x) const
    {
        return ptr[y * linestep + x];
    }

    const_matrix_view<T> cview(std::uint32_t off_y,
                               std::uint32_t off_x,
                               std::uint32_t op_y,
                               std::uint32_t op_x) const
    {
        return const_matrix_view<T>{ ptr + off_y * linestep + off_x, op_y, op_x, linestep };
    }

    const T* ptr;
    const std::uint32_t size_y;
    const std::uint32_t size_x;
    const std::uint32_t linestep;
};

template<typename T>
struct matrix_view
{
    const T& operator()(std::uint32_t y, std::uint32_t x) const
    {
        return ptr[y * linestep + x];
    }

    T& operator()(std::uint32_t y, std::uint32_t x)
    {
        return ptr[y * linestep + x];
    }

    const_matrix_view<T> cview(std::uint32_t off_y,
                               std::uint32_t off_x,
                               std::uint32_t op_y,
                               std::uint32_t op_x) const
    {
        return const_matrix_view<T>{ ptr + off_y * linestep + off_x, op_y, op_x, linestep };
    }

    matrix_view<T> view(std::uint32_t off_y,
                        std::uint32_t off_x,
                        std::uint32_t op_y,
                        std::uint32_t op_x)
    {
        return matrix_view<T>{ ptr + off_y * linestep + off_x, op_y, op_x, linestep };
    }

    T* ptr;
    const std::uint32_t size_y;
    const std::uint32_t size_x;
    const std::uint32_t linestep;
};

template<typename T, std::uint32_t NY, std::uint32_t NX>
struct matrix
{
public:
    using type = T;
    static constexpr std::uint32_t szy = NY;
    static constexpr std::uint32_t szx = NX;

    matrix() : impl{}{}
    matrix(const std::array<T, NY*NX>& impl_) : impl(impl_) {}
    matrix(bool randomise)
    {
        if (randomise)
        {
            for(auto& x:impl)  x = (rand()%100 / 50.0 - 1.0);
        }
        else
        {
            for(std::uint32_t n=0; n!=impl.size(); ++n)  impl[n] = (n / (impl.size()/2.0) - 1.0);
        }
    }

public:
    const_matrix_view<T> cview(std::uint32_t off_y,
                               std::uint32_t off_x,
                               std::uint32_t op_y,
                               std::uint32_t op_x) const
    {
        return const_matrix_view<T>{ &impl[off_y * NX + off_x], op_y, op_x, NX };
    }

    const_matrix_view<T> cview() const
    {
        return const_matrix_view<T>{ &impl[0], NY, NX, NX };
    }

    matrix_view<T> view(std::uint32_t off_y,
                        std::uint32_t off_x,
                        std::uint32_t op_y,
                        std::uint32_t op_x)
    {
        return matrix_view<T>{ &impl[off_y * NX + off_x], op_y, op_x, NX };
    }

    matrix_view<T> view()
    {
        return matrix_view<T>{ &impl[0], NY, NX, NX };
    }

public:
    std::array<T, NY*NX> impl;
};

// *********************** //
// *** Basic operation *** //
// *********************** //
template<typename T>
bool is_same(const const_matrix_view<T>& lhs,
             const const_matrix_view<T>& rhs, T tol)
{
    if (lhs.size_y != rhs.size_y) return false;
    if (lhs.size_x != rhs.size_x) return false;
    std::uint32_t error_count = 0;

    const T* ptr0 = lhs.ptr;
    const T* ptr1 = rhs.ptr;

    for(std::uint32_t y=0; y!=lhs.size_y; ++y)
    {   for(std::uint32_t x=0; x!=lhs.size_x; ++x)
        {
            auto diff = ptr1[x] - ptr0[x];
            if (diff > tol || diff < -tol) ++error_count;
        }
        ptr0 += lhs.linestep;
        ptr1 += rhs.linestep;
    }
    return (error_count == 0);
}

template<typename T>
void print(const const_matrix_view<T>& mat)
{
    const T* ptr = mat.ptr;
    for(std::uint32_t y=0; y!=mat.size_y; ++y)
    {   for(std::uint32_t x=0; x!=mat.size_x; ++x)
        {
            std::cout << ptr[x] << " ";
        }
        ptr += mat.linestep;
        std::cout << "\n"; 
    }
}

template<typename T>
void set(matrix_view<T> A, const T& value)
{
    T* ptr = A.ptr;
    for(std::uint32_t y=0; y!=A.size_y; ++y)
    {   for(std::uint32_t x=0; x!=A.size_x; ++x)
        {
            ptr[x] = value;
        }
        ptr += A.linestep;
    }
}

template<typename T>
bool ATB_add_4x4(const const_matrix_view<T>& lhs,
                 const const_matrix_view<T>& rhs,
                             matrix_view<T>  ans)
{
    if (lhs.size_x != rhs.size_x) return false;
    if (lhs.size_y != 4 || rhs.size_y !=4) return false;
    if (ans.size_y != 4 || ans.size_x !=4) return false;

    const T* ptr0 = lhs.ptr;
          T* ptr  = ans.ptr;
    for(std::uint32_t y=0; y!=4; ++y)
    {
        const T* ptr1 = rhs.ptr;
        for(std::uint32_t x=0; x!=4; ++x)
        {
            for(std::uint32_t z=0; z!=lhs.size_x; ++z)
            {
                ptr[x] += ptr0[z] * ptr1[z];
            }
            ptr1 += rhs.linestep;
        }
        ptr0 += lhs.linestep;
        ptr  += ans.linestep;
    }
    return true;
}

// ********************* //
// *** Inner product *** //
// ********************* //
template<typename T> 
bool inner_product(const const_matrix_view<T>& A, matrix_view<T> ATA)
{
    if (ATA.size_y != A.size_y) return false;
    if (ATA.size_x != A.size_y) return false;

    for(std::uint32_t y=0; y!=A.size_y; ++y)
    {
        const T* ptr0 = &A(y,0);
        for(std::uint32_t x=0; x!=A.size_y; ++x)
        {
            const T* ptr1 = &A(x,0); T temp = 0;
            for(std::uint32_t z=0; z!=A.size_x; ++z)
            {
                temp += ptr0[z] * ptr1[z];
            }
            ATA(y,x) = temp;
        }
    }
    return true;
}
  
bool inner_product_4x4(const const_matrix_view<float>& A, matrix_view<float> ATA)
{
    if (A.size_y % 4 != 0)      return false;
    if (A.size_x % 4 != 0)      return false;
    if (ATA.size_y != A.size_y) return false;
    if (ATA.size_x != A.size_y) return false;

    for(std::uint32_t y=0; y!=A.size_y; y+=4)
    {
        for(std::uint32_t x=0; x!=A.size_y; x+=4)
        {
            matrix_view<float> ans = ATA.view(y,x,4,4);
            set(ans, .0f);

            ATB_add_4x4(A.cview(y,0,4,A.size_x), A.cview(x,1,4,A.size_x), ans);
        }
    }
    return true;
}
  
