#include<iostream>
#include<array>
#include<time.h>

template<typename T>
struct const_matrix_view
{
    const T& operator()(std::uint32_t y, std::uint32_t x) const
    {
        return ptr[y * linestep + x];
    }

    const_matrix_view<T> view(std::uint32_t off_y,
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
:Wq
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

    const_matrix_view<T> view(std::uint32_t off_y,
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
    const_matrix_view<T> view(std::uint32_t off_y,
                              std::uint32_t off_x,
                              std::uint32_t op_y,
                              std::uint32_t op_x) const
    {
        return const_matrix_view<T>{ &impl[off_y * NX + off_x], op_y, op_x, NX };
    }

    const_matrix_view<T> view() const
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
void set(matrix_view<T>& A, const T& value)
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
bool ATB_add_4x4(const matrix_view<T>& lhs,
                 const matrix_view<T>& rhs,
                       matrix_view<T>& ans)
{
    if (lhs.size_x != rhs.size_x) return false;
    if (lhs.size_y != 4 || rhs.size_y !=4) return false;
    if (ans.size_y != 4 || ans.size_x !=4) return false;

    T* ptr0 = lhs.ptr;
    T* ptr1 = rhs.ptr;
    for(std::uint32_t y=0; y!=lhs.size_y; ++y)
    {   for(std::uint32_t x=0; x!=lhs.size_x; ++x)
        {
            ptr0[x] += ptr1[x];
        }
        ptr0 += lhs.linestep;
        ptr1 += rhs.linestep;
    }
    return true;
}

// ********************* //
// *** Inner product *** //
// ********************* //
template<typename T>
bool inner_product(const matrix_view<T>& A, matrix_view<T> ATA)
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

bool inner_product_4x4(const matrix_view<float>& A, matrix_view<float> ATA)
{
    if (A.size_y % 4 != 0)      return false;
    if (A.size_x % 4 != 0)      return false;
    if (ATA.size_y != A.size_y) return false;
    if (ATA.size_x != A.size_y) return false;
    matrix<float,4,4> TMP;

    for(std::uint32_t y=0; y!=A.size_y; y+=4)
    {
        for(std::uint32_t x=0; x!=A.size_y; x+=4)
        {
            matrix_view<float> ans = ATA.view(y,x,4,4);
            set(ans,(float)0);

            for(std::uint32_t z=0; z!=A.size_x; z+=4)
            {
                ATB_add_4x4(A.view(y,z,4,4), A.view(x,z,4,4), ans);
            }
        }
    }
    return true;
}

// ******************* //
// *** Accessaries *** //
// ******************* //
template<typename T>
bool is_same(const matrix_view<T>& lhs,
             const matrix_view<T>& rhs, T tol)
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
void print(const matrix_view<T>& mat)
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

// ************ //
// *** Test *** //
// ************ /
std::uint64_t timediff(const timespec& ts0, const timespec& ts1)
{
    std::uint32_t t0 = ts0.tv_sec * 1e9 + ts0.tv_nsec;
    std::uint32_t t1 = ts1.tv_sec * 1e9 + ts1.tv_nsec;
    return t1-t0;
}

void test_simd0()
{
    matrix<float,16,8> A;
    matrix<float,16,8> B(true);
    matrix<float,16,8> C(false);

    matrix<float,16,16> BTB;
    matrix<float,16,16> CTC;
    inner_product(B.view(), BTB.view());
    inner_product(C.view(), CTC.view());

    std::cout << "\nA   = \n"; print(A.view());
    std::cout << "\nB   = \n"; print(B.view());
    std::cout << "\nC   = \n"; print(C.view());
    std::cout << "\nBTB = \n"; print(BTB.view());
    std::cout << "\nCTC = \n"; print(CTC.view());
}

void test_simd1()
{
    matrix<float,64,256> A(true);
    matrix<float,64,64> ATA0;
    matrix<float,64,64> ATA1;
    matrix<float,64,64> ATA2;

    for(std::uint32_t n=0; n!=100; ++n)
    {
        timespec ts0;
        timespec ts1;
        timespec ts2;
        timespec ts3;

        clock_gettime(CLOCK_MONOTONIC, &ts0);
        inner_product(A.view(), ATA0.view());
        clock_gettime(CLOCK_MONOTONIC, &ts1);
        inner_product_4x4(A.view(), ATA1.view());
        clock_gettime(CLOCK_MONOTONIC, &ts2);

        std::cout << "\nis_same = " << is_same(ATA0.view(), ATA1.view(), (float)0.00001)
        //        << ", is_same = " << is_same(ATA0.view(), ATA2.view(), (float)0.00001)
                  << ", time0 = "   << timediff(ts0,ts1)
                  << ", time1 = "   << timediff(ts1,ts2);
        //        << ", time2 = "   << timediff(ts2,ts3);
    }
}
