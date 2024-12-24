#include<iostream>
#include<iomanip>
#include<cassert>
#include<stack.h>


void test_queue_from_stack()
{
    using tester_type = alg::container_tester<std::queue<std::uint32_t>, alg::queue<std::uint32_t>>;
    void (std::queue<std::uint32_t>::* std_push)(const std::uint32_t&) = &std::queue<std::uint32_t>::push;
    void (alg::queue<std::uint32_t>::* alg_push)(const std::uint32_t&) = &alg::queue<std::uint32_t>::push;
    void (std::queue<std::uint32_t>::* std_pop)() = &std::queue<std::uint32_t>::pop;
    void (alg::queue<std::uint32_t>::* alg_pop)() = &alg::queue<std::uint32_t>::pop;
    const std::uint32_t& (std::queue<std::uint32_t>::* std_front)() const = &std::queue<std::uint32_t>::front;
    const std::uint32_t& (alg::queue<std::uint32_t>::* alg_front)() const = &alg::queue<std::uint32_t>::front;

    // empty case
    tester_type tester;
    tester.apply0(&std::queue<std::uint32_t>::push, &alg::queue<std::uint32_t>::push, (std::uint32_t)100);
    tester.apply0(&std::queue<std::uint32_t>::pop,  &alg::queue<std::uint32_t>::pop);
    assert(tester.compare(std_front, alg_front));
    std::cout << "\n[OK]  empty queue, apply0() with fct_ptr directly passed";

    tester.apply0(std_push, alg_push, (std::uint32_t)100); 
    tester.apply0(std_pop,  alg_pop);
    assert(tester.compare(std_front, alg_front));
    std::cout << "\n[OK]  empty queue, apply0() with fct_ptr passed as variable";

//  tester.apply1(&std::queue<std::uint32_t>::push, &alg::queue<std::uint32_t>::push, (std::uint32_t)100);  // BUG : cannot resolve FP0, can only resolve FP1
//  tester.apply1(&std::queue<std::uint32_t>::pop,  &alg::queue<std::uint32_t>::pop);                       //       can    resolve FP0 and FP1
//  assert(tester.compare(std_front, alg_front));                                                           //       since there are overloads for push()

    tester.apply1(std_push, alg_push, 100); 
    tester.apply1(std_pop,  alg_pop);
    assert(tester.compare(std_front, alg_front));
    std::cout << "\n[OK]  empty queue, apply1() with fct_ptr passed as variable";

    // normal case
    std::uint32_t error = 0;
    for(std::uint32_t t=0; t!=100; ++t)
    {
        for(std::uint32_t n=0; n!=100; ++n)
        {
            tester.apply1(std_push, alg_push, 100+n);
            if (rand()%3==0)
            {
                tester.apply1(std_pop, alg_pop);
            }
        }
        assert(tester.compare(std_front, alg_front));
    }
    std::cout << "\n[OK] normal queue, apply1() with fct_ptr directly passed";
}

// ******************************************************** //
// Compare remark 1&2
// * there is  a must to cast the element type for apply0()
// * there is no need to cast the element type for apply1()
// ******************************************************** //
void test_stack_from_queue()
{
    using tester_type = alg::container_tester<std::stack<std::uint32_t>, alg::stack<std::uint32_t>>;
    void (std::stack<std::uint32_t>::* std_push)(const std::uint32_t&) = &std::stack<std::uint32_t>::push;
    void (alg::stack<std::uint32_t>::* alg_push)(const std::uint32_t&) = &alg::stack<std::uint32_t>::push;
    void (std::stack<std::uint32_t>::* std_pop)() = &std::stack<std::uint32_t>::pop;
    void (alg::stack<std::uint32_t>::* alg_pop)() = &alg::stack<std::uint32_t>::pop;
    const std::uint32_t& (std::stack<std::uint32_t>::* std_top)() const = &std::stack<std::uint32_t>::top;
    const std::uint32_t& (alg::stack<std::uint32_t>::* alg_top)() const = &alg::stack<std::uint32_t>::top;

    // empty case
    tester_type tester;
    tester.apply0(std_push, alg_push, (std::uint32_t)100); // remark 1
    tester.apply0(std_pop,  alg_pop);
    assert(tester.compare(std_top, alg_top));
    std::cout << "\n[OK]  empty stack, apply0() with fct_ptr passed as variable";

    tester.apply1(std_push, alg_push, 100); // remark 2
    tester.apply1(std_pop,  alg_pop);
    assert(tester.compare(std_top, alg_top));
    std::cout << "\n[OK]  empty stack, apply1() with fct_ptr passed as variable"; 

    // normal case
    for(std::uint32_t t=0; t!=100; ++t)
    {
        for(std::uint32_t n=0; n!=100; ++n)
        {
            tester.apply1(std_push, alg_push, 100+n);
            if (rand()%3==0)
            {
                tester.apply1(std_pop, alg_pop);
            }
        }
        assert(tester.compare(std_top, alg_top));
    }
    std::cout << "\n[OK] normal stack, apply1() with fct_ptr directly passed";
}

void test_stack_from_vector()
{
}

void test_stack_with_medium()
{
}

void test_stack()
{
    test_queue_from_stack();
    test_stack_from_queue();
    test_stack_from_vector();
    test_stack_with_medium();
}

