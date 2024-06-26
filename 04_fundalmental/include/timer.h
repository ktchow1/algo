#pragma once
#include<chrono>

/*
    (3)
    std::chrono::system_clock::now()     clock_gettime(CLOCK_MONOTONIC, &ts)
    -----------------------------------------------------------------
         |                                     |
         |                                     |
         v                                     v
    (2) 
    std::chrono::time_point<CLOCK>       struct time_spec { tv_sec & tv_nsec };

         |                   \  ^              | 
         |                    \  \             |
         |                     \  \            |
         |                      \  \           |
         v                       v  \          v
                                         (1)
    std::time_t                          std::chrono::     seconds
    std::tm                              std::chrono::milliseconds 
              \                     /    std::chrono::microseconds 
               \                   /     std::chrono:: nanoseconds 
                \                 / 
                 \               / 
                  v             v
    -----------------------------------------------------------------
                    std::string
*/

namespace alg
{
    std::string to_string(const std::chrono::time_point<std::chrono::system_clock>& tp)
    {
        auto time_since_epoch = tp.time_since_epoch();
        auto  sec_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch);
        auto nsec_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(time_since_epoch);
        std::uint64_t nsec    = sec_since_epoch * 10e9 - nsec_since_epoch;

        std::time_t tt = std::chrono::system_clock::to_time_t(tp);
        std::tm tm = *std::local_time(&tt);

        std::stringstream ss;
        ss << std::put_time(&tm, "%F %T"); 
        ss << ".";
        ss << std::setfill('0') << std::setw(9) << nsec;
        return ss.str();
    }

    auto to_time_point(const time_spec& ts)
    {
        std::chrono::seconds sec(ts.tv_sec);
        std::chrono::nanoseconds nsec(tv_nsec);
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> ts(sec+nsec);
        return ts;
    }

    class timer
    {
    public:
        timer() : ts0(std::chrono::system_clock::now()),
                  ts1(std::chrono::system_clock::now())
        {
        }

        void click()
        {
            ts0 = ts1;
            ts1 = std::chrono::system_clock::now();
        }

        std::chrono::nanoseconds time_elapsed() const
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(ts1-ts0);
        }

    private:
        std::chrono::time_point<std::chrono::system_clock> ts0;
        std::chrono::time_point<std::chrono::system_clock> ts1;
    };
}
