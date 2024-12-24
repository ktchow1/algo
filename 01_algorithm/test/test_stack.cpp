#include<iostream>
#include<stack.h>


void test_queue_from_stack()
{
    alg::container_tester<std::queue<std::uint32_t>, alg::queue_from_stack<std::uint32_t>> tester;

    tester.apply(&std::queue<std::uint32_t>::push, &alg::queue_from_stack<std::uint32_t>::push, (std::uint32_t)123);
    tester.apply(&std::queue<std::uint32_t>::pop,  &alg::queue_from_stack<std::uint32_t>::pop);

    tester.apply2(&std::queue<std::uint32_t>::push, &alg::queue_from_stack<std::uint32_t>::push, (std::uint32_t)123);
    tester.apply2(&std::queue<std::uint32_t>::pop,  &alg::queue_from_stack<std::uint32_t>::pop);
    tester.compare();

}

void test_stack_from_queue()
{
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

