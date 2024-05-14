#pragma once
#include<iostream>
#include<tuple>
#include<idx_seq.h>

// In the following : 
// 1. template class is type-traits
// 2. template function is factory 
// 3. they might differ in implementation 
// 4. for template class, we usually have to (see tuple_size comment)
// -  implement a generic class as interface 
// -  implement a specialized class as implementation 
// -  the generic class defines dimension of template 
// -  the specialized class converts original dimension to another dimension (see remark A)
// 5. if the specialized class is implemented recursively, we need to provide boundary case(s)



// ****************** //
// *** Tuple size *** //
// ****************** //
template<typename TUP>
struct tuple_size 
{
};

template<typename...Ts>
struct tuple_size<std::tuple<Ts...>>               // <--- Change in interface, from TUP to Ts...
{
    using type = std::integral_constant<std::size_t, sizeof...(Ts)>;
    static const std::size_t value = type::value;
};



// ********************* //
// *** Tuple element *** //
// ********************* //
// *** Method 1 *** //
template<std::size_t N, typename TUP>              // <--- This is interface.
struct tuple_element 
{
};

template<std::size_t N, typename T, typename...Ts> // <--- This is recursion.
struct tuple_element<N, std::tuple<T,Ts...>>
{
    using type = tuple_element<N-1, std::tuple<Ts...>>::type;
};

template<typename T, typename...Ts>                // <--- This is boundary case.
struct tuple_element<0, std::tuple<T,Ts...>>
{
    using type = T;
};

// *** Method 2 *** //
template<std::size_t N, typename TUP> 
struct tuple_element2
{
    using type = std::remove_cvref_t<decltype(std::get<N>(std::declval<TUP>()))>;
//  using type =                     decltype(std::get<N>(std::declval<TUP>())); // BUG : Does not work !!!
};



// ********************* //
// *** Shuffle tuple *** //
// ********************* //
// Slight difference in interface between method 1&2, results are the same.
// 1. shuffle_tuple         <TUP,1,3,5,7>
// 2. shuffle_tuple2<idx_seq<TUP,1,3,5,7>>
  
// *** Method 1 *** //
template<typename TUP, std::size_t...Ns>
struct shuffle_tuple
{
    using type = std::tuple<typename std::tuple_element<Ns,TUP>::type...>; 
};

// *** Method 2 *** //
template<typename TUP, typename T> 
struct shuffle_tuple2                                      // <--- This is mainly for facilitating other tuple algo with helper.
{
};

template<typename TUP, std::size_t...Ns> 
struct shuffle_tuple2<TUP, idx_seq<Ns...>> 
{
    using type = std::tuple<typename std::tuple_element<Ns,TUP>::type...>; 
};

// *** Factory *** //
template<typename TUP, std::size_t...Ns>
auto make_shuffle_tuple(const TUP& tup, idx_seq<Ns...>)    // <--- This is mainly for facilitating other tuple algo with helper.
{
    return std::make_tuple(std::get<Ns>(tup)...);
}



// ********************************** //
// *** Push front & back to tuple *** //
// ********************************** //
template<typename TUP, typename...Ts> 
struct push_front_tuple                                    // <--- This is interface.
{
};

template<typename...TUP_Ts, typename T, typename...Ts>
struct push_front_tuple<std::tuple<TUP_Ts...>,T,Ts...>     // <--- This is recursion.
{
    using type = typename push_front_tuple<std::tuple<T,TUP_Ts...>, Ts...>::type;
};

template<typename...TUP_Ts>
struct push_front_tuple<std::tuple<TUP_Ts...>>             // <--- This is boundary case.
{
    using type = std::tuple<TUP_Ts...>;
};

template<typename TUP, typename...Ts> 
struct push_back_tuple                                     // <--- This is interface.
{
};

template<typename...TUP_Ts, typename T, typename...Ts>
struct push_back_tuple<std::tuple<TUP_Ts...>,T,Ts...>      // <--- This is recursion.
{
    using type = typename push_back_tuple<std::tuple<TUP_Ts...,T>,Ts...>::type;
};

