#include<iostream>
#include<limits>
#include<vector>
#include<map>
#include<02_dynprog.h>

std::uint64_t nCr(std::uint16_t n, std::uint16_t r)
{
    if (n==1) return 1;
    if (r==0) return 1; // LHS of Pascal triangle
    if (r==n) return 1; // RHS of Pascal triangle
    return nCr(n-1,r) + nCr(n-1,r-1);
}

std::uint64_t bell_number(std::uint16_t n)
{
    if (n==0) return 1;
    if (n==1) return 1;

    std::uint64_t ans = 0;
    for(std::uint16_t r=0; r<=n-1; ++r)
    {
        ans += nCr(n-1,r) * bell_number(n-1-r); // pick n to form a cluster with the latest item
    }
    return ans;
}

// *************************** //
// *** Boolean parenthesis *** //
// *************************** //
enum class logic : std::uint8_t 
{
    OR,
    XOR,
    AND
};

struct bool_logic
{
    bool value;
    logic op;
};

auto merge_bool_parenthesis_subproblems(const logic& op, const auto& sub0, const auto& sub1)
{
    std::uint32_t num_true;
    std::uint32_t num_false;
    if (op == logic::OR)
    {
        num_true  = sub0.first * sub1.first + sub0.first * sub1.second + sub0.second * sub1.first; 
        num_false = sub0.second * sub1.second;
    }
    else if (op == logic::XOR)
    {
        num_true  = sub0.first * sub1.second + sub0.second * sub1.first; 
        num_false = sub0.first * sub1.first + sub0.second * sub1.second;
    }
    else
    {
        num_true  = sub0.first * sub1.first;
        num_false = sub0.first * sub1.second + sub0.second * sub1.first + sub0.second * sub1.second;
    }
    return std::make_pair(num_true, num_false);
}

// Expression is represented (ignores vec.back().op)
//
// = vec[0].value vec[0].op vec[1].value vec[1].op ... vec[n-2].value vec[n-2].op vec[n-1].value
//
std::pair<std::uint32_t, std::uint32_t> 
num_bool_parenthesis_recursive(const std::vector<bool_logic>::const_iterator& begin,
                               const std::vector<bool_logic>::const_iterator& end)
{
    if (begin+1==end)
    {
        if (begin->value) return {1,0};
        else              return {0,1};
    } 

    std::uint32_t num_true  = 0;
    std::uint32_t num_false = 0;
    for(auto iter=begin; iter!=end-1; ++iter)
    {
        auto sub0 = num_bool_parenthesis_recursive(begin, iter+1);
        auto sub1 = num_bool_parenthesis_recursive(iter+1, end);
        auto sub  = merge_bool_parenthesis_subproblems(iter->op, sub0, sub1);
        num_true  += sub.first;
        num_false += sub.second;
    }
    return {num_true, num_false};
} 

std::pair<std::uint32_t, std::uint32_t> 
num_bool_parenthesis_iterative(const std::vector<bool_logic>::const_iterator& begin,
                               const std::vector<bool_logic>::const_iterator& end)
{
    std::uint32_t N = std::distance(begin, end);
    matrix<std::pair<std::uint32_t, std::uint32_t>> mat(N,N); // #true and #false

    std::uint32_t k=0; 
    for(auto iter=begin; iter!=end; ++iter, ++k) 
    {
        mat(k,k) = (iter->value? std::pair<std::uint32_t, std::uint32_t>{1,0}: 
                                 std::pair<std::uint32_t, std::uint32_t>{0,1});
    }

    // Iterate through each sub-diagonal 
    for(std::uint32_t subdiag=1; subdiag!=N; ++subdiag)
    {
        for(std::uint32_t n=0; n!=N-subdiag; ++n)
        {
            std::uint32_t m=n+subdiag;
            mat(n,m).first  = 0;
            mat(n,m).second = 0;
            for(std::uint32_t k=n; k!=m; ++k)
            {
                auto sub = merge_bool_parenthesis_subproblems((begin+k)->op, mat(n,k), mat(k+1,m));
                mat(n,m).first  += sub.first;
                mat(n,m).second += sub.second;
            }
        }
    }
    return mat(0,N-1);
}

