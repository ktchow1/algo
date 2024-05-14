#include<iostream>


// *************************** //
// *** Design my own tuple *** //
// *************************** //
// Keep tuple as POD, no explicit constructor is needed, aggregate initialization works.
//
template<typename T, typename...Ts>
struct my_tuple
{
    T x;
    my_tuple<Ts...> tup;
};

template<typename T>
struct my_tuple<T>
{
    T x;
};



// ****************** //
// *** Tuple size *** //
// ****************** //
template<typename T>
struct my_tuple_size
{
    using type = std::integral_constant<std::size_t, 0>;
    static const std::size_t value = type::value;
};

template<typename...Ts>
struct my_tuple_size<my_tuple<Ts...>>
{
    using type = std::integral_constant<std::size_t, sizeof...(Ts)>;
    static const std::size_t value = type::value;
};



// ***************************************** //
// *** Get (method 1 2 works and simple) *** //
// ***************************************** //
template<std::size_t N, typename TUP>
auto get(const TUP& tup)                                   // <--- This is interface.
{
    return get_helper(tup, std::integral_constant<std::size_t,N>{});
}

template<typename TUP, std::size_t N>
auto get_helper(const TUP& tup,                            // <--- This is recursion.
                std::integral_constant<std::size_t,N>) 
{
    return get_helper(tup.tup, std::integral_constant<std::size_t,N-1>{});
}

template<typename TUP>
auto get_helper(const TUP& tup,                            // <--- This is boundary case.
                std::integral_constant<std::size_t,0>)
{
    return tup.x;
}



// *************** //
// *** Factory *** //
// *************** //
template<typename...Ts>
auto make_my_tuple(const Ts&...ts)
{
//  return my_tuple<Ts...>(ts...); // Bug : does not work, please check the unpack of () vs {}
    return my_tuple<Ts...>{ts...}; 
}
