#include<iostream>
#include<sstream>
#include<cassert>
#include<string>
#include<traits.h>
#include<variadic.h>


void test_parameter_pack_expansion()
{
    alg::ostream_in_old_way(std::cout, "\nostream_in_old_way = ", 1, 2, 3, 4, std::string{"ABC"}, std::string{"DEF"}, std::string{"GHIJ"}, std::string{"KLMN"});
}

void test_parameter_pack_nested_expansion()
{
    alg::ostream_nested(std::cout, 1, 2, 3, 4, std::string{"ABC"}, std::string{"DEF"}, std::string{"GHIJ"}, std::string{"KLMN"});
}

void test_fold_expansion()
{
    std::stringstream ss0;
    alg::ostream_by_ostream_op(ss0, 1, 2, 3, 4, "ABC", "DEF", "GHIJ", "KLMN");

    std::stringstream ss1;
    alg::ostream_by_comma_op(ss1, 1, 2, 3, 4, "ABC", "DEF", "GHIJ", "KLMN");

    std::stringstream ss2;
    alg::ostream_by_comma_op2(ss2, 1, 2, 3, 4, "ABC", "DEF", "GHIJ", "KLMN");

    std::stringstream ss3;
    alg::ostream_by_comma_op_decorated(ss3, 1, 2, 3, 4, "ABC", "DEF", "GHIJ", "KLMN");

    std::stringstream ss4;
    alg::ostream_by_comma_op_decorated2(ss4, 1, 2, 3, 4, "ABC", "DEF", "GHIJ", "KLMN");

    std::stringstream ss5;
    (ss5 << 1, ss5 << 2, ss5 << 3, ss5 << 4, ss5 << "ABC", ss5 << "DEF", ss5 << "GHIJ", ss5 << "KLMN");

    std::cout << "\nostream_by_ostream_op          = " << ss0.str();
    std::cout << "\nostream_by_comma_op            = " << ss1.str();
    std::cout << "\nostream_by_comma_op2           = " << ss2.str();
    std::cout << "\nostream_by_comma_op_decorated  = " << ss3.str();
    std::cout << "\nostream_by_comma_op_decorated2 = " << ss4.str();
    std::cout << "\nostream manully                = " << ss5.str();
    std::cout << "\n";

    std::cout << "\n[Difference between parameter-pack-expansion & fold-expression-with-comma ]";
    alg::double_up_and_ostream(1, 2, 3, 4, std::string{"ABC"}, std::string{"DEF"}, std::string{"GHIJ"}, std::string{"KLMN"});

    std::cout << "\n";
}

template<int N> struct base 
{
    explicit base(int id) : m_id(id) 
    {
    }

    void fct()  
    { 
        std::cout << "\nThis is base" << N << " with id " << m_id;
        m_fct_invoked = true; 
    }

    bool m_fct_invoked = false;
    int m_id = 0;
};

void test_other_variadic()
{
    static_assert(alg::multiple_inheritance<base<1>>                        ::num_of_base == 1, "failed to use multiple inheritance");
    static_assert(alg::multiple_inheritance<base<1>,base<2>>                ::num_of_base == 2, "failed to use multiple inheritance");
    static_assert(alg::multiple_inheritance<base<1>,base<2>,base<4>>        ::num_of_base == 3, "failed to use multiple inheritance");
    static_assert(alg::multiple_inheritance<base<1>,base<2>,base<4>,base<8>>::num_of_base == 4, "failed to use multiple inheritance");

    alg::multiple_inheritance<base<1>,base<2>,base<4>,base<8>> derived(12345);
    assert(derived.base<1>::m_id == 12345);
    assert(derived.base<2>::m_id == 12345);
//  assert(derived.base<3>::m_id == 12346); // compile error as expected, no this base class : base<3>
    assert(derived.base<4>::m_id == 12345);
    assert(derived.base<8>::m_id == 12345);

    assert(derived.base<1>::m_fct_invoked == false);
    assert(derived.base<2>::m_fct_invoked == false);
    assert(derived.base<4>::m_fct_invoked == false);
    assert(derived.base<8>::m_fct_invoked == false);
    derived.run();
    assert(derived.base<1>::m_fct_invoked ==  true);
    assert(derived.base<2>::m_fct_invoked ==  true);
    assert(derived.base<4>::m_fct_invoked ==  true);
    assert(derived.base<8>::m_fct_invoked ==  true);

    // pack indexing will be available in c++26
//  static_assert(alg::is_same_v<typename alg::pack_indexing<0,bool,char,int,std::string>::type,        bool>, "failed to use pack idnexing"); 
//  static_assert(alg::is_same_v<typename alg::pack_indexing<1,bool,char,int,std::string>::type,        char>, "failed to use pack idnexing"); 
//  static_assert(alg::is_same_v<typename alg::pack_indexing<2,bool,char,int,std::string>::type,         int>, "failed to use pack idnexing"); 
//  static_assert(alg::is_same_v<typename alg::pack_indexing<3,bool,char,int,std::string>::type, std::string>, "failed to use pack idnexing"); 
}

void test_variadic()
{
    test_parameter_pack_expansion();
    test_parameter_pack_nested_expansion();
    test_fold_expansion();
    test_other_variadic();
}
