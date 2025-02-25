#include<iostream>
#include<cassert>
#include<vector>
#include<ranges>
#include<range.h>


void test_range()
{
    std::vector<std::uint32_t> vec;
    for(std::uint32_t n=0; n!=50; ++n)
    {
        vec.push_back(rand()%100);
    }


    // ***************** //
    // *** std range *** //
    // ***************** //
    auto std_vec0 = vec | std::views::filter   ([](const auto& x) { return x%2==1; } )
                        | std::views::transform([](const auto& x) { return x*2;    } )
                        | std::views::take(10);

    auto std_vec1 = vec | std::views::filter   ([](const auto& x) { return x%2==1; } )
                        | std::views::transform([](const auto& x) { return x*2;    } );
    

    std::vector<std::uint32_t> std_vec2;
    for(const auto& x : vec | std::views::filter   ([](const auto& x) { return x%2==1; } )
                            | std::views::transform([](const auto& x) { return x*2;    } ))
    {
        std_vec2.push_back(x);
    }


    // ***************** //
    // *** alg range *** //
    // ***************** //
    // alg range does not support "auto"
    std::vector<std::uint32_t> alg_vec0 = vec | alg::filter   ([](const auto& x) { return x%2==1; } )
                                              | alg::transform([](const auto& x) { return x*2;    } ) 
                                              | alg::take(10); 

    std::vector<std::uint32_t> alg_vec1 = vec | alg::filter   ([](const auto& x) { return x%2==1; } )
                                              | alg::transform([](const auto& x) { return x*2;    } );

  
    std::vector<std::uint32_t> alg_vec2;
    for(const auto& x : vec | alg::filter   ([](const auto& x) { return x%2==1; } )
                            | alg::transform([](const auto& x) { return x*2;    } ))
    {
        alg_vec2.push_back(x);
    }
  

    // **************** //
    // *** checking *** //
    // **************** //
    std::cout << "\nvec      = "; for(const auto& x:vec)      std::cout << x << " ";
    std::cout << "\nstd_vec0 = "; for(const auto& x:std_vec0) std::cout << x << " ";
    std::cout << "\nalg_vec0 = "; for(const auto& x:alg_vec0) std::cout << x << " ";
    std::cout << "\nstd_vec1 = "; for(const auto& x:std_vec1) std::cout << x << " ";
    std::cout << "\nalg_vec1 = "; for(const auto& x:alg_vec1) std::cout << x << " ";
    std::cout << "\nstd_vec2 = "; for(const auto& x:std_vec2) std::cout << x << " ";
    std::cout << "\nalg_vec2 = "; for(const auto& x:alg_vec2) std::cout << x << " ";
}