// ***************** //
// *** Coin game *** //
// ***************** //
std::uint32_t merge_coin_game_subproblem(std::uint32_t value_front, 
                                         std::uint32_t value_back, 
                                         std::uint32_t sub0, // two front coins are popped
                                         std::uint32_t sub1, // one front coin and one back coin are popped
                                         std::uint32_t sub2) // two back  coins are popped
{
    return std::max
    (
        value_front + std::min(sub0, sub1),
        value_back  + std::min(sub1, sub2)
    );
}

std::uint32_t coin_game_recursive(const std::vector<std::uint32_t>::const_iterator& begin, 
                                  const std::vector<std::uint32_t>::const_iterator& end) 
{
    if (begin+1==end) // diagonal
    {
        return *begin;
    }
    else if (begin+2==end) // first subdiagonal
    {
        return std::max(*begin, *(begin+1));
    }

    auto sub0 = coin_game_recursive(begin+2, end);   
    auto sub1 = coin_game_recursive(begin+1, end-1); 
    auto sub2 = coin_game_recursive(begin,   end-2); 
    return merge_coin_game_subproblem(*begin, *(end-1), sub0, sub1, sub2);
}

std::uint32_t coin_game_iterative(const std::vector<std::uint32_t>::const_iterator& begin, 
                                  const std::vector<std::uint32_t>::const_iterator& end) 
{
    std::uint32_t N = std::distance(begin, end);
    matrix<std::uint32_t> mat(N,N); 

    std::uint32_t k=0; 
    for(auto iter=begin; iter!=end; ++iter, ++k) mat(k,k) = *iter; // diagonal
    if (N>=2)
    {
        k=0;
        for(auto iter=begin; iter!=end-1; ++iter, ++k) mat(k,k+1) = std::max(*iter,*(iter+1)); // first diaogonal
    }
    
    // Iterate through each sub-diagonal 
    for(std::uint32_t subdiag=2; subdiag!=N; ++subdiag)
    {
        for(std::uint32_t n=0; n!=N-subdiag; ++n)
        {
            std::uint32_t m=n+subdiag;
            mat(n,m) = merge_coin_game_subproblem(*(begin+n), *(begin+m), mat(n+2,m), mat(n+1,m-1), mat(n,m-2));
        }
    }
    return mat(0,N-1);
}

// *************************************** //
// *** Two persons traversal of cities *** //
// *************************************** //
// optimum f(N) = min(g(0,N-1), g(1,N-1), g(2,N-1), ..., g(N-2,N-1))
//           |                              | |
//           +--- number of cities          | +--- last city visited by B
//                                          +----- last city visited by A, and city beyond this point are traversed by B
//
// given 0 <= n < m < N :
// g(n,m) = g(n.m-1) + d(m-1,m)                                              <--- case 1 : if n < m-1
// g(n,m) = min(g(0,n) + dist(0,m),            when B's previous city is 0   <--- case 2 : if n== m-1 (i.e. first subdiagonal)
//              g(1,n) + dist(1,m),            when B's previous city is 1
//              ...
//              g(n-1,n) + dist(n-1,m),        when B's previous city is n-1
//              dist(0,1) + ... + dist(n-1,n)  when B has no previous city
//
// f's input is a number
// g's input is an index
//
// ******************** //
// *** Bitonic tour *** //
// ******************** //
// outbound = from RHS to LHS 
//  inbound = from LHS to RHS
//
//           +--- number of cities
//           |
// optimum f(N) = min(g(0,N-1) + dist(0,N-1),
//                    g(1,N-1) + dist(1,N-1),
//                    g(2,N-1) + dist(2,N-1),
//                    ...,
//                    g(N-2,N-1)) + dist(N-2,N-1))
//                      |   |
//                      |   +-- first city in outbound trip
//                      +------  last city in  inbound trip, and city beyond this point are traversed in outbound trip
//
// given 0 <= n < m < N :
// g(n,m) = g(n.m-1) + d(m-1,m)                                              <--- case 1 : if n < m-1
// g(n,m) = min(g(0,n) + dist(0,m),            when 2nd outbound city is 0   <--- case 2 : if n== m-1 (i.e. first subdiagonal)
//              g(1,n) + dist(1,m),            when 2nd outbound city is 1
//              ...
//              g(n-1,n) + dist(n-1,m),        when 2nd outbound city is n-1
//              dist(0,1) + ... + dist(n-1,n)  when B has no previous stop
//
// *********************************** //
// *** Piecewise linear regression *** //
// *********************************** //
// The original problem is defined as f(N,M) with N & M as number.
// Unlike the above question, there is no need to convert number in f(N) into index in g(n).
//
// f(N,M) = regression error for a set of N points with M segments
// e(n)   = sum of error for point[n], points[n+1], ... points[N-1] using line joining point[n] and point[N-1]
//
// optimum f(N,M) = min(f(N-1,M-1) + e(N-2),
//                      f(N-2,M-1) + e(N-3),
//                      ...,
//                      f(M  ,M-1) + e(M-1))

