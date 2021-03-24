#include<iostream>
#include<array>
#include<time.h>
#include<nmmintrin.h>
#include<immintrin.h>

// ********************************************************* //
// [Remark 1]
// For all input  : use const const_matrix_view<T>&
// For all output : use             matrix_view<T>
//
// Why? In order to bind to rvalue, we need either :
// const MAT<T>& or
//       MAT<T>
//
// Besides, we need const_matrix_view to ensure const T*.
// ********************************************************* //
// [Remark 2]
// Although ATA is symmetric, we do full matrix calculation
// in all implementations, to ensure fair speed comparison.
// ********************************************************* //
// [Remark 3]
// Implementations
// 1. ordinary version
// 2. cache friendly version (using 4x4 grid)
// 3. SIMD version           (using 4x4 grid + SIMD)
// ********************************************************* //
template<typename T>
struct const_matrix_view
{
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
    const_matrix_view<T> cview(std::uint32_t off_y,
                               std::uint32_t off_x,
                               std::uint32_t op_y,
                               std::uint32_t op_x) const
    {
        return const_matrix_view<T>{ ptr + off_y * linestep + off_x, op_y, op_x, linestep };
    }

    const_matrix_view<T> cview() const
    {
        return const_matrix_view<T>{ ptr, 0, 0, linestep };
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
    if (lhs.size_y != 4 || lhs.size_x !=4) return false;
    if (rhs.size_y != 4 || rhs.size_x !=4) return false;
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

bool ATB_add_4x4_SIMD(const const_matrix_view<float>& lhs,
                      const const_matrix_view<float>& rhs,
                                  matrix_view<float>  ans)
{
    if (lhs.size_y != 4 || lhs.size_x !=4) return false;
    if (rhs.size_y != 4 || rhs.size_x !=4) return false;
    if (ans.size_y != 4 || ans.size_x !=4) return false;

    alignas(64) __m128 a[4];
    alignas(64) __m128 b[4];
    alignas(64) __m128 c;

    alignas(64) const float* ptr0 = lhs.ptr;
    alignas(64) const float* ptr1 = rhs.ptr;
    for(std::uint32_t y=0; y!=4; ++y)
    {
    //  a[y] = _mm_set_ps(ptr0[0], ptr0[1], ptr0[2], ptr0[3]); // THIS IS SLOW.
    //  b[y] = _mm_set_ps(ptr1[0], ptr1[1], ptr1[2], ptr1[3]); // THIS IS SLOW.
        a[y] = _mm_load_ps(ptr0); // SLIGHTY FASTER
        b[y] = _mm_load_ps(ptr1); // SLIGHTY FASTER
        ptr0 += lhs.linestep;
        ptr1 += rhs.linestep;
    }

    alignas(64) float* ptr = ans.ptr;
    for(std::uint32_t y=0; y!=4; ++y)
    {
        for(std::uint32_t x=0; x!=4; ++x)
        {
            c = _mm_mul_ps(a[y],b[x]);
            float* p = reinterpret_cast<float*>(&c);
            ptr[x] += (p[0] + p[1] + p[2] + p[3]);
        }
        ptr += ans.linestep;
    }
    return true;
}
// ********************* //
// *** Inner product *** //
// ********************* //
template<typename T>
bool inner_product(const const_matrix_view<T>& A, matrix_view<T> ATA)
{
//  set(ATA, T{}); // Init here or ...
    if (ATA.size_y != A.size_y) return false;
    if (ATA.size_x != A.size_y) return false;

    const T* ptr0 = A.ptr;
          T* ptr  = ATA.ptr;
    for(std::uint32_t y=0; y!=A.size_y; ++y)
    {
        const T* ptr1 = A.ptr; // Init here or ...
        for(std::uint32_t x=0; x!=A.size_y; ++x)
        {
            T temp{};
            for(std::uint32_t z=0; z!=A.size_x; ++z)
            {
                temp += ptr0[z] * ptr1[z];
            }
            ptr[x] = temp;
            ptr1 += A.linestep;
        }
        ptr0 += A.linestep;
        ptr  += ATA.linestep;
    }
    return true;
}

template<bool SIMD>
bool inner_product_4x4(const const_matrix_view<float>& A, matrix_view<float> ATA)
{
    set(ATA, .0f);
    if (A.size_y % 4 != 0)      return false;
    if (A.size_x % 4 != 0)      return false;
    if (ATA.size_y != A.size_y) return false;
    if (ATA.size_x != A.size_y) return false;

    for(std::uint32_t y=0; y!=A.size_y; y+=4)
    {
        for(std::uint32_t x=0; x!=A.size_y; x+=4)
        {
            matrix_view<float> ans = ATA.view(y,x,4,4);
            for(std::uint32_t z=0; z!= A.size_x; z+=4)
            {
                if constexpr (!SIMD)
                {
                    ATB_add_4x4(A.cview(y,z,4,4), A.cview(x,z,4,4), ans);
                }
                else
                {
                    ATB_add_4x4_SIMD(A.cview(y,z,4,4), A.cview(x,z,4,4), ans);
                }
            }
        }
    }
    return true;
}