#include<iostream>
#include<string>
#include<vector>
#include<function.h>
#include<utility.h>



void sample_function0() 
{ 
    std::cout << "\nsample_function0"; 
}

struct sample_functor0
{
    void operator()() const 
    {
       std::cout << "\nsample_functor0"; 
    }
};

std::string sample_function1(int, int) 
{
    std::cout << "\nsample_function1"; 
    return "xxx"; 
}

struct sample_functor1
{
    std::string operator()(int, int) const 
    { 
        std::cout << "\nsample_functor1"; 
        return "xxx"; 
    }
};

// *******************************************//
// Objective : function class that can wrap :
// * function pointer
// * functor
// * lambda 
// *******************************************//
void test_simple_function()
{
    sample_functor0 f;
    std::vector<alg::simple_function> fs; 

    // using raw type
    fs.push_back(&sample_function0);   // <--- cannot compile all these lines, if "explicit" is added to simple_function constructor
    fs.push_back(sample_functor0{});
    fs.push_back(f);                   // <--- cannot compile this line, if std::decay_t is removed
    fs.push_back([]()
    {
        std::cout << "\nsample_lambda0"; 
    });

    // using simple_function type
    alg::simple_function f0(&sample_function0);
    alg::simple_function f1(sample_functor0{});
    alg::simple_function f2(f);
    alg::simple_function f3([]()
    {
        std::cout << "\nsample_lambda0"; 
    });
    fs.push_back(f0);                  // <--- cannot compile all these lines, if unique_ptr is used instead of shared_ptr inside simple_function
    fs.push_back(f1);                  //      std::unique_ptr makes simple_function non-copyable 
    fs.push_back(f2);                  //      std::shared_ptr makes simple_function     copyable 
    fs.push_back(f3);

    for(const auto& f:fs) f();    

    print_summary("re-implement function - nullary", "succeeded");
}
  
void test_general_function()
{
    sample_functor1 f;
    std::vector<alg::function<std::string,int,int>> fs; 

    // using raw type
    fs.push_back(&sample_function1);   
    fs.push_back(sample_functor1{});
    fs.push_back(f);           
    fs.push_back([](int,int) -> std::string
    {
        std::cout << "\nsample_lambda1";
        return "xxx"; 
    });

    // using simple_function type
    alg::function<std::string,int,int> f0(&sample_function1);
    alg::function<std::string,int,int> f1(sample_functor1{});
    alg::function<std::string,int,int> f2(f);
    alg::function<std::string,int,int> f3([](int,int) -> std::string
    {
        std::cout << "\nsample_lambda1";
        return "xxx"; 
    });
    fs.push_back(f0); 
    fs.push_back(f1);
    fs.push_back(f2);
    fs.push_back(f3);

    for(const auto& f:fs) f(123,123);    

    print_summary("re-implement function - N-ary return R", "succeeded");
}

void test_function()
{
    test_simple_function();
    test_general_function();
}