struct coord
{
    double x;
    double y;
};

double sum_error_square(const std::vector<coord>::const_iterator& begin, 
                        const std::vector<coord>::const_iterator& end) // coord must be sorted in x
{
    double m = ((end-1)->y - begin->y) / ((end-1)->x - begin->x);
    double c = begin->y - m * begin->x;
    double err = 0;
    for(auto iter=begin; iter!=end; ++iter)
    {
        double temp = (m * iter->x + c) - iter->y;
        err += temp*temp;
    }
    return err;
}

double piecewise_linear_regresson(const std::vector<coord>::const_iterator& begin, 
                                  const std::vector<coord>::const_iterator& end, // coord must be sorted in x
                                  std::uint32_t num_of_segments)                 // i.e. M
{
    std::uint32_t num_pt  = std::distance(begin, end);
    std::uint32_t num_seg = num_of_segments;
    matrix<double> f(num_pt+1, num_seg+1); 

    // starts from N=2, M=1
    for(std::uint32_t N=2; N<=num_pt; ++N)
    {
        f(N,1) = sum_error_square(begin, begin+N);
    }

    // starts from M=2
    for(std::uint32_t M=2; M<=num_seg; ++M) 
    {
        for(std::uint32_t N=M+1; N<=num_pt; ++N)
        {
            f(N,M) = std::numeric_limits<double>::max();
            for(std::uint32_t K=M; K<=num_pt-1; ++K) // K = num of points in previous subproblem
            {
                auto temp = f(K,M-1) + sum_error_square(begin+(K-1), end);
                if (f(N,M) < temp)
                    f(N,M) = temp;
            }
        }
    }
    return f(num_pt,num_seg);
}

// ******************** //
// *** Box stacking *** //
// ******************** //
// Experiment finds that iterative approach is much faster than recursive approach.

struct box
{
    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t z;
};

// Constraint : the order of box in vector cannot be changed in stacking.
std::uint32_t box_stacking_recursive(const std::vector<box>::const_iterator& begin, 
                                     const std::vector<box>::const_iterator& end,
                                     std::uint32_t base_short = 0,
                                     std::uint32_t base_long  = 0)
{
    if (begin==end) return 0;
    std::uint32_t ans = box_stacking_recursive(begin+1, end, base_short, base_long);

    if (std::min(begin->x, begin->y) >= base_short &&
        std::max(begin->x, begin->y) >= base_long)
    {
        auto height = box_stacking_recursive(begin+1, end, std::min(begin->x, begin->y), std::max(begin->x, begin->y)) + begin->z;
        if (ans < height) ans = height;
    }
    if (std::min(begin->y, begin->z) >= base_short &&
        std::max(begin->y, begin->z) >= base_long)
    {
        auto height = box_stacking_recursive(begin+1, end, std::min(begin->y, begin->z), std::max(begin->y, begin->z)) + begin->x;
        if (ans < height) ans = height;
    }
    if (std::min(begin->z, begin->x) >= base_short &&
        std::max(begin->z, begin->x) >= base_long)
    {
        auto height = box_stacking_recursive(begin+1, end, std::min(begin->z, begin->x), std::max(begin->z, begin->x)) + begin->y;
        if (ans < height) ans = height;
    }
    return ans;
}

