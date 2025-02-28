#include<iostream>
#include<cstdint>
#include<cassert>
#include<exception>
#include<utility.h>




namespace test
{


    struct ex_base
    {
    };

    struct ex_derived : public ex_base
    {
    };




}



void test_exception()
{

    print_summary("exception", "succeeded");
}