template<typename...TUP_Ts>
struct push_back_tuple<std::tuple<TUP_Ts...>>              // <--- This is boundary case.
{
    using type = std::tuple<TUP_Ts...>;
};

// *** Factory *** //
// The following technique is useful in push / reverse / cat :
// - we have TUP   in the interface
// - we need Ns... in the implementation
// - introduce a helper to do conversion
//
template<typename TUP, typename...Ts>
auto make_push_front_tuple(const TUP& tup, const Ts&...xs)                                                   // <--- This is interface, delegate to helper.
{
    return make_push_front_tuple_helper(tup, typename idx_seq_generator<std::tuple_size<TUP>::value>::type{}, xs...);
}

template<typename TUP, std::size_t...Ns, typename T, typename...Ts>
auto make_push_front_tuple_helper(const TUP& tup, idx_seq<Ns...> dummy, const T& x, const Ts&...xs)          // <--- This is recursion of helper.
{
    return make_push_front_tuple_helper
    (
        std::make_tuple(x, std::get<Ns>(tup)...),
        typename idx_seq_generator<std::tuple_size<TUP>::value + 1>::type{},
        xs...
    );
}

template<typename TUP, std::size_t...Ns>
auto make_push_front_tuple_helper(const TUP& tup, idx_seq<Ns...> dummy)                                      // <--- This is boundary case of helper.
{
    return tup;
}

template<typename TUP, typename...Ts>
auto make_push_back_tuple(const TUP& tup, const Ts&...xs)                                                    // <--- This is interface, delegate to helper.
{
    return make_push_back_tuple_helper(tup, typename idx_seq_generator<std::tuple_size<TUP>::value>::type{}, xs...);
}

template<typename TUP, std::size_t...Ns, typename T, typename...Ts>
auto make_push_back_tuple_helper(const TUP& tup, idx_seq<Ns...> dummy, const T& x, const Ts&...xs)           // <--- This is recursion of helper.
{
    return make_push_back_tuple_helper
    (
        std::make_tuple(std::get<Ns>(tup)..., x),
        typename idx_seq_generator<std::tuple_size<TUP>::value + 1>::type{},
        xs...
    );
}

template<typename TUP, std::size_t...Ns>
auto make_push_back_tuple_helper(const TUP& tup, idx_seq<Ns...> dummy)                                       // <--- This is boundary case of helper.
{
    return tup;
}



// *********************************** //
// *** Pop front & back from tuple *** //
// *********************************** //
template<typename TUP> 
struct pop_front_tuple
{
};

template<typename TUP_T, typename...TUP_Ts>
struct pop_front_tuple<std::tuple<TUP_T,TUP_Ts...>>
{
    using type = std::tuple<TUP_Ts...>;
};

template<typename T, std::size_t N>
struct pop_N_front_tuple
{
    using type = typename pop_N_front_tuple<typename pop_front_tuple<T>::type, N-1>::type;
};

template<typename T>
struct pop_N_front_tuple<T,0>
{
    using type = T;
};



// ********************* //
// *** Reverse tuple *** //
// ********************* //
// *** Method 1 (using same implementation as reverse_idx_seq) *** //
template<typename T>
struct reverse_tuple                                       // <--- This is interface.
{
};

template<typename T, typename...Ts> 
struct reverse_tuple<std::tuple<T,Ts...>>                  // <--- This is recursion.
{
    using type = typename push_back_tuple<typename reverse_tuple<std::tuple<Ts...>>::type, T>::type;
};

template<>
struct reverse_tuple<std::tuple<>>                         // <--- This is boundary case.
{
    using type = std::tuple<>; 
};


// *** Method 2 (using inv_idx_seq_generator) *** //
template<typename TUP>
struct reverse_tuple2
{
    using type = typename shuffle_tuple2<TUP, typename inv_idx_seq_generator<std::tuple_size<TUP>::value>::type>::type;
};

