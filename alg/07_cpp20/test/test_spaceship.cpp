#include<iostream>
#include<cstdint>
#include<cassert>
#include<utility.h>


namespace test
{
    std::uint32_t invoke_count0  = 0;
    std::uint32_t invoke_count1  = 0;
    std::uint32_t invoke_countA0 = 0;
    std::uint32_t invoke_countA1 = 0;
    std::uint32_t invoke_countB0 = 0;
    std::uint32_t invoke_countB1 = 0;
    std::uint32_t invoke_countC0 = 0;
    std::uint32_t invoke_countC1 = 0;


    inline void debug()
    {
        std::cout << "\ndebug = ";
        std::cout << invoke_count0  << ", ";
        std::cout << invoke_count1  << ", ";
        std::cout << invoke_countA0 << ", ";
        std::cout << invoke_countA1 << ", ";
        std::cout << invoke_countB0 << ", ";
        std::cout << invoke_countB1 << ", ";
        std::cout << invoke_countC0 << ", ";
        std::cout << invoke_countC1;
    }

    struct sampleA
    {
        std::uint32_t m_x;
    };

    struct sampleB
    {
        std::uint32_t m_y;
    };

    struct sampleC
    {
        std::uint32_t m_z;
    };

    struct sample
    {
        inline bool operator==(const sample& rhs)
        {
            ++invoke_count0;
            return m_x == rhs.m_x && 
                   m_y == rhs.m_y &&
                   m_z == rhs.m_z; 
        }

        inline bool operator==(const sampleA& rhs)
        {
            ++invoke_countA0;
            return m_x == rhs.m_x;
        }

        inline bool operator==(const sampleB& rhs)
        {
            ++invoke_countB0;
            return m_y == rhs.m_y;
        }

        inline bool operator==(const sampleC& rhs)
        {
            ++invoke_countC0;
            return m_z == rhs.m_z;
        }

        // ******************************************************* //
        // *** Typical element-wise spaceship operator for POD *** //
        // ******************************************************* //
        // 1. spaceship operator returns ordering, does NOT return bool
        // 2. ordering cannot be static_cast into bool / int
        // 3. ordering can    be converted   into bool by comparison with zero
        //
        inline auto operator<=>(const sample& rhs)
        {
            ++invoke_count1;

            if (auto tmp = m_x <=> rhs.m_x; tmp != 0) return tmp;
            if (auto tmp = m_y <=> rhs.m_y; tmp != 0) return tmp;
            return         m_z <=> rhs.m_z;
        }

        inline auto operator<=>(const sampleA& rhs)
        {
            ++invoke_countA1;
            return m_x <=> rhs.m_x;
        }

        inline auto operator<=>(const sampleB& rhs)
        {
            ++invoke_countB1;
            return m_y <=> rhs.m_y;
        }
        
        inline auto operator<=>(const sampleC& rhs)
        {
            ++invoke_countC1;
            return m_z <=> rhs.m_z;
        }

        std::uint32_t m_x;
        std::uint32_t m_y;
        std::uint32_t m_z;
    };
}


void test_spaceship()
{
    test::sample   s{15,25,35};
    test::sampleA a0{14};
    test::sampleA a1{15};
    test::sampleA a2{16};
    test::sampleB b0{24};
    test::sampleB b1{25};
    test::sampleB b2{26};
    test::sampleC c0{34};
    test::sampleC c1{35};
    test::sampleC c2{36};

    // primary reverse
    std::cout << "\n" << (s == a0) << " " << (s == a1) << " " << (s == a2);   test::debug();
    std::cout << "\n" << (a0 == s) << " " << (a1 == s) << " " << (a2 == s);   test::debug();
    std::cout << "\n" << (s == b0) << " " << (s == b1) << " " << (s == b2);   test::debug();
    std::cout << "\n" << (b0 == s) << " " << (b1 == s) << " " << (b2 == s);   test::debug();
    std::cout << "\n" << (s == c0) << " " << (s == c1) << " " << (s == c2);   test::debug();
    std::cout << "\n" << (c0 == s) << " " << (c1 == s) << " " << (c2 == s);   test::debug();
     
    std::cout << "\n" << (a0 <=> s < 0) << " " << (a1 <=> s == 0) << " " << (a2 <=> s > 0);   test::debug();
    std::cout << "\n" << (b0 <=> s < 0) << " " << (b1 <=> s == 0) << " " << (b2 <=> s > 0);   test::debug();
    std::cout << "\n" << (c0 <=> s < 0) << " " << (c1 <=> s == 0) << " " << (c2 <=> s > 0);   test::debug();

    // secondary rewrite
    std::cout << "\n" << (s != a0) << " " << (s != a1) << " " << (s != a2);   test::debug();
    std::cout << "\n" << (a0 != s) << " " << (a1 != s) << " " << (a2 != s);   test::debug();
    std::cout << "\n" << (s != b0) << " " << (s != b1) << " " << (s != b2);   test::debug();
    std::cout << "\n" << (b0 != s) << " " << (b1 != s) << " " << (b2 != s);   test::debug();
    std::cout << "\n" << (s != c0) << " " << (s != c1) << " " << (s != c2);   test::debug();
    std::cout << "\n" << (c0 != s) << " " << (c1 != s) << " " << (c2 != s);   test::debug();

    std::cout << "\n" << (a0 <= s) << " " << (a1 <= s) << " " << (a2 >= s);   test::debug();
    std::cout << "\n" << (b0 <= s) << " " << (b1 <= s) << " " << (b2 >= s);   test::debug();
    std::cout << "\n" << (c0 <= s) << " " << (c1 <= s) << " " << (c2 >= s);   test::debug();
    print_summary("spaceship operator", "succeeded");
}