// This approach is like time-state diagram approach.
struct base
{
    std::uint32_t base_short;
    std::uint32_t base_long;
};

bool operator<(const base& lhs, const base& rhs)
{
    if (lhs.base_short < rhs.base_short) return  true;
    if (lhs.base_short > rhs.base_short) return false;
    if (lhs.base_long  < rhs.base_long)  return  true;
    return false;
}

// map[base] = total_height
void euler_update(std::map<base, std::uint32_t>& status, std::uint32_t base0, std::uint32_t base1, std::uint32_t height)
{
    std::uint32_t base_min = std::min(base0, base1);
    std::uint32_t base_max = std::max(base0, base1);
    if (auto iter=status.find(base{base_min, base_max}); iter!=status.end())
    {
        if (iter->second < height)
            iter->second = height;
    }
    else
    {
        status[base{base_min, base_max}] = height;
    }
}

std::uint32_t box_stacking_iterative(const std::vector<box>::const_iterator& begin, 
                                     const std::vector<box>::const_iterator& end,
                                     std::uint32_t base_short = 0,
                                     std::uint32_t base_long  = 0)
{
    std::map<base, std::uint32_t> status;
    for(auto iter=begin; iter!=end; ++iter) // iterate in time domain
    {
        std::map<base, std::uint32_t> status0;
        euler_update(status0, iter->x, iter->y, iter->z); // Todo : move this part to the beginning with zero base and zero height, more readable 
        euler_update(status0, iter->y, iter->z, iter->x);
        euler_update(status0, iter->z, iter->x, iter->y);
        for(const auto& info:status) // iterate in state domain
        {
            auto new_base_short = std::min(iter->x, iter->y);
            auto new_base_long  = std::max(iter->x, iter->y);
            if (new_base_short >= info.first.base_short && 
                new_base_long  >= info.first.base_long) 
            {
                euler_update(status0, new_base_short, new_base_long, info.second + iter->z);
            }
            new_base_short = std::min(iter->y, iter->z);
            new_base_long  = std::max(iter->y, iter->z);
            if (new_base_short >= info.first.base_short && 
                new_base_long  >= info.first.base_long) 
            {
                euler_update(status0, new_base_short, new_base_long, info.second + iter->x);
            }
            new_base_short = std::min(iter->z, iter->x);
            new_base_long  = std::max(iter->z, iter->x);
            if (new_base_short >= info.first.base_short && 
                new_base_long  >= info.first.base_long) 
            {
                euler_update(status0, new_base_short, new_base_long, info.second + iter->y);
            }

            // BUG : Dont forget this case
            status0.insert(info);
        }
        status = std::move(status0);
    }

    // Find max height 
    std::uint32_t ans = 0;
    for(const auto& info:status)
    {
        if (ans < info.second) ans = info.second;
    }
    return ans;
}

// ******************* //
// *** Bin packing *** //
// ******************* //
std::uint32_t bin_packing(std::uint32_t numA, 
                          std::uint32_t numB, 
                          std::uint32_t numC,
                          std::uint32_t sizeA,
                          std::uint32_t sizeB,
                          std::uint32_t sizeC,
                          std::uint32_t capacity)
{
    if (numA*sizeA + numB*sizeB + numC*sizeC <= capacity) return 1;

    std::uint32_t ans = std::numeric_limits<std::uint32_t>::max(); 
    for(std::uint32_t nA=0; nA!=numA; ++nA)
    {
        for(std::uint32_t nB=0; nB!=numB; ++nB)
        {
            for(std::uint32_t nC=0; nC!=numC; ++nC)
            {
                auto temp = bin_packing(     nA,      nB,      nC, sizeA, sizeB, sizeC, capacity) + 
                            bin_packing(numA-nA, numB-nB, numC-nC, sizeA, sizeB, sizeC, capacity);
                if (ans > temp); 
                    ans = temp;
            }
        }
    }
    return ans;
}
    
