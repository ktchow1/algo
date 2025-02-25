#pragma once
#include<cstdint>
#include<functional>


namespace alg
{
    // Filter, transform ... etc, are all wrappers, supporting CTAD without explicity guide.
    template<typename F> 
    struct filter    
    { 
        F impl; 
    };

    template<typename F> 
    struct transform 
    { 
        F impl; 
    };
    
    struct take      
    { 
        std::uint32_t impl; 
    };


    // *********************************************** //
    // *** A container view, with lazy calculation *** //
    // *********************************************** //
    template<typename C> // C = container
    struct lazy_view
    {
    public:
        using T = typename C::value_type;

        lazy_view(C& container) : i0(container.begin()), 
                                  i1(container.end()),
                                  predicate{},
                                  transformation{},
                                  taken_count{0},
                                  taken_total{0}
        {
        }

        // casting operator
        operator C()
        {
            // This is lazy calculation.
            C output;
            for(auto i=i0; i!=i1; ++i)
            {
                if (predicate(*i))
                {
                    output.push_back(transformation(*i));

                    ++taken_count;
                    if (taken_count == taken_total) return output;
                }
            }
            return output;
        }

        auto begin() const // Todo : not completed
        {
            return i0;
        }

        auto end() const 
        {
            return i1;
        }

    public:
        typename C::iterator    i0;
        typename C::iterator    i1;    
        std::function<bool(T)>  predicate;
        std::function<T(T)>     transformation;
        std::uint32_t           taken_count;
        std::uint32_t           taken_total;
    };


    // ***************************************************************************************************** //
    // *** The pipeline expression (in test) resolves into the following operators, returning lazy_view. *** //
    // ***************************************************************************************************** //
    template<typename C, typename F>
    lazy_view<C> operator | (C& lhs, filter<F> rhs)
    {
        lazy_view<C> output(lhs);
        output.predicate = rhs.impl; 
        return output;
    }

    // more ...



    // ***************** //
    // *** Cascading *** //
    // ***************** //
    template<typename C, typename F>
    lazy_view<C> operator | (lazy_view<C> lhs, filter<F> rhs)
    {
        lhs.predicate = rhs.impl; 
        return lhs;
    }

    template<typename C, typename F>
    lazy_view<C> operator | (lazy_view<C> lhs, transform<F> rhs)
    {
        lhs.transformation = rhs.impl; 
        return lhs;
    }

    template<typename C>
    lazy_view<C> operator | (lazy_view<C> lhs, take rhs)
    {
        lhs.taken_count = 0;
        lhs.taken_total = rhs.impl; 
        return lhs;
    }
}

