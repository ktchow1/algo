#include <template_specialization_in_header.h>

template<>
void template_function(const ARG2& x)
{
    x.fct();
}

void test_template_specialization_in_header()
{
    std::cout << "\ntest template specialization in header";

    ARG0 arg0;
    ARG1 arg1;
    ARG2 arg2;

    template_function(arg0);
    template_function(arg1);
    template_function(arg2);

}