// *** Factory *** //
template<typename TUP>
auto make_reverse_tuple(const TUP& tup)
{
    return make_reverse_tuple_helper(tup, typename idx_seq_generator<std::tuple_size<TUP>::value>::type{});
}

template<typename TUP, std::size_t...Ns> 
auto make_reverse_tuple_helper(const TUP& tup, idx_seq<Ns...> dummy)
{
    return std::make_tuple(std::get<std::tuple_size<TUP>::value-1-Ns>(tup)...); // BUG : Don't forget minus one, otherwise it goes out of tuple range
}



// *********************** //
// **** Odd pick tuple *** //
// *********************** //
// using same implementation as odd_pick_idx_seq
template<typename T>
struct odd_pick_tuple                                      // <--- This is interface.
{
};

template<typename T0, typename T1, typename...Ts> 
struct odd_pick_tuple<std::tuple<T0,T1,Ts...>>             // <--- This is recursion.
{
    using type = typename push_front_tuple<typename odd_pick_tuple<std::tuple<Ts...>>::type, T0>::type;
}; 

template<>
struct odd_pick_tuple<std::tuple<>>                        // <--- This is boundary case for even size.
{
    using type = std::tuple<>; 
};

template<typename T>
struct odd_pick_tuple<std::tuple<T>>                       // <--- This is boundary case for odd size.
{
    using type = std::tuple<T>; 
};


  
// *** Factory *** //
// using same implementation as odd_pick_tuple
template<typename TUP>
auto make_odd_pick_tuple(const TUP& tup)                   // <--- This is interface.
{
    return make_odd_pick_tuple_helper(tup, typename idx_seq_generator<std::tuple_size<TUP>::value>::type{}); 
}

template<typename TUP, std::size_t N0, std::size_t N1, std::size_t...Ns> 
auto make_odd_pick_tuple_helper(const TUP& tup, idx_seq<N0,N1,Ns...> dummy) 
{
    return make_push_front_tuple                           // <--- This is recursion.
    (
        make_odd_pick_tuple_helper(tup, idx_seq<Ns...>{}),  std::get<N0>(tup)
    );
}

template<typename TUP> 
auto make_odd_pick_tuple_helper(const TUP& tup, idx_seq<> dummy)
{
    return std::tuple<>{};                                 // <--- This is boundary case for even size. 
}

template<typename TUP, std::size_t N> 
auto make_odd_pick_tuple_helper(const TUP& tup, idx_seq<N> dummy)
{
    return std::make_tuple(std::get<N>(tup));              // <--- This is boundary case for odd size.
}



// ***************** //
// *** Tuple cat *** //
// ***************** //
template<typename TUP0, typename TUP1>
struct tuple_cat                                           // <--- This is interface.
{
};

template<typename TUP0, typename...Ts>
struct tuple_cat<TUP0, std::tuple<Ts...>>                  // <--- This is recursion.
{
    using type = typename push_back_tuple<TUP0,Ts...>::type;
}; 

// *** Factory *** //
template<typename TUP0, typename TUP1>  
auto make_tuple_cat(const TUP0& tup0, const TUP1& tup1)    // <--- This is interface.
{
    return make_tuple_cat_helper(tup0, tup1, typename idx_seq_generator<std::tuple_size<TUP1>::value>::type{});
}

template<typename TUP0, typename TUP1, std::size_t...Ns>   // <--- This is recursion.
auto make_tuple_cat_helper(const TUP0& tup0, const TUP1& tup1, idx_seq<Ns...> dummy)
{
    return make_push_back_tuple(tup0, std::get<Ns>(tup1)...);
}



// ******************* //
// *** Tuple apply *** //
// ******************* //
template<typename TUP, typename F>
void apply(TUP& tup, F& fct)                               // <--- This is interface.
{
    apply_helper(tup, typename idx_seq_generator<std::tuple_size<TUP>::value>::type{}, fct);
}

template<typename TUP, std::size_t...Ns, typename F>
void apply_helper(TUP& tup, idx_seq<Ns...> dummy, F& fct)  // <--- This is implementation without recursion.
{
    fct(std::get<Ns>(tup)...);
}



