#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include "web_server.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <sstream>

namespace beast = boost::beast;
using std::string;
using std::vector;
using boost::asio::ip::address;
using boost::asio::io_context;
using tcp = boost::asio::ip::tcp;
using namespace web_server;

int main(){


   address const address = boost::asio::ip::make_address("127.0.0.1");
   unsigned short port = static_cast<unsigned short>(std::atoi("8083"));

   io_context ioc(1);   //is same as io_context ioc{1}
   tcp::acceptor acceptor(ioc,{address,port});
   web_server * server = new web_server

   while(true){
      tcp::socket socket(ioc);
      acceptor.accept(socket);
      std::cout << "socket accepted" << std::endl;

      //spawning a thread to work with accepted socket
      std::thread(process_socket_connection,std::move(socket)).detach();
   }
   return 0;
}