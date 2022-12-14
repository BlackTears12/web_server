#include "client_connection_resolver.hpp"
#include <iostream>
using namespace web_client;
#define host "127.0.0.1"
#define port "8083"

int old_client_connect(int argc, char **argv)
{
   try
   {

      string text = "request";
      // The io_context is required for all I/O
      net::io_context ioc;

      // These objects perform our I/O
      tcp::resolver resolver{ioc};
      websocket::stream<tcp::socket> ws{ioc};

      // Look up the domain name
      auto const results = resolver.resolve(host, port);

      // Make the connection on the IP address we get from a lookup
      net::connect(ws.next_layer(), results.begin(), results.end());

      // Set a decorator to change the User-Agent of the handshake
      ws.set_option(websocket::stream_base::decorator(
          [](websocket::request_type &req) 
          {
             req.set(http::field::user_agent,
                     std::string(BOOST_BEAST_VERSION_STRING) +
                         " websocket-client-coro");
          }));

      // Perform the websocket handshake
      ws.handshake(host, "/");

      // Send the message
      ws.write(net::buffer(std::string(text)));

      // This buffer will hold the incoming message
      beast::flat_buffer buffer;

      // Read a message into our buffer
      ws.read(buffer);

      // Close the WebSocket connection
      ws.close(websocket::close_code::normal);

      // If we get here then the connection is closed gracefully

      // The make_printable() function helps print a ConstBufferSequence
      std::cout << beast::make_printable(buffer.data()) << std::endl;
   }
   catch (std::exception const &e)
   {
      std::cerr << "Error: " << e.what() << std::endl;
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}

int main(int argc,char** argv){
   try{
      Client_Connection_Resolver ccr;
      ccr.connect(host,port);
      std::cout << ccr.send_request("asd");
      ccr.disconnect();
   }catch(std::exception const &e){
      std::cerr << "Error" << e.what() << std::endl;
      return EXIT_FAILURE;
   }
}