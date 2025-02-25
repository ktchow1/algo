#pragma once
#include<cstdint>
#include<functional>


namespace alg
{
    // ******************************************************** //
    // *** Wrappers supporting CTAD without explicity guide *** //
    // ******************************************************** //
    template<typename FCT> 
    struct filter    
    { 
        FCT value; 
    };

    template<typename FCT> 
    struct transform 
    { 
        FCT value; 
    };
    
    struct take      
    { 
        std::uint32_t value; 
    };


    // ********************************************** //
    // *** A container view, with lazy evaluation *** //
    // ********************************************** //
    template<typename C> // C = container
    struct lazy_view
    {
    public:
        using T = typename C::value_type;

        using  filt_type = std::function<bool(T)>;
        using  tran_type = std::function<T(T)>;
        struct take_type
        {
            std::uint32_t m_count;
            std::uint32_t m_limit;
        };

    public:
        lazy_view(C& container) : i0(container.begin()), 
                                  i1(container.end()),
                                  m_filter{},
                                  m_transform{},
                                  m_take{}
        {
        }

        // casting operator
        operator C()
        {
            // This is lazy evaluation.
            C output;
            for(auto i=i0; i!=i1; ++i)
            {
                if (m_filter(*i))
                {
                    output.push_back(m_transform(*i));

                    ++m_take.m_count;
                    if (m_take.m_count == m_take.m_limit) return output;
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
        typename C::iterator i0;
        typename C::iterator i1;    
        filt_type m_filter;
        tran_type m_transform;
        take_type m_take;
    };


    // ***************************************************************************************************** //
    // *** The pipeline expression (in test) resolves into the following operators, returning lazy_view. *** //
    // ***************************************************************************************************** //
    template<typename C, typename FCT>
    lazy_view<C> operator | (C& lhs, filter<FCT> rhs)
    {
        lazy_view<C> output(lhs);
        output.m_filter = rhs.value; 
        return output;
    }

    // more ...



    // ***************** //
    // *** Cascading *** //
    // ***************** //
    template<typename C, typename FCT>
    lazy_view<C> operator | (lazy_view<C> lhs, filter<FCT> rhs)
    {
        lhs.m_filter = rhs.value; 
        return lhs;
    }

    template<typename C, typename FCT>
    lazy_view<C> operator | (lazy_view<C> lhs, transform<FCT> rhs)
    {
        lhs.m_transform = rhs.value; 
        return lhs;
    }

    template<typename C>
    lazy_view<C> operator | (lazy_view<C> lhs, take rhs)
    {
        lhs.m_take.m_count = 0;
        lhs.m_take.m_limit = rhs.value; 
        return lhs;
    }
}

