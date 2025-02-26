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
        using  iterator_type = typename C::iterator;
        using    filter_type = std::function<bool(T)>;
        using transform_type = std::function<T(T)>;

        struct take_type
        {
            std::uint32_t m_count;
            std::uint32_t m_limit;
        }; 

        using logic = std::variant<filter_type, transform_type, take_type>;

    public:
        struct iterator
        {
            iterator(lazy_view& view, iterator_type& iter) : m_view(view), m_iter(iter)
            {
            //  if (invalid()) next_valid(); 
            }
            
            void next_valid()
            {
/*              if (m_iter == m_view.m_end) return;
                ++  m_iter;
                m_value = *m_iter;


                bool valid = true;
                for(auto& logic:m_view.m_logics)
                { 
                    if (logic.index() == 0)
                    {
                        if (!std::get<0>(logic)(m_value)) valid = false;
                    }
                    else if (logic.index() == 1)
                    {
                        m_value = std::get<1>(logic)(m_value);
                    }
                    else if (logic.index() == 2)
                    {
                        if (std::get<2>(logic).m_count == 
                            std::get<2>(logic).m_limit) 
                        {
                            m_iter = m_view.m_end;
                            return; 
                        }
                        if (valid)
                        {
                            ++std::get<2>(logic).m_count;
                        }
                    }
                } */
            }

            void operator++()
            {
                next_valid();
            }

            bool operator==(const iterator& rhs) const
            {
                return m_iter == rhs.m_iter;
            }

            const T& operator*() const
            {
                return *m_iter;
            }

            T& operator*()
            {
                return *m_iter;
            }

        public:
            lazy_view&    m_view;
            iterator_type m_iter;
            T             m_value;
        };

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
                bool add_x = true;

                for(auto& logic:m_logics)
                { 
                    if (logic.index() == 0)
                    {
                        if (!std::get<0>(logic)(x)) add_x = false;
                    }
                    else if (logic.index() == 1)
                    {
                        x = std::get<1>(logic)(x);
                    }
                    else if (logic.index() == 2)
                    {
                        if (std::get<2>(logic).m_count == 
                            std::get<2>(logic).m_limit) 
                        {
                            return output; 
                        }
                        if (add_x)
                        {
                            ++std::get<2>(logic).m_count;
                        }
                    }
                }

                if (add_x)
                {
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
        iterator_type      m_begin;
        iterator_type      m_end;    
        std::vector<logic> m_logics;
    };


    // *********************************** //
    // *** Pipe for creating lazy_view *** //
    // *********************************** //
    template<typename C, typename FCT>
    lazy_view<C> operator | (C& lhs, const filter<FCT>& rhs)
    {
        lazy_view<C> output(lhs);
        output.m_logics.push_back(typename lazy_view<C>::filter_type{rhs.value}); 
        return output;
    }

    template<typename C, typename FCT>
    lazy_view<C> operator | (C& lhs, const transform<FCT>& rhs)
    {
        lazy_view<C> output(lhs);
        output.m_logics.push_back(typename lazy_view<C>::transform_type{rhs.value}); 
        return output;
    }

    template<typename C>
    lazy_view<C> operator | (C& lhs, const take& rhs)
    {
        lazy_view<C> output(lhs);
        output.m_logics.push_back(typename lazy_view<C>::take_type{0, rhs.value}); 
        return output;
    }


    // ************************** //
    // *** Pipe for cascading *** //
    // ************************** //
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

