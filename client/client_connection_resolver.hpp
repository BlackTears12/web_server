#include "common/commonheader.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

namespace web_client
{
   class Client_Connection_Resolver{
   public:
      Client_Connection_Resolver();
      ~Client_Connection_Resolver();
      void connect();
      string send_request(string request);
   private:
      bool connected;   
      net::io_context* ioc;
      tcp::resolver* resolver;
      websocket::stream<tcp::socket>* ws;
   };

   Client_Connection_Resolver::Client_Connection_Resolver()
   {
      connected = false;
      ioc = new net::io_context();
   }
   
}