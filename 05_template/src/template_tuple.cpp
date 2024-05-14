#include<iostream>
#include<vector>
#include<tuple.h>
#include<tuple2.h>
#include<assert.h> // for assert


// ************ //
// *** Test *** //
// ************ //
void test_size_and_element()
{
    using T0 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using T1 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>>;
    using T2 = std::tuple<char>;
    using T3 = std::tuple<>;
    using T4 = tuple_size<T0>::type;
    using T5 = tuple_size<T1>::type;
    using T6 = tuple_size<T2>::type;
    using T7 = tuple_size<T3>::type;

    static_assert(std::is_same_v<T4, std::integral_constant<std::size_t,5>>, "failed to tuple_size");
    static_assert(std::is_same_v<T5, std::integral_constant<std::size_t,4>>, "failed to tuple_size");
    static_assert(std::is_same_v<T6, std::integral_constant<std::size_t,1>>, "failed to tuple_size");
    static_assert(std::is_same_v<T7, std::integral_constant<std::size_t,0>>, "failed to tuple_size");

    using X0 = tuple_element<0,T0>::type;
    using X1 = tuple_element<1,T0>::type;
    using X2 = tuple_element<2,T0>::type;
    using X3 = tuple_element<3,T0>::type;
    using X4 = tuple_element<4,T0>::type;

    static_assert(std::is_same_v<X0, char>,                     "failed to tuple_element");
    static_assert(std::is_same_v<X1, std::uint32_t>,            "failed to tuple_element");
    static_assert(std::is_same_v<X2, std::string>,              "failed to tuple_element");
    static_assert(std::is_same_v<X3, std::pair<double,double>>, "failed to tuple_element");
    static_assert(std::is_same_v<X4, double>,                   "failed to tuple_element");

    using Y0 = tuple_element2<0,T0>::type;
    using Y1 = tuple_element2<1,T0>::type;
    using Y2 = tuple_element2<2,T0>::type;
    using Y3 = tuple_element2<3,T0>::type;
    using Y4 = tuple_element2<4,T0>::type;

    static_assert(std::is_same_v<Y0, char>,                     "failed to tuple_element");
    static_assert(std::is_same_v<Y1, std::uint32_t>,            "failed to tuple_element");
    static_assert(std::is_same_v<Y2, std::string>,              "failed to tuple_element");
    static_assert(std::is_same_v<Y3, std::pair<double,double>>, "failed to tuple_element");
    static_assert(std::is_same_v<Y4, double>,                   "failed to tuple_element");
}

void test_shuffle()
{
    using T   = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using X0  = shuffle_tuple <T,3,2,1>::type;
    using X1  = shuffle_tuple2<T,idx_seq<3,2,1>>::type;
    using ANS = std::tuple<std::pair<double,double>, std::string, std::uint32_t>;

    static_assert(std::is_same_v<X0, ANS>, "failed to shuffle_tuple");
    static_assert(std::is_same_v<X1, ANS>, "failed to shuffle_tuple");
    static_assert(std::is_same_v<std::tuple_size<T >::type, std::integral_constant<std::size_t,5>>, "failed to tuple_size");
    static_assert(std::is_same_v<std::tuple_size<X0>::type, std::integral_constant<std::size_t,3>>, "failed to tuple_size");
    static_assert(std::is_same_v<std::tuple_size<X1>::type, std::integral_constant<std::size_t,3>>, "failed to tuple_size");
    static_assert(std::tuple_size<T >::value == 5, "failed to tuple_size");
    static_assert(std::tuple_size<X0>::value == 3, "failed to tuple_size");
    static_assert(std::tuple_size<X1>::value == 3, "failed to tuple_size");

    // *** Factory *** //
    auto t  = T{'a', 12345, "wxyz", std::make_pair(3.1415, 1.4141), 0.98765};
    auto x2 = make_shuffle_tuple(t, idx_seq<3,2,1>{}); // remember to construct an object here

    static_assert(std::is_same_v<decltype(x2),ANS>, "failed to make_shuffle_tuple");
    assert(std::get<1>(t) == std::get<2>(x2));
    assert(std::get<2>(t) == std::get<1>(x2));
}

