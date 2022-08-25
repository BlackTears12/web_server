#pragma once
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define WINDOWS_PLATFORM true
    #include "boost/beast/core.hpp"
    #include "boost/beast/websocket.hpp"
    #include "boost/asio/connect.hpp"
    #include "boost/asio/ip/tcp.hpp"
#else
    #include <boost/beast/core.hpp>
    #include <boost/beast/websocket.hpp>
    #include <boost/asio/connect.hpp>
    #include <boost/asio/ip/tcp.hpp>
#endif