// ************ //
// *** Test *** //
// ************ //
std::vector<bool_logic> gen_bool_logic_vector(std::uint32_t N)
{
    // vec.back().op is redundant
    std::vector<bool_logic> vec;

    for(std::uint32_t n=0; n!=N; ++n)
    {
        auto tmp = rand()%6;
        if      (tmp==0)   vec.push_back({ true, logic:: OR});
        else if (tmp==1)   vec.push_back({ true, logic::XOR});
        else if (tmp==2)   vec.push_back({ true, logic::AND});
        else if (tmp==3)   vec.push_back({false, logic:: OR});
        else if (tmp==4)   vec.push_back({false, logic::XOR});
        else               vec.push_back({false, logic::AND});
    } 
    return vec;
}

std::vector<std::uint32_t> gen_coin_game(std::uint32_t N)
{
    std::vector<std::uint32_t> vec;
    for(std::uint32_t n=0; n!=N; ++n)
    {
        vec.push_back(rand()%10+1);
    }
    return vec;
}

std::vector<box> gen_box_stack(std::uint32_t N)
{
    std::vector<box> vec;
    for(std::uint32_t n=0; n!=N; ++n)
    {
        vec.push_back(box{ 1+n+rand()%5, 1+n+rand()%5, 1+n+rand()%5 });
    }
    return vec;
}

void test_nCr()
{
    std::cout << "\nPascal triangle";
    for(std::uint16_t n=1; n!=10; ++n)
    {
        std::cout << "\n";
        for(std::uint16_t r=0; r<=n; ++r)
        {
            std::cout << nCr(n,r) << " ";
        }
    }
}

void test_bell_number()
{
    for(std::uint16_t n=0; n!=10; ++n)
    {
        std::cout << "\nbell number " << n << " : " << bell_number(n);
    }
}

void test_num_bool_parenthesis()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        auto vec = gen_bool_logic_vector(rand()%15+2); 
        auto ans0 = num_bool_parenthesis_recursive(vec.begin(), vec.end());
        auto ans1 = num_bool_parenthesis_iterative(vec.begin(), vec.end());

        std::cout << "\ninput = [";
        for(const auto& x:vec) 
        {
            if (&x!=&vec.back())
            {
                std::cout << (x.value? "T":"F") << " " 
                          << (x.op==logic::OR? "or" :
                             (x.op==logic::XOR? "xor" : "and")) << " ";
            }
            else
            {
                std::cout << (x.value? "T":"F") << "]";
            }
        }
        std::cout << "\nans0  = " << ans0.first << " + " << ans0.second;
        std::cout << "\nans1  = " << ans1.first << " + " << ans1.second; 
        if (ans0.first  != ans1.first ||
            ans0.second != ans1.second) std::cout << " [error]";
    }
}

void test_coin_game()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        auto vec = gen_coin_game(rand()%15+2); 
        auto ans0 = coin_game_recursive(vec.begin(), vec.end());
        auto ans1 = coin_game_iterative(vec.begin(), vec.end());

        std::cout << "\ninput = ";
        for(const auto& x:vec)
        {
            std::cout << x << " ";
        }   
        std::cout << "\nans0  = " << ans0;
        std::cout << "\nans1  = " << ans1; 
        if (ans0 != ans1) std::cout << " [error]";
    }
}

void test_box_stacking()
{
    for(std::uint32_t n=0; n!=100; ++n)
    {
        auto vec = gen_box_stack(13);
        auto ans0 = box_stacking_recursive(vec.begin(), vec.end());
        auto ans1 = box_stacking_iterative(vec.begin(), vec.end());

        std::cout << "\ninput = ";
        for(const auto& box:vec)
        {
            std::cout << "[" << box.x << "/" << box.y << "/" << box.z << "] ";
        }   
        std::cout << "\nans0  = " << ans0;
        std::cout << "\nans1  = " << ans1;
        if (ans0 != ans1) std::cout << " [error]";
    }
}

void test_bin_packing()
{
}

void test_multi_link_subproblem()
{
//  test_nCr();
//  test_bell_number();
    test_num_bool_parenthesis();
//  test_coin_game();
//  test_box_stacking();
//  test_bin_packing();
}