void test_push_front()
{
    using T    = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using X0   = push_front_tuple<T,  std::string, char>::type;
    using X1   = push_front_tuple<X0, std::string, char>::type;
    using X2   = push_front_tuple<std::tuple<>, std::string, char>::type;
    using ANS0 = std::tuple<char, std::string, char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using ANS1 = std::tuple<char, std::string, char, std::string, char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using ANS2 = std::tuple<char, std::string>;

    static_assert(std::is_same_v<X0, ANS0>, "failed to push_front_tuple");
    static_assert(std::is_same_v<X1, ANS1>, "failed to push_front_tuple");
    static_assert(std::is_same_v<X2, ANS2>, "failed to push_front_tuple");

    // *** Factory *** //
    auto  t    = T{'a', 12345, "wxyz", std::make_pair(3.1415, 1.4141), 0.98765};
    auto  x3   = make_push_front_tuple(t, std::string{"hello-world"});
    auto  x4   = make_push_front_tuple(t, std::string{"hello-world"}, 'A', 'B', 'C');
    auto  x5   = make_push_front_tuple(std::tuple<>{}, std::string{"hello-world"}, 'A');
    using ANS3 = std::tuple                  <std::string, char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using ANS4 = std::tuple<char, char, char, std::string, char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using ANS5 = std::tuple            <char, std::string>;

    static_assert(std::is_same_v<decltype(x3), ANS3>, "failed to make_push_front_tuple");
    static_assert(std::is_same_v<decltype(x4), ANS4>, "failed to make_push_front_tuple");
    static_assert(std::is_same_v<decltype(x5), ANS5>, "failed to make_push_front_tuple");
}

void test_push_back()
{
    using T    = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using X0   = push_back_tuple<T,  std::string, char>::type;
    using X1   = push_back_tuple<X0, std::string, char>::type;
    using X2   = push_back_tuple<std::tuple<>, std::string, char>::type;
    using ANS0 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double, std::string, char>;
    using ANS1 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double, std::string, char, std::string, char>;
    using ANS2 = std::tuple<std::string, char>;

    static_assert(std::is_same_v<X0, ANS0>, "failed to push_back_tuple");
    static_assert(std::is_same_v<X1, ANS1>, "failed to push_back_tuple");
    static_assert(std::is_same_v<X2, ANS2>, "failed to push_back_tuple");

    // *** Factory *** //
    auto  t    = T{'a', 12345, "wxyz", std::make_pair(3.1415, 1.4141), 0.98765};
    auto  x3   = make_push_back_tuple(t, std::string{"hello-world"});
    auto  x4   = make_push_back_tuple(t, std::string{"hello-world"}, 'A', 'B', 'C');
    auto  x5   = make_push_back_tuple(std::tuple<>{}, std::string{"hello-world"}, 'A');
    using ANS3 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double, std::string>;
    using ANS4 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double, std::string, char, char, char>;
    using ANS5 = std::tuple<std::string, char>;

    static_assert(std::is_same_v<decltype(x3), ANS3>, "failed to make_push_back_tuple");
    static_assert(std::is_same_v<decltype(x4), ANS4>, "failed to make_push_back_tuple");
    static_assert(std::is_same_v<decltype(x5), ANS5>, "failed to make_push_back_tuple");
}

void test_pop_front()
{
    using T    = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using X0   = pop_N_front_tuple<T, 0>::type;
    using X1   = pop_N_front_tuple<T, 1>::type;
    using X2   = pop_N_front_tuple<T, 2>::type;
    using ANS0 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, double>;
    using ANS1 = std::tuple<std::uint32_t, std::string, std::pair<double,double>, double>;
    using ANS2 = std::tuple<std::string, std::pair<double,double>, double>;
    
    static_assert(std::is_same_v<X0, ANS0>, "failed to pop_N_front_tuple");
    static_assert(std::is_same_v<X1, ANS1>, "failed to pop_N_front_tuple");
    static_assert(std::is_same_v<X2, ANS2>, "failed to pop_N_front_tuple");
}

