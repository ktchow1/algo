#pragma once
#include<atomic>


namespace alg
{
    // ***************************** //
    // *** Using pthread library *** //
    // ***************************** //
    class spinlock_p final
    {
    public:
        spinlock_p()
        {
            pthread_spin_init(&m_impl, PTHREAD_PROCESS_PRIVATE); // for threads inside same process only
        }

        ~spinlock_p()
        {
            pthread_spin_destroy(&m_impl);
        }

        spinlock_p(const spinlock_p&) = delete;
        spinlock_p(spinlock_p&&) = delete;
        spinlock_p& operator=(const spinlock_p&) = delete;
        spinlock_p& operator=(spinlock_p&&) = delete;

    public:
        inline void lock() noexcept
        {
            pthread_spin_lock(&m_impl);
        }

        inline void unlock() noexcept
        {
            pthread_spin_unlock(&m_impl);
        }

    private:
        pthread_spinlock_t m_impl;
    };


    // ********************************* //
    // *** Using std::thread library *** //
    // ********************************* //
    class spinlock final
    {
    public:
        spinlock()
        {
        }

        ~spinlock()
        {
        }

        spinlock(const spinlock&) = delete;
        spinlock(spinlock&&) = delete;
        spinlock& operator=(const spinlock&) = delete;
        spinlock& operator=(spinlock&&) = delete;

    public:
        inline void lock() noexcept
        {
        }

        inline void unlock() noexcept
        {
        }

    private:
        std::atomic_flag m_flag;
    };
}
