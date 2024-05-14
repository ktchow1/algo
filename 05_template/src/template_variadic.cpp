#include<iostream>

template<typename OS, typename...Ts>
OS& ostreaming0(OS& os, Ts&&...args)
{
//   os    ... << std::forward<Ts>(args);  // compile error, bracket is a must
    (os << ... << std::forward<Ts>(args));
    return os;
}

template<typename OS, typename...Ts>
OS& ostreaming1(OS& os, Ts&&...args)
{
//   (os << args), ... ; // compile error, outer bracket is a must
//  ( os << args , ...); // compile error, inner bracket is a must
    ((os << args), ...);
    return os;
}

// ********************************************************************* //
// *** Advantage comma operator to ostream operator : Add formatting *** //
// ********************************************************************* //
template<typename OS, typename...Ts>
OS& ostreaming2(OS& os, Ts&&...args)
{
    std::size_t n=0;
//  ((os << args <<  ++n==sizeof...(Ts)? "." : ", " ), ...); // compile error, bracket around ?: operator is a must
    ((os << args << (++n==sizeof...(Ts)? "." : ", ")), ...);
    return os;
}

void test_template_variadic()
{
    std::cout << "\n";
    ostreaming0(std::cout, 1, 2, 3, "ABC", 123, "DEF", "WORLD");

    std::cout << "\n";
    ostreaming1(std::cout, 1, 2, 3, "ABC", 123, "DEF", "WORLD");

    std::cout << "\n";
    ostreaming2(std::cout, 1, 2, 3, "ABC", 123, "DEF", "WORLD");
}