void test_reverse()
{    
    using T   = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, std::vector<double>, double>;
    using X0  = reverse_tuple<T >::type;
    using X1  = reverse_tuple<X0>::type;
    using X2  = reverse_tuple2<T >::type;
    using X3  = reverse_tuple2<X2>::type;
    using ANS = std::tuple<double, std::vector<double>, std::pair<double,double>, std::string, std::uint32_t, char>;

    static_assert(std::is_same_v<X0, ANS>, "failed to reverse_tuple");
    static_assert(std::is_same_v<X1, T>,   "failed to reverse_tuple");
    static_assert(std::is_same_v<X2, ANS>, "failed to reverse_tuple2");
    static_assert(std::is_same_v<X3, T>,   "failed to reverse_tuple2");

    // *** Factory *** //
    auto  t  = T{'a', 12345, "wxyz", std::make_pair(3.1415, 1.4141), std::vector<double>{0.1,0.2,0.3,0.4}, 0.98765};
    auto  x4 = make_reverse_tuple_helper(t, idx_seq<0,1,3,4,5>{}); 
    auto  x5 = make_reverse_tuple(t); 
    using T4 = std::tuple<double, std::vector<double>, std::string, std::uint32_t, char>;
    using T5 = ANS;

    static_assert(std::is_same_v<decltype(x4), T4>, "failed to make_reverse_tuple");
    assert(std::get<0>(t) == std::get<4>(x4));
    assert(std::get<1>(t) == std::get<3>(x4));
    assert(std::get<2>(t) == std::get<2>(x4));

    static_assert(std::is_same_v<decltype(x5), T5>, "failed to make_reverse_tuple");
    assert(std::get<0>(t) == std::get<5>(x5));
    assert(std::get<1>(t) == std::get<4>(x5));
    assert(std::get<2>(t) == std::get<3>(x5));
}

void test_odd_pick()
{    
    using T   = std::tuple<char, char,
                           std::uint32_t, std::uint32_t,
                           std::string, std::string, 
                           std::pair<double,double>, std::pair<double,double>,
                           std::vector<double>, std::vector<double>,
                           double, double>;
    using X0   = odd_pick_tuple<T >::type;
    using X1   = odd_pick_tuple<X0>::type;
    using ANS0 = std::tuple<char, std::uint32_t, std::string, std::pair<double,double>, std::vector<double>, double>;
    using ANS1 = std::tuple<char, std::string, std::vector<double>>;

    static_assert(std::is_same_v<X0, ANS0>, "failed to odd_pick_tuple");
    static_assert(std::is_same_v<X1, ANS1>, "failed to odd_pick_tuple");

    // *** Factory *** //
    auto  t  = T{'a', 'a', 12345, 23456, 
                 "abcdf", "wxyz", 
                 std::make_pair(3.1415, 1.4141), 
                 std::make_pair(3.1415, 1.4141), 
                 std::vector<double>{0.1,0.2,0.3,0.4},
                 std::vector<double>{0.1,0.2,0.3,0.4},
                 0.98765, 0.98765};
    auto  x2   = make_odd_pick_tuple(t); 
    auto  x3   = make_odd_pick_tuple(x2); 
    using ANS2 = ANS0;
    using ANS3 = ANS1;

    static_assert(std::is_same_v<decltype(x2), ANS2>, "failed to make_reverse_tuple");
    assert(std::get<0>(t) == std::get<0>(x2));
    assert(std::get<2>(t) == std::get<1>(x2));
    assert(std::get<4>(t) == std::get<2>(x2));
      
    static_assert(std::is_same_v<decltype(x3), ANS3>, "failed to make_reverse_tuple");
    assert(std::get<0>(t) == std::get<0>(x3));
    assert(std::get<4>(t) == std::get<1>(x3));
}

void test_cat()
{
    using T0  = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>>;
    using T1  = std::tuple<char, std::uint32_t, std::string>;
    using T2  = tuple_cat<T0,T1>::type;
    using ANS = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>, char, std::uint32_t, std::string>;
    static_assert(std::is_same_v<T2, ANS>, "failed to tuple_cat");

    // *** Factory *** //
    auto  x0 = T0{'a', 12345, "wxyz", std::make_pair(3.1415, 1.4141)};
    auto  x1 = T1{'a', 12345, "wxyz"};
    auto  x2 = make_tuple_cat(x0, x1);

    static_assert(std::is_same_v<decltype(x2), ANS>, "failed to make_tuple_cat");
    assert(std::get<0>(x0) == std::get<0>(x2));
    assert(std::get<1>(x0) == std::get<1>(x2));
    assert(std::get<2>(x0) == std::get<2>(x2));
    assert(std::get<0>(x1) == std::get<4>(x2));
    assert(std::get<1>(x1) == std::get<5>(x2));
    assert(std::get<2>(x1) == std::get<6>(x2));
}

