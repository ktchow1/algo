#pragma once
#include<cstdint>
#include<functional>
#include<variant>


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


    // ********************** //
    // *** Container view *** //
    // ********************** //
    template<typename C> // C = container
    struct lazy_view
    {
    public:
        using              T = typename C::value_type;
        using    filter_type = std::function<bool(T)>;
        using transform_type = std::function<T(T)>;

        struct take_type
        {
            std::uint32_t m_count;
            std::uint32_t m_limit;
        }; 

        using logic = std::variant<filter_type, transform_type, take_type>;


    public:
        lazy_view(C& container) : m_begin(container.begin()), m_end(container.end()), m_logics{}
        {
        }

        // *********************************************** //
        // *** Casting operator - with lazy evaluation *** //
        // *********************************************** //
        operator C()
        {
            C output;
            for(auto iter=m_begin; iter!=m_end; ++iter)
            {
                auto x = *iter;
                for(auto& logic:m_logics)
                { 
                    if (logic.index() == 0)
                    {
                        if (!std::get<0>(logic)(x)) break; // break inner-for, but continue outer-for
                    }
                    else if (logic.index() == 1)
                    {
                        x = std::get<1>(logic)(x);
                    }
                    else if (logic.index() == 2)
                    {
                        if (std::get<2>(logic).m_count == 
                            std::get<2>(logic).m_limit) return output;
                        ++  std::get<2>(logic).m_count;
                    }
                    output.push_back(x);
                }
            } 
            return output;
        }

        auto begin() const // Todo : not completed
        {
            return m_begin;
        }

        auto end() const 
        {
            return m_end;
        }

    public:
        typename C::iterator m_begin;
        typename C::iterator m_end;    
        std::vector<logic>   m_logics;
    };


    // ***************************************************************************************************** //
    // *** The pipeline expression (in test) resolves into the following operators, returning lazy_view. *** //
    // ***************************************************************************************************** //
    template<typename C, typename FCT>
    lazy_view<C> operator | (C& lhs, const filter<FCT>& rhs)
    {
        lazy_view<C> output(lhs);
        output.m_logics.push_back(typename lazy_view<C>::filter_type{rhs.value}); 
        return output;
    }

    // more ...



    // ***************** //
    // *** Cascading *** //
    // ***************** //
    template<typename C, typename FCT>
    lazy_view<C> operator | (lazy_view<C>&& lhs, const filter<FCT>& rhs) // Is "lhs" universal reference or rvalue reference?
    {
        lhs.m_logics.push_back(typename lazy_view<C>::filter_type{rhs.value}); 
        return lhs;
    }

    template<typename C, typename FCT>
    lazy_view<C> operator | (lazy_view<C>&& lhs, const transform<FCT>& rhs)
    {
        lhs.m_logics.push_back(typename lazy_view<C>::transform_type{rhs.value}); 
        return lhs;
    }

    template<typename C>
    lazy_view<C> operator | (lazy_view<C>&& lhs, const take& rhs)
    {
        lhs.m_logics.push_back(typename lazy_view<C>::take_type{0, rhs.value});
        return lhs;
    }
}

