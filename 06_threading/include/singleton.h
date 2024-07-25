#pragma once



namespace alg
{
    template<typename T>
    class singleton
    {
    public:




    public:
        const T& value() const noexcept
        {
            return m_impl;
        }

        T& value() noexcept
        {
            return m_impl;
        }

    private:
        T m_impl;
    };
}
