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



// ****************** //
// *** Boost ASIO *** //
// ****************** //
/*  Plesae determine the prototype in sequence of (1), (2) and (3)

                    |     constructor                    function argument
    ----------------+-----------------------------------------------------------
    deadline_timer  |     io_service, interval
    tcp::acceptor   |     io_service, port
    tcp::socket     |     io_service
    ----------------+-----------------------------------------------------------
    async_wait      |                                (1) callback     
    async_accept    |                                    callback, socket
    asycn_read_some |                                    callback, buffer
    ----------------+-----------------------------------------------------------
    callback        | (3) deadline_timer             (2) error_placeholder
    callback        |     tcp::async_accept, socket      error_placeholder
    callback        |     tcp::socket, buffer            error_placeholder, size
*/



class deadline_timer_callback
{
public:
    // (3) callback constructor
    deadline_timer_callback(deadline_timer& _timer, std::uint32_t _count) : timer(_timer), count(_count)
    {
    }
    
    // (2) callback function argument
    void fct(boost::placeholders::error)
    {
        --count;
        if (count==0) return;

        // (1) async function argument 
        timer.async_wait(std::bind(&deadline_timer_callback::fct, this));
    }

private:
    deadline_timer& timer;
    std::uint32_t& count;
};

void run_deadline_timer()
{
    boost::asio::io_service io;
    boost::asio::deadline_timer timer(io, boost::posic_time::seconds(1));
    deadline_timer_callback cb(timer, 100);
    timer.async_wait(std::bind(&deadline_timer_callback::fct, std::ref(cb)));
    io.run();
}

class tcp_server
{
public:
    tcp_server(boost::asio::io_service& _io, std::uint32_t port) : io(_io), acceptor(_io, port)
    {
    }

    void async_run()
    {
        socket = std::make_shared<boost::asio::tcp::socket>(io);
        acceptor->async_wait(*socket, std::bind(&tcp_server::callback, this, boost::placeholders::error));
    }

private:
    void callback(boost::placeholders::error& err)
    {
        socket->set_option(boost::asio::tcp::no_delay);
        auto session = std::make_shared<tcp_session>(socket); 
        session.async_run();
        sessions.insert(session);
        async_run();
    }

private:
    boost::asio::io_service& io;
    boost::asio::tcp::acceptor acceptor;
    std::shared_ptr<boost::asio::tcp::socket> socket;
    std::set<std::shared_ptr<tcp_sessions>> sessions;
};

class tcp_session
{
public:
    tcp_session(std::shared_ptr<boost::asio::tcp::socket>& _socket) : socket(_socket)
    {
    }

    void async_run()
    {
        socket->async_run
        (
            boost::asio::buffer(str),
            std::bind(&tcp_session::callback, this, boost::placeholders::error, boost::placeholders::_1);
        );
    }

private:
    void callback(boost::placeholders::error& err, std::uint32_t size)
    {
        std::cout << str.substr(0,size);
        async_run();
    }

private:
    std::shared_ptr<boost::asio::tcp::socket> socket;
    std::string str;
};

class tcp_client
{
public:
    tcp_client(boost::asio::io_service& _io) : io(_io), socket(_io)
    {
    }

    bool connect(const std::string& ip, std::uint32_t port)
    {
        boost::asio::tcp::resolver resolver(io);
        boost::asio::tcp::query query(io, port);
        boost::asio::tcp::endpoint endpoint = resolver.resolve(query);
        socket.connect(endpoint);
        socket.set_option(boost::asio::tcp::no_delay);
    }

private:
    boost::asio::io_service& io;
    boost::asio::tcp::socket socket;
};