void test_apply()
{
    using T = std::tuple<char, std::uint32_t, std::string, std::pair<double, double>>;
    auto  t = T{'a', 12345, "wxyz", std::make_pair(3.1415, 1.4141)};
    auto  f = [](char c, std::uint32_t n, const std::string& s, const std::pair<double,double>& p)
    {
        std::cout << "\n";
        std::cout << "\nThis is tuple apply.";
        std::cout << "\nchar          = " << c;
        std::cout << "\nstd::uint32_t = " << n;
        std::cout << "\nstd::string   = " << s;
        std::cout << "\nstd::pair<>   = " << p.first << "/" << p.second;
        std::cout << "\n";
    };
    apply(t,f);
}

void test_my_tuple()
{
    my_tuple<std::uint32_t,std::uint32_t,std::uint32_t> tup0
    {
        1, 2, 3
    };

    my_tuple<std::uint32_t,std::string,std::string,double> tup1
    {
        1, "ABC", "DEF", 3.14
    };

    my_tuple<std::uint32_t,std::vector<int>,std::vector<double>,double,std::string> tup2
    {
        1, std::vector{11,12,13}, std::vector<double>{0.123,0.234}, 2.718238, "hello"
    };

    static_assert(my_tuple_size<decltype(tup0)>::value == 3, "failed to my_tuple_size");
    static_assert(my_tuple_size<decltype(tup1)>::value == 4, "failed to my_tuple_size");
    static_assert(my_tuple_size<decltype(tup2)>::value == 5, "failed to my_tuple_size");

    std::cout << "\ntuple0 get = " << get<0>(tup0);
    std::cout << "\ntuple0 get = " << get<1>(tup0);
    std::cout << "\ntuple0 get = " << get<2>(tup0);
    std::cout << "\ntuple1 get = " << get<0>(tup1);
    std::cout << "\ntuple1 get = " << get<1>(tup1);
    std::cout << "\ntuple1 get = " << get<2>(tup1);
    std::cout << "\ntuple1 get = " << get<3>(tup1);
    std::cout << "\ntuple2 get = " << get<0>(tup2);
    std::cout << "\ntuple2 get = " << get<1>(tup2)[0];
    std::cout << "\ntuple2 get = " << get<2>(tup2)[0];
    std::cout << "\ntuple2 get = " << get<3>(tup2);
    std::cout << "\ntuple2 get = " << get<4>(tup2);

    auto tup3 = make_my_tuple
    (
        std::uint32_t{1}, std::uint32_t{2}, std::uint32_t{3}
    );

    auto tup4 = make_my_tuple
    (
        std::uint32_t{1}, std::string{"ABC"}, std::string{"DEF"}, double{3.14}
    );

    auto tup5 = make_my_tuple
    (
        std::uint32_t{1}, std::vector{11,12,13}, std::vector<double>{0.123,0.234}, double{2.718238}, std::string{"hello"}
    );

    static_assert(my_tuple_size<decltype(tup3)>::value == 3, "failed to my_tuple_size");
    static_assert(my_tuple_size<decltype(tup4)>::value == 4, "failed to my_tuple_size");
    static_assert(my_tuple_size<decltype(tup5)>::value == 5, "failed to my_tuple_size");

    std::cout << "\ntuple3 get = " << get<0>(tup3);
    std::cout << "\ntuple3 get = " << get<1>(tup3);
    std::cout << "\ntuple3 get = " << get<2>(tup3);
    std::cout << "\ntuple4 get = " << get<0>(tup4);
    std::cout << "\ntuple4 get = " << get<1>(tup4);
    std::cout << "\ntuple4 get = " << get<2>(tup4);
    std::cout << "\ntuple4 get = " << get<3>(tup4);
    std::cout << "\ntuple5 get = " << get<0>(tup5);
    std::cout << "\ntuple5 get = " << get<1>(tup5)[0];
    std::cout << "\ntuple5 get = " << get<2>(tup5)[0];
    std::cout << "\ntuple5 get = " << get<3>(tup5);
    std::cout << "\ntuple5 get = " << get<4>(tup5);
}

void test_template_tuple()
{
    test_size_and_element();
    test_shuffle();
    test_push_front();
    test_push_back();
    test_pop_front();
    test_reverse();
    test_odd_pick();
    test_cat();
    test_apply();
    test_my_tuple();
}

