#include "../common/commonheader.hpp"
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
      void connect(string host,string port);
      void disconnect();
      string send_request(string request);
      string listen_for_request();
   class NotConnectedClient : std::exception{};
   private:
      bool connected; 
      net::io_context* ioc;
      websocket::stream<tcp::socket>* ws;
      tcp::resolver* resolver;
   };

   Client_Connection_Resolver::Client_Connection_Resolver(){
      connected = false;
      ioc = new net::io_context();
      resolver = new tcp::resolver(*ioc);
      ws = new websocket::stream<tcp::socket>(*ioc);
   }

   Client_Connection_Resolver::~Client_Connection_Resolver(){
      if(connected)
         disconnect();
      delete ioc;
      delete resolver;
      delete ws;
   }

   void Client_Connection_Resolver::connect(string host,string port){
      tcp::resolver::results_type const results = resolver->resolve(host,port);
      net::connect(ws->next_layer(),results.begin(),results.end());
      ws->set_option(websocket::stream_base::decorator(
          [](websocket::request_type &req) 
          {
             req.set(http::field::user_agent,
                     std::string(BOOST_BEAST_VERSION_STRING) +
                         " websocket-client-coro");
          }));
      ws->handshake(host,"/");
      connected = true;
   }

   void Client_Connection_Resolver::disconnect(){
      ws->close(websocket::close_code::normal);
      connected = false;
   }

   string Client_Connection_Resolver::send_request(string request){
      if(!connected)
         throw NotConnectedClient();
      ws->write(net::buffer(request));
      beast::flat_buffer buffer;
      ws->read(buffer);
      return beast::buffers_to_string(buffer.data());
   }

   string Client_Connection_Resolver::listen_for_request(){

   }

   void login(){
      string username;
      string passw;
      std::cout << "Username: ";
      std::cin >> username;
      std::cout << "Password: ";
      std::cin >> passw;
   }
